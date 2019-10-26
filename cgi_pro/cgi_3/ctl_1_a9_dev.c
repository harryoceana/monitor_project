#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./cgic.h"
#include "./all.h"




int errflag = 0;
void ctrl_led()
{
	char buf[4] = {0};
	int dev_led_fd = 0;
	if ((dev_led_fd = open(LED_PATH, O_RDWR)) < 0) {
		perror("open led driver error");
		errflag = 1;
		return ;
	}
	cgiFormString("led", buf, 4);
	if (strncmp(buf, "on", 2) == 0) {
		ioctl(dev_led_fd, DRV_LED_ON, 1);	
	}
	else if(strncmp(buf, "off", 3) == 0){
		ioctl(dev_led_fd, DRV_LED_OFF, 1);	
	}else
		errflag = 1;
}


int cgiMain()
{
	char device[10] = {0};

	cgiFormString("device", device, 6);
	switch(device[0])
	{
	case 'l': //led
		ctrl_led();
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




