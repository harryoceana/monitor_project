#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "cgic.h"

#define STORAGE_NO	'1'

#define N 32

char errflag = 0;

struct msg
{
	long type;			//消息队列里的消息类型
	long msgtype;		//区别消息类型
	unsigned char text[N];  //消息长度
};

struct msg msg_buf = {0, 0, {0}};

void ctrl_fan() 	//控制风扇
{
	char buf[4] = {0};
	cgiFormString("fan", buf, 4);
	if (strncmp(buf, "off", 3) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (0 << 4) | (0 << 0);
	}
	else if(strncmp(buf,"on1", 3) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (0 << 4) | (1 << 0);
	}
	else if(strncmp(buf, "on2", 3) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (0 << 4) | (2 << 0);
	}
	else 
	{
		errflag = 1;
	}
	msg_buf.type = 1L;		//这个是什么？？
	msg_buf.msgtype = 4L;	//这个是不是为了检测写入消息队列的是什么信息？？
}

void ctrl_led() 		//控制灯
{
	char buf[4] = {0};
	cgiFormString("led", buf, 4);
	if(strncmp(buf,"off", 3) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (1 << 4) | (0 << 0);
	}
	else if (strncmp(buf, "on", 2) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (1 << 4) | (1 << 0);
	} 
	else 
	{
		errflag = 2;
	}
	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
}

void ctrl_water_pump() 		//控制水泵
{
	char buf[4] = {0};
	cgiFormString("led", buf, 4);	//这里led要进行修改，对网页进行修改！！！
	if(strncmp(buf,"off", 3) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (2 << 4) | (0 << 0);
	}
	else if (strncmp(buf, "on", 2) == 0)
	{
		msg_buf.text[0] = ((STORAGE_NO - 48)) << 6 | (2 << 4) | (1 << 0);
	} 
	else 
	{
		errflag = 3;
	}
	msg_buf.type = 1L;
	msg_buf.msgtype = 4L;
}

int cgiMain()
{
//	key_t key; 
//	int msgid; 
	char device[6] = {0};
	
	memset(&msg_buf, 0, sizeof(msg_buf)); 	//对这段内存进行初始化，防止有其他的值存在。

	cgiFormString("device", device, 6);		//看看网页中的device。这个地方要进行修改个字符！！！
	switch(device[0])           		//检测这一位，然后看看控制的是哪种东西
	{
		case 'f': // fan
			ctrl_fan();
			break;
		case 'l': // led
			ctrl_led();
			break;
		case 'w': // 水泵
			ctrl_water_pump();
			break;		
		default : 
			goto err;
	}
	if(errflag) 	//可以手动设置errflag，进行调试
		goto err;
#if 0
	//去掉写入消息队列，调试
	if((key = ftok("/app", 'g')) < 0) 	//生成消息队列需要的key，要更改路径！！！
	{
		perror("ftok"); 	//多个进程通过同一个值来访问消息队列
		exit(1);
	}

	if((msgid = msgget(key, 0666)) < 0)  //创建或者打开消息队列
	{
		perror("msgget");
		exit(1);
	}
	
	msgsnd(msgid, &msg_buf, sizeof(msg_buf)-sizeof(long), 0);  //添加消息到消息队列，OK。再就是读了。
#endif
	device[4] = ' ';	//这是什么？？
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../control1.html\">");
	fprintf(cgiOut, "<H2>设置<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>%s设备掩码%x</H2>", device, msg_buf.text[0]);
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;

err:
	device[4] = ' ';
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../control1.html\">");
	fprintf(cgiOut, "<H2>设置<font color=\"#FF0000\" size=\"+3\">失败！</font>未知错误，请重试！本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2><font color=\"#FF0000\">%s</font>设备掩码<font color=\"#FF0000\">%x</font></H2>", device, msg_buf.text[0]);
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;
}
