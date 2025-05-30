obj-m += Lamps.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
insert:
	sudo insmod Lamps.ko
remove:
	sudo rmmod Lamps.ko
show:
	dmesg | tail -1
see_info:
	modinfo -a Lamps.ko
	modinfo -d Lamps.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean