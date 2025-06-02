obj-m += linkkernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	sudo insmod linkkernel.ko
remove:
	sudo rmmod linkkernel.ko
show:
	dmesg | tail -3
see_info:
	modinfo -a linkkernel.ko
	modinfo -d linkkernel.ko
user:
	gcc user.c -o user
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user