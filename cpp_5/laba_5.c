#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define BUFSIZE 100

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");

static int irq = 10;
module_param(irq, int, 0660);

static struct proc_dir_entry *ent;
static int i;

static int factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

static ssize_t writing(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
    int num;
    char buf[BUFSIZE];

    if (*ppos > 0 || count > BUFSIZE)
        return -EFAULT;

    if (copy_from_user(buf, ubuf, count))
        return -EFAULT;

    num = sscanf(buf, "%d", &i);

    if (num != 1)
        return -EFAULT;

    
    int fact_result = factorial(i);
    irq = fact_result;
    printk(KERN_ALERT "mydev: The factorial of %d is %d\n", i, irq);

    *ppos = count;
    return count;
}

static ssize_t reading(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
    char buf[BUFSIZE];
    int len = 0;
    if (*ppos > 0 || count < BUFSIZE)
        return 0;
    len += sprintf(buf, "irq = %d\n", irq);

    if (copy_to_user(ubuf, buf, len))
        return -EFAULT;
    *ppos = len;
    return len;
}

static struct file_operations myops = {
    .owner = THIS_MODULE,
    .read = reading,
    .write = writing,
};

static int simple_init(void) {
    ent = proc_create("mydev", 0660, NULL, &myops);
    printk(KERN_ALERT "hello...\n");
    return 0;
}

static void simple_cleanup(void) {
    proc_remove(ent);
    printk(KERN_WARNING "bye ...\n");
}

module_init(simple_init);
module_exit(simple_cleanup);
