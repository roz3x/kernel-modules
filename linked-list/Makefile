obj-m += hello.o
ksource := /lib/modules/$(shell uname -r)/build
all:
	make -C $(ksource) M=$(shell pwd) modules
clean:
	make -C $(ksource) M=$(shell pwd) clean

