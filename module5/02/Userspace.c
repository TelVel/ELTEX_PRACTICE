#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/version.h>

#define PROC_ENTRY_NAME "proc_comm"
#define MAX_MSG_SIZE 256

static struct proc_dir_entry *proc_entry;
static char *msg;
static size_t msg_len;
static size_t temp_len;

static ssize_t proc_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
    if (count > temp_len) {
        count = temp_len;
    }
    
    if (copy_to_user(buf, msg, count)) {
        return -EFAULT;
    }
    
    temp_len -= count;
    
    if (count == 0) {
        temp_len = msg_len;
    }
    
    return count;
}

static ssize_t proc_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
    if (count > MAX_MSG_SIZE) {
        return -EINVAL;
    }
    
    if (copy_from_user(msg, buf, count)) {
        return -EFAULT;
    }
    
    msg_len = count;
    temp_len = msg_len;
    
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int __init proc_init(void)
{
    msg = kmalloc(MAX_MSG_SIZE, GFP_KERNEL);
    if (!msg) {
        return -ENOMEM;
    }
    
    proc_entry = proc_create(PROC_ENTRY_NAME, 0666, NULL, &proc_fops);
    if (!proc_entry) {
        kfree(msg);
        return -ENOMEM;
    }
    
    msg_len = 0;
    temp_len = 0;
    
    pr_info("Module initialized\n");
    return 0;
}

static void __exit proc_cleanup(void)
{
    remove_proc_entry(PROC_ENTRY_NAME, NULL);
    kfree(msg);
    
    pr_info("Module removed\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oblaukhov P.K.");
MODULE_DESCRIPTION("Kernel module for userspace communication via procfs");
module_init(proc_init);
module_exit(proc_cleanup);