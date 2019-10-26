
SRCFILE=app-zigbeea9.c
APP=zigbeefantest

NFSDIR=/nfs/rootfs

all:
	arm-none-linux-gnueabi-gcc $(SRCFILE) -o $(APP)
install:
		cp $(APP) $(NFSDIR)
clean:
	rm $(APP)
