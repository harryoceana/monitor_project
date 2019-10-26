//设置温度光照等的上下限

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

struct msg
{
	long type;
	long msgtype;
	char text[N];
};

struct setEnv
{
	int soil_temMAX; //10-50
	int soil_temMIN; //10-50
	int soil_humMAX; //10-50
	int soil_humMIN; //10-50
	int air_temMAX; //10-50
	int air_temMIN; //10-50
	int air_humMAX; //10-50
	int air_humMIN; //10-50
	int illMAX; //10-500
	int illMIN; //10-500
	int co2MAX;	//暂时未定大小
	int co2MIN;	
	int waterMAX;	//暂时未定大小
	int waterMIN;
};

int cgiMain()
{
	key_t key; 
	int msgid; 
	char buf[5] = {0}; //字符接收缓冲区
	char errflag = 0; //错误标志
	struct setEnv new; //环境变量临时结构体
	struct msg msg_buf = {0, 0, {0}}; //消息结构体
	
	memset(&msg_buf,0,sizeof(msg_buf));

	cgiFormString("temMAX", buf, 5);	//提取表单。看网页，找到这个表单的符号！！！
	new.temMAX = atoi (buf); //字符转换为整型
	cgiFormString("temMIN", buf, 5);	//要修改网页！！！
	new.temMIN = atoi (buf);
	cgiFormString("humMAX", buf, 5);
	new.humMAX = atoi (buf);
	cgiFormString("humMIN", buf, 5);
	new.humMIN = atoi (buf);
	cgiFormString("illMAX", buf, 5);
	new.illMAX = atoi (buf);
	cgiFormString("illMIN", buf, 5);
	new.illMIN = atoi (buf);

	if(new.soil_temMAX < new.soil_temMIN || new.soil_temMAX > 100)  //soil
		errflag |= 1 << 11; 	//这个是否可以为十进制数？？
	if(new.soil_temMAX < new.soil_temMIN || new.soil_temMIN < 0)
		errflag |= 1 << 10;		
	if(new.soil_humMAX < new.soil_humMIN || new.soil_humMAX > 100)
		errflag |= 1 << 9;
	if(new.soil_humMAX < new.soil_humMIN || new.soil_humMIN < 0)
		errflag |= 1 << 8;		
	if(new.air_temMAX < new.air_temMIN || new.air_temMAX > 100)
		errflag |= 1 << 7;
	if(new.air_temMAX < new.air_temMIN || new.air_temMIN < 0)
		errflag |= 1 << 6;		
	if(new.air_humMAX < new.air_humMIN || new.air_humMAX > 100)
		errflag |= 1 << 5;
	if(new.air_humMAX < new.air_humMIN || new.air_humMIN < 0)
		errflag |= 1 << 4;		
	if(new.illMAX < new.illMIN || new.illMAX > 4000)
		errflag |= 1 << 3;
	if(new.illMAX < new.illMIN || new.illMIN < 0)
		errflag |= 1 << 2;
	if(new.co2MAX < new.co2MIN || new.co2MAX > 40)  //要进行设置最大值!!!
		errflag |= 1 << 1;
	if(new.co2MAX < new.co2MIN || new.co2MIN < 0)	
		errflag |= 1 << 0;
	if(errflag)
		goto skip;
#if 1
	if((key = ftok("/app", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}

	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
		
	msg_buf.type = 1L;
	msg_buf.msgtype = 5L;
	msg_buf.text[0] = STORAGE_NO;
	memcpy (msg_buf.text+1, &new, 24);
	
	msgsnd(msgid, &msg_buf, sizeof(msg_buf)-sizeof(long), 0);
#endif
skip:	
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../setting1.html\">");
	if(errflag)
		fprintf(cgiOut, "<H2>提交<font color=\"#FF0000\" size=\"+3\">失败！</font>请仔细检查参数设置！本页面3秒后自动关闭。</H2>");
	else
		fprintf(cgiOut, "<H2>提交<font color=\"#FF0000\" size=\"+3\">成功！</font>请点击环境信息查看！本页面3秒后自动关闭。</H2>");	
	if(errflag & (1 << 11))  //是否识别十进制？？
		fprintf(cgiOut, "<H2>土壤温度上限：<font color=\"#FF0000\">%d</font></H2>", new.soil_temMAX);
	else
		fprintf(cgiOut, "<H2>土壤温度上限：%d</H2>", new.soil_temMAX);
	if(errflag & (1 << 10))		//所有的数字都没有进行修改！！！
		fprintf(cgiOut, "<H2>土壤温度下限：<font color=\"#FF0000\">%d</font></H2>", new.soil_temMIN);
	else
		fprintf(cgiOut, "<H2>土壤温度下限：%d</H2>", new.soil_temMIN);
	if(errflag & (1 << 9))
		fprintf(cgiOut, "<H2>土壤湿度上限：<font color=\"#FF0000\">%d</font></H2>", new.soil_humMAX);
	else
		fprintf(cgiOut, "<H2>土壤湿度上限：%d</H2>", new.soil_humMAX);
	if(errflag & (1 << 8))
		fprintf(cgiOut, "<H2>土壤湿度下限：<font color=\"#FF0000\">%d</font></H2>", new.soil_humMIN);
	else
		fprintf(cgiOut, "<H2>土壤湿度下限：%d</H2>", new.soil_humMIN);
		
	if(errflag & (1 << 11))  //是否识别十进制？？
		fprintf(cgiOut, "<H2>空气温度上限：<font color=\"#FF0000\">%d</font></H2>", new.air_temMAX);
	else
		fprintf(cgiOut, "<H2>空气温度上限：%d</H2>", new.air_temMAX);
	if(errflag & (1 << 10))
		fprintf(cgiOut, "<H2>空气温度下限：<font color=\"#FF0000\">%d</font></H2>", new.air_temMIN);
	else
		fprintf(cgiOut, "<H2>空气温度下限：%d</H2>", new.air_temMIN);
	if(errflag & (1 << 9))
		fprintf(cgiOut, "<H2>空气湿度上限：<font color=\"#FF0000\">%d</font></H2>", new.air_humMAX);
	else
		fprintf(cgiOut, "<H2>空气湿度上限：%d</H2>", new.air_humMAX);
	if(errflag & (1 << 8))
		fprintf(cgiOut, "<H2>空气湿度下限：<font color=\"#FF0000\">%d</font></H2>", new.air_humMIN);
	else
		fprintf(cgiOut, "<H2>空气湿度下限：%d</H2>", new.air_humMIN);
		
	if(errflag & (1 << 1))
		fprintf(cgiOut, "<H2>光度上限：<font color=\"#FF0000\">%d</font></H2>", new.illMAX);
	else
		fprintf(cgiOut, "<H2>光度上限：%d</H2>", new.illMAX);
	if(errflag & (1 << 0))
		fprintf(cgiOut, "<H2>光度下限：<font color=\"#FF0000\">%d</font></H2>", new.illMIN);
	else
		fprintf(cgiOut, "<H2>光度下限：%d</H2>", new.illMIN);
		
		
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	
	return 0; 
}
