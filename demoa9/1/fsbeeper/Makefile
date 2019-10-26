export ARCH=arm
export CROSS_COMPILE=arm-linux-

#KERNELDIR :=/lib/modules/$(shell uname -r)/build
KERNELDIR :=/home/linux/test/software/kernel/linux-5.1.8/
PWD  :=$(shell pwd)

obj-m +=fspwmbeeper.o 

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	arm-linux-gcc test.c -o music_test

install:
	sudo cp music_test *.ko /nfs/rootfs/drv_module/monitor_project/monitor_demo/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf music_test


