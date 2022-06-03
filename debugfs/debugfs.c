// SPDX-License-Identifier: GPL-2.0-or-later

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#define DRIVER_VERSION "1"
#define DRIVER_AUTHOR "shivang upadhyay"
#define DRIVER_DESC "task 8"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");


// source inpired from problem in
// https://stackoverflow.com/questions/26272068/implement-a-write-function-to-a-debugfs-file
char *id = "bedff03b2176";
const int id_size = 12;
static struct dentry *dir;
static struct dentry *file;
static ssize_t fs_write(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	int result;
	char ourbuf[13];

	if (strlen(id) != len-1)
		return -EINVAL;

	result = simple_write_to_buffer(ourbuf, len, ppos, buf, len);
	if (result < 0)
		return result;

	if (strncmp(ourbuf, id, len-1) != 0)
		return -EINVAL;

	return len;
}

static ssize_t fs_read(struct file *file, char *buf, size_t len,
		loff_t *ppos)
{
	return simple_read_from_buffer(buf, len, ppos, id, 12);
}



static const  struct file_operations fs_fops = {
	.owner = THIS_MODULE,
	.read = fs_read,
	.write = fs_write,
};

static int __init fs_init(void)
{
	pr_info("loadeing eudyptula debugfs module");

	dir = debugfs_create_dir("eudyptula", NULL);
	if (dir == NULL) {
		pr_alert("cant create eudyptule directory");
		return -ENOMEM;
	}
	file = debugfs_create_file("id", 0644, dir, NULL, &fs_fops);
	if (file == NULL) {
		pr_alert("cant create file");
		debugfs_remove(dir);
		return -ENOMEM;
	}
	return 0;
}


static void __exit fs_exit(void)
{
	pr_info("eudyptula module is unloaded");
	debugfs_remove_recursive(dir);
}

module_init(fs_init);
module_exit(fs_exit);

