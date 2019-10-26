#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./cgic.h"
#include "./fs4412_pwm.h"

int errflag = 0;
void ctrl_pwm()
{
	char buf[4] = {0};
	int dev_pwm_fd = 0;
	//	dev_pwm_fd = open("/dev/pwm", O_RDWR);

	if ((dev_pwm_fd = open("/dev/buzzer", O_RDWR)) < 0) {
		perror("open pwm driver error");
		errflag = 1;
		return ;
	}
	cgiFormString("beep", buf, 4);

	if (strncmp(buf, "on", 2) == 0) {
//		printf("PWM_ON = %#x\n", PWM_ON);
		ioctl(dev_pwm_fd,PWM_ON);
	} else if (strncmp(buf, "off", 3) == 0) {
		ioctl(dev_pwm_fd, PWM_OFF);	
//		printf("pwm off");
	} else
		errflag = 1;

	//	printf("fd_pwm = %d led_fd = %d\n",dev_pwm_fd, dev_led_fd);
}

int cgiMain()
{
	char device[10] = {0};

	cgiFormString("device", device, 6);

//	printf("Content- type: text/html\n\n");
//	printf("%s\n", device);
	switch(device[0])
	{
	case 'b': //led
		ctrl_pwm();
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

	return 0;
}




