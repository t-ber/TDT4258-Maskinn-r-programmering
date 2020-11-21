/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <asm/signal.h>
#include <linux/signal.h>
#include <asm/siginfo.h>

// #include "driver-gamepad.h"
#include "efm32gg.h"

#define DRIVER_NAME "gamepad_driver"

#define GPIO_MEM_SIZE 0x120

#define IRQ_NUM_GPIO_EVEN 17
#define IRQ_NUM_GPIO_ODD 18

irqreturn_t GPIO_IRQHandler(int irq, void *dev_id, struct pt_regs *regs);

static ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp);
static int gamepad_open(struct inode *inode, struct file *filp);
static int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_fasync(int fd, struct file *filp, int mode);

struct cdev cdev;
dev_t devno;
struct class *cl;
struct resource *gpio;
struct fasync_struct *async_queue;
uint8_t button_status;

struct file_operations gamepad_fops = {
	.owner = THIS_MODULE,
	.read = gamepad_read,
	.write = gamepad_write,
	.open = gamepad_open,
	.release = gamepad_release,
	.fasync = gamepad_fasync,
};

/*
 *
 *		INTERRUPT HANDLER 
 * 
 */

irqreturn_t GPIO_IRQHandler(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("driver: interrupt received\n");	
	
	// Write the pressed buttons to the button status variable
	button_status = ioread32(GPIO_IF);

	// Clear interrupt flags
	iowrite32(0xff, GPIO_IFC);

	// Notify user application(s) through async signal
	if (async_queue) {
		kill_fasync(&async_queue, SIGIO, POLL_IN);
	}

	return IRQ_HANDLED;
}

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
	// Allocate major and minor numbers
	int err = alloc_chrdev_region(&devno, 0, 1, DRIVER_NAME);

	if (err < 0) {
		printk("driver: can't get major number.\n");
		return err;
	}

	// Register device
	cdev_init(&cdev, &gamepad_fops);
	cdev.owner = THIS_MODULE;
	err = cdev_add(&cdev, devno, 1);

	if (err < 0) {
		printk("driver: error %d adding gamepad.\n", err);
		return err;
	}

	// Make device visible
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, devno, NULL, DRIVER_NAME);

	// Allocate the GPIO part of memory
	gpio = request_mem_region(GPIO_PA_BASE, GPIO_MEM_SIZE, DRIVER_NAME);

	if (gpio != NULL) {
		printk("driver: GPIO memory allocated to gamepad_driver.\n");
	}

	// Set up the buttons, like in previous exercises
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xff, GPIO_PC_DOUT);

	// Request interrupt lines for even and odd gpio interrupt
	request_irq(IRQ_NUM_GPIO_EVEN, (irq_handler_t) GPIO_IRQHandler, 0, DRIVER_NAME, &cdev);
	request_irq(IRQ_NUM_GPIO_ODD, (irq_handler_t) GPIO_IRQHandler, 0, DRIVER_NAME, &cdev);

	// Enable GPIO interrupt generation, like in previous exercises
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xff, GPIO_EXTIFALL);
	iowrite32(0xff, GPIO_IEN);

	// Initialize button_status
	button_status = 0x00;

	printk("driver: Hello World, here is your gamepad speaking\n");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	// Undo allocations from init, reverse order
	// Release interrupt lines
	free_irq(IRQ_NUM_GPIO_EVEN, &cdev);
	free_irq(IRQ_NUM_GPIO_ODD, &cdev);

	// Release GPIO memory region
	release_mem_region(GPIO_PA_BASE, GPIO_MEM_SIZE);

	// Delete cdev
	cdev_del(&cdev);

	// Unregister major and minor numbers
	unregister_chrdev_region(devno, 1);

	printk("driver: Short life for a small module...\n");
}

/*
 *	file op functions
 */

static int gamepad_open(struct inode *inode, struct file *filp)
{
	// We don't do anything here
	printk("driver: gamepad driver opened.\n");
	return 0;
}

static int gamepad_release(struct inode *inode, struct file *filp)
{
	// remove from async queue (signals)
	gamepad_fasync(-1, filp, 0);

	printk("driver: gamepad driver released.\n");
	return 0;
}

// Sends an 8 bit number showing the status of each of the 8 buttons: 1 = pressed
static ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	// We send the status of the buttons to the user application
	copy_to_user(buf, &button_status, 1);
	
	printk("driver: gamepad driver read.\n");
	return 1;
}

static ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	// The driver has no write functionality
	
	printk("driver: gamepad driver written to.\n");
	return 0;
}

static int gamepad_fasync(int fd, struct file *filp, int mode)
{
	// Register user application for async signals
	
	printk("driver: fasync invoked.\n");
	return fasync_helper(fd, filp, mode, &async_queue);
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for TDT4258 Gamepad.");
MODULE_LICENSE("GPL");
