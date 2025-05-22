obj-m += HelloWorldKernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	sudo insmod HelloWorldKernel.ko
remove:
	sudo rmmod HelloWorldKernel.ko
show:
	dmesg | tail -1
see_info:
	modinfo -a HelloWorldKernel.ko
	modinfo -d HelloWorldKernel.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean