// SPDX-License-Identifier: GPL-2.0-or-later

// source has been inspired from
// https://kernel.googlesource.com/pub/scm/linux/kernel/git/stable/linux-stable/+/v4.2.6/samples/kobject/kobject-example.c

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

#define DRIVER_VERSION "1"
#define DRIVER_AUTHOR "shivang upadhyay"
#define DRIVER_DESC "task 9"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	char *id = "bedff03b2176";

	return sysfs_emit(buf, "%s\n", id);
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,
		size_t len)
{
	char *id = "bedff03b2176";

	if (len != 13)
		return -EINVAL;

	if (strncmp(buf, id, len-1))
		return -EINVAL;

	return len;
}

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%ld\n", jiffies);
}

static DEFINE_MUTEX(foo_mutex);
static char foo_data[PAGE_SIZE];

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int size;

	mutex_lock(&foo_mutex);

	size = strlen(foo_data);
	sysfs_emit(buf, "%s\n", foo_data);

	mutex_unlock(&foo_mutex);

	return size;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf,
		size_t len)
{
	if (len > PAGE_SIZE)
		return -EINVAL;

	mutex_lock(&foo_mutex);
	strcpy(foo_data, buf);
	mutex_unlock(&foo_mutex);

	return len;
}

static struct kobj_attribute jiffies_attribute = __ATTR_RO(jiffies);
static struct kobj_attribute id_attribute = __ATTR_RW(id);
static struct kobj_attribute foo_attribute = __ATTR_RW(foo);

static struct attribute *attrs[] = {
	&id_attribute.attr,
	&jiffies_attribute.attr,
	&foo_attribute.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *eudy;

static int __init fs_init(void)
{
	int result;

	eudy = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!eudy) {
		pr_alert("no eudy this time");
		return -ENOMEM;
	}

	result = sysfs_create_group(eudy, &attr_group);
	if (result) {
		pr_alert("supa fail");
		kobject_put(eudy);
	}
	return result;
}


static void __exit fs_exit(void)
{
	kobject_put(eudy);
}

module_init(fs_init);
module_exit(fs_exit);

