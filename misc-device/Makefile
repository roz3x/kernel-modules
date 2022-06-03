# SPDX-License-Identifier: GPL-2.0
# 
# source has been taken from drivers/hid/usbhid/Makefile
#
CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n
obj-m += misc.o
ksource := /lib/modules/$(shell uname -r)/build

all:
	make -C $(ksource) M=$(shell pwd) modules

clean:
	make -C $(ksource) M=$(shell pwd) clean
