obj-m += Chrdev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	sudo insmod Chrdev.ko
remove:
	sudo rmmod Chrdev.ko
show:
	dmesg | tail -3
see_info:
	modinfo -a Chrdev.ko
	modinfo -d Chrdev.ko
user:
	gcc user.c -o user
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm user