// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * source has been inspired from  https://gist.github.com/17twenty/6313566
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#define DRIVER_VERSION "1"
#define DRIVER_AUTHOR "shivang upadhyay"
#define DRIVER_DESC "task 6"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");


char *id = "bedff03b2176";
const int id_size = 12;

static int misc_open(struct inode *inode, struct file *file)
{
	pr_info("eudyptula module is open");
	return 0;
}

static int misc_close(struct inode *inodep, struct file *file)
{
	pr_info("eudyptula module is closed");
	return 0;
}


// source https://www.kernel.org/doc/htmldocs/kernel-api/API---copy-from-user.html
// https://www.linuxtopia.org/online_books/Linux_Kernel_Module_Programming_Guide/x773.html
// https://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html
static ssize_t misc_write(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	char inputbuf[13];

	if (len != id_size+1)
		return -EINVAL;

	if (copy_from_user(inputbuf, buf, id_size + 1))
		return -EINVAL;

	if (strncmp(id, inputbuf, id_size))
		return -EINVAL;

	return len;
}

// source inspired from https://www.quora.com/Linux-Kernel-How-does-copy_to_user-work
static ssize_t misc_read(struct file *file, char *buf, size_t len,
		loff_t *ppos)
{
	if (*ppos != 0)
		return 0;
	*ppos += len;
	if (copy_to_user(buf, id, id_size)) {
		pr_alert("something went wrong");
		return -1;
	}
	return id_size;
}
static const struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.write = misc_write,
	.open = misc_open,
	.release = misc_close,
	.llseek = no_llseek,
	.read = misc_read,
};



// registering
struct miscdevice misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &misc_fops,
};

static int __init misc_init(void)
{
	int result;

	result = misc_register(&misc_device);
	if (result) {
		pr_alert("cant load the eudyptula modules :(");
		return result;
	}
	pr_info("eudyptula module is loaded");
	return 0;
}


static void __exit misc_exit(void)
{
	misc_deregister(&misc_device);
	pr_info("eudyptula module is unloaded");
}

module_init(misc_init);
module_exit(misc_exit);

