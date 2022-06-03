/*
 * source more or less has been grabbed from 
 * https://tldp.org/LDP/lkmpg/2.6/html/x121.html 
 */



#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

int init_module(void) {
    printk(KERN_DEBUG "Hello World!");
    return 0;
}

void cleanup_module(void) {
    return;
}

