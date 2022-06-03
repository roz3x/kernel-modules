// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * source has been taken from  https://www.kernel.org/doc/htmldocs/writing_usb_driver/basics.html
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

#define DRIVER_VERSION "1"
#define DRIVER_AUTHOR "shivang upadhyay"
#define DRIVER_DESC "task 5"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

static const struct usb_device_id id_table[] = {
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
		USB_INTERFACE_SUBCLASS_BOOT,
		USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{}
};

MODULE_DEVICE_TABLE(usb, id_table);

static int __init kb_init(void)
{
	pr_alert("hello world");
	return 0;
}


static void __exit kb_exit(void)
{
}

module_init(kb_init);
module_exit(kb_exit);

