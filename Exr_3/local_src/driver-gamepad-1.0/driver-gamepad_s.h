#ifndef DRIVER_GAMEPAD_S_H
#define DRIVER_GAMEPAD_S_H

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
};

irqreturn_t GPIO_IRQHandler(int irq, void *dev_id, struct pt_regs regs*);

static ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp);
static int gamepad_open(struct inode *inode, struct file *filp);
static int gamepad_release(struct inode *inode, struct file *filp);
static int gamepad_fasync(int fd, struct file *filp, int mode);

static int __init gamepad_init(void);
static void __exit gamepad_cleanup(void);

#endif