#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/msg.h>
#include <errno.h>

#include "./cgic.h"
#include "./all.h"
#include "./struct.h"


int errflag = 0;
int zigbee_fd = 0;
int msgid = 0;

void ctrl_pump()
{
	char buf[4] = {0};
	int cmd;
	cgiFormString("pump", buf, 4);
	if (strncmp(buf, "on", 2) == 0) {
		cmd = 0x41;
	}
	else if (strncmp(buf, "off", 3) == 0) { 
		cmd = 0x40;
	}else {
		errflag = 1;
		return ;
	}
	write(zigbee_fd, &cmd, 1);
}

void ctrl_fan()
{
	char buf[4] = {0};
	int cmd;
	cgiFormString("fan", buf, 4);
	if (strncmp(buf, "on", 2) == 0){ 
		//		cmd = 0x21;
		cmd = 0x31;
	}
	else if (strncmp(buf, "off", 3) == 0) {
		//		cmd = 0x20;
		cmd = 0x30;
	}else {
		errflag = 1;
		return ;
	}

	key_t key;

	/*创建消息队列*/
	if ((key = ftok("/lib", 'a')) < 0) {
		perror("ftok msgqueue");
		return;
	}
	if ((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
		if (errno == EEXIST) {
			msgid = msgget(key, 0666);
		}else{
			perror("msgget msgid");
			return;
		}
	}
	
	struct message msgbuf;
	msgbuf.type = 1L;
	msgbuf.msg_type = 3L;
	msgbuf.text.fan = cmd;
	msgsnd (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 0);
}

void ctrl_beep()
{
	char buf[4] = {0};
	int cmd;
	cgiFormString("beep", buf, 4);
	if (strncmp(buf, "on", 2) == 0) {
		cmd = 0x61;
	}else if (strncmp(buf, "off", 3) == 0) {
		cmd = 0x60;
	}else {
		errflag = 1;
		return ;
	}
	write(zigbee_fd, &cmd, 1);
}

void tty_set()
{
	struct termios old, new_attr;
	tcgetattr(zigbee_fd, &old);
	new_attr = old;
	cfsetispeed(&new_attr, B115200); 
	cfsetospeed(&new_attr, B115200);
	new_attr.c_cflag |= (CLOCAL | CREAD);
	new_attr.c_cflag &= ~CSIZE;
	new_attr.c_cflag |= CS8;
	new_attr.c_cflag &= ~CSTOPB;	
	new_attr.c_cflag &= ~PARENB;
	new_attr.c_cflag &= ~CRTSCTS;
	new_attr.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	new_attr.c_iflag |= (IXON | IXOFF | IXANY);
	new_attr.c_oflag &= ~OPOST;
	//	new_attr.c_cc[VMIN] = 0;
	//	new_attr.c_cc[VTIME] = 0; 

	if (-1 == tcsetattr(zigbee_fd, TCSANOW, &new_attr)) {
		perror("set ttyusbx error");
		errflag = 1;
	}

}

int cgiMain()
{
	char device[10] = {0};

	if ((zigbee_fd = open(ZIGBEE_PATH, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
		perror("open zigbee error");
		errflag = -1;
	}
	tty_set();
	tcflush(zigbee_fd, TCIOFLUSH);

	cgiFormString("device", device, 6);
	switch(device[0])
	{
	case 'f': //fan
		ctrl_fan();
		break;
	case 'b': //beep
		ctrl_beep();
		break;
	default :
		goto err;
	}
	if(errflag) //for debug
		goto err;

	errflag = 0;
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../control1.html\">");
	fprintf(cgiOut, "<H2>设置<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	sleep(1);
	close(zigbee_fd);
	return 0;
err:
	errflag = 0;
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../control1.html\">");
	fprintf(cgiOut, "<H2>设置<font color=\"#FF0000\" size=\"+3\">失败！</font>未知错误，请重试！本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	sleep(1);
	if (zigbee_fd != -1) {
		close(zigbee_fd);
	}
	return 0;
}




