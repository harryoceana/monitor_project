#C=arm-cortex_a8-linux-gnueabi-gcc
#CC=/usr/local/toolchain/toolchain-4.5.1/bin/arm-none-linux-gnueabi-gcc
CC=/home/linux/gcc-4.6.4/bin/arm-none-linux-gnueabi-gcc

AFLAGS=-Wall -g -L ./lib -lsqlite3

OBJS=beep.cgi led.cgi capture.cgi picture1.cgi picture2.cgi ctl_1_a9_dev.cgi ctl_1_a9_dev_beep.cgi ctl_1_zigbee_dev.cgi take_photo.cgi\
show_photo.cgi m0_seg.cgi env_1_a9_info.cgi env1.cgi env2.cgi set_1_a9_env.cgi setEnv.cgi login.cgi set_sms.cgi sql1.cgi sql2.cgi set_msg.cgi setting1.cgi

run:$(OBJS)
	mv *.cgi ./cgi-bin
$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< cgic.c -o $@

install:
	sudo cp ./cgi-bin/*.cgi /nfs/rootfs/boa/cgi-bin/
.PHONY:clean
clean:
	rm -rf cgi-bin/*

