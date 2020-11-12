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


#include "efm32gg.h"

#define DRIVER_NAME "gamepad_driver"

#define IRQ_NUM_GPIO_EVEN 17
#define IRQ_NUM_GPIO_ODD 18

struct gamepad_dev {
	struct cdev cdev;
	dev_t devno;
	struct class *cl;
	struct resource *gpio;
	struct fasync_struct *async_queue;
	uint8_t button_status;
}

struct gamepad_dev *dev;

// Deklarerer GPIO Interrupt Handler
irqreturn_t GPIO_IRQHandler(int irq, void *dev_id, struct pt_regs regs*);

// Deklarerer fops funksjoner
ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp);
int gamepad_open(struct inode *inode, struct file *filp);
int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_fasync(int fd, struct file *filp, int mode);

struct file_operations gamepad_fops = {
	.owner = THIS_MODULE,
	.read = gamepad_read,
	.write = gamepad_write,
	.open = gamepad_open,
	.release = gamepad_release,
	.fasync = gamepad_fasync,
};

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
	int err = alloc_chrdev_region(&dev->devno, 0, 1 DRIVER_NAME);

	if (err < 0) {
		printk(KERN_WARNING "gamepad_driver: can't get major number\n");
		return err;
	}

	// Register device
	cdev_init(&dev->cdev, &gamepad_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, &dev->devno, 1);

	if (err < 0) {
		printk(KERN_NOTICE "Error %d adding gamepad.", err);
		return err;
	}

	// Make device visible
	dev->cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, dev->devno, NULL, DRIVER_NAME);

	// Allocate the GPIO part of memory
	dev->gpio = request_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE, DRIVER_NAME);

	if (dev->gpio != NULL) {
		printk("GPIO memory allocated to gamepad_driver.");
	}

	// Set up the buttons, like in previous exercises
	iowrite32(0x33333333, GPIO_PC_MODEL);
	iowrite32(0xff, GPIO_PC_DOUT);

	// Request interrupt lines for even and odd gpio interrupt
	request_irq(IRQ_NUM_GPIO_EVEN, GPIO_IRQHandler, 0, DRIVER_NAME, &dev->cdev);
	request_irq(IRQ_NUM_GPIO_ODD, GPIO_IRQHandler, 0, DRIVER_NAME, &dev->cdev);

	// Enable GPIO interrupt generation, like in previous exercises
	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xff, GPIO_EXTIFALL);
	iowrite(0xff, GPIO_IEN);

	// Initialize button_status
	dev->button_status = 0x00;

	printk("Hello World, here is your gamepad speaking\n");
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
	free_irq(IRQ_NUM_GPIO_EVEN, dev->cdev);
	free_irq(IRQ_NUM_GPIO_ODD, dev->cdev);

	// Release GPIO memory region
	release_mem_region(GPIO_PA_BASE, GPIO_IFC - GPIO_PA_BASE);

	// Delete cdev
	cdev_del(dev->cdev);

	// Unregister major and minor numbers
	unregister_chrdev_region(dev->devno, 1);

	printk("Short life for a small module...\n");
}

/*
 *	file op functions
 */

int gamepad_open(struct inode *inode, struct file *filp)
{
	// We don't do anything here
	printk("Gamepad driver opened.");
	return 0;
}

int gamepad_release(struct inode *inode, struct file *filp)
{
	// remove from async queue (signals)
	gamepad_fasync(-1, filp, 0);

	printk("Gamepad driver released.");
	return 0;
}

// Sender et 8 bits tall som viser hvilke knapper som har vært trykket siden sist
// Alternativ: vise hvilke knapper som er trykket nå
ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	// We send the status of the buttons to the user application
	copy_to_user(buf, &dev->button_status, 1);

	// Clear the buttons_status variable
	// NB! This would be a problem if there was several user applications using the driver
	// In that case, we would have to make use of filp->private_data
	dev->button_status = 0x00;

	return 1;
}

ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	// The driver has no write functionality
	return 0;
}

static int gamepad_fasync(int fd, struct file *filp, int mode)
{
	// Register user application for async signals
	return fasync_helper(fd, filp, mode, &dev->async_queue);
}

/*
 *	Interrupt handler
 */

irqreturn_t GPIO_IRQHandler(int irq, void *dev_id, struct pt_regs regs*)
{
	// Write the pressed buttons to the button status variable
	dev->button_status |= ioread32(GPIO_IF);

	// Clear interrupt flags
	iowrite32(0xff, GPIO_IFC);

	// Notify user application(s) through async signal
	if (dev->async_queue) {
		kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
	}

	return IRQHANDLED;
}


module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Driver for TDT4258 Gamepad.");
MODULE_LICENSE("GPL");
