#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

static int device_major = 0;
static char *device_name = "fengtian_char";
static int device_open(struct inode *inode, struct file *file);
static ssize_t device_write(struct file *file, const char __user *buffer, ssize_t length, loff_t* offset);

static struct file_operations fops = {
    .open = device_open,
    .write = device_write,
};

static int __init crashdriver_init(void) {
    device_major = register_chrdev(0, device_name, &fops);
    if (device_major < 0) {
        printk(KERN_ALERT "注册字符设备失败 device_major=%d\n", device_major);
        return device_major;
    }
    printk(KERN_INFO "主设备号=%d", device_major);
    return 0;
}

static void __exit crashdriver_exit(void) {
    unregister_chrdev(device_major, device_name);
    printk(KERN_ALERT "卸载驱动 major=%d, device_name=%s\n", device_major, device_name);
}

static irqreturn_t crash_interrupt(int irq, void *dev_id) {
    int *crash_ptr = NULL;
    printk(KERN_INFO "interrupt 处理开始");
    *crash_ptr = 100;
    printk(KERN_INFO "interrupt 结束");
    return IRQ_HANDLED;
}

static int device_open(struct inode *inode, struct file * file)
{
    if (request_irq(12345, crash_interrupt, 0, device_name, NULL)){
        printk(KERN_ALERT "can't register interrupt");
        return -EIO;
    }
    printk(KERN_ALERT "open device , register interrupt\n");
    return 0;
}

static ssize_t device_write(struct inode* inode, struct file * file, ssize_t length, loff_t *offset)
{
    printk(KERN_ERR "begin device_write\n");
    force_irq(12345);
    printk(KERN_ERR "write length=%d finish device write", length);
    return length;
