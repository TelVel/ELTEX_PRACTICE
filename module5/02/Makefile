obj-m += Userspace.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	sudo insmod Userspace.ko
remove:
	sudo rmmod Userspace.ko
show:
	dmesg | tail -1
see_info:
	modinfo -a Userspace.ko
	modinfo -d Userspace.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean