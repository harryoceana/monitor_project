export ARCH=arm
export CROSS_COMPILE=arm-linux-

#KERNELDIR :=/lib/modules/$(shell uname -r)/build
KERNELDIR :=/home/fengjunhui/kernel/linux-3.14
PWD  :=$(shell pwd)

obj-m += fsmpu6050.o  

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	arm-linux-gcc test.c -o mpu6050_test

install:
	cp  mpu6050_test *.ko ~/source/rootfs/drv/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf mpu6050_test


