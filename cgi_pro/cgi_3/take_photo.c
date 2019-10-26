#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "./cgic.h"
#include "./struct.h"
#define N 8
#if 0
struct msg
{
	long type;
	long msgtype;
	unsigned char text[N];
};
#endif
int cgiMain()
{
	key_t key; 
	int msgid; 
	char buf[2] = {0};
//	struct msg msg_buf = {0, 0, {0}};
	
//	memset(&msg_buf, 0, sizeof(msg_buf));
	if((key = ftok("/lib", 'a')) < 0)
	{
		perror("ftok");
		exit(1);
	}
	if ((msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		if (errno == EEXIST) {
			msgid = msgget(key, 0666);
		}else{
			perror("msgget msgid");
			return 0;
		}
	}
#if 0 
	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
#endif
	
	struct message msgbuf;
	msgbuf.type = 1L;
	msgbuf.msg_type = 2L;

	cgiFormString("mode", buf, 2);
	if(buf[0] <= '0' || buf[0] > '9')
		goto err;
	
	msgbuf.text.camera = buf[0] - '0';
	msgsnd (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 0);

#if 0
	msg_buf.type = 1L;
	msg_buf.msgtype = 2L;
	msg_buf.text[0] = buf[0] - '0';
	
	msgsnd(msgid, &msg_buf, sizeof(msg_buf)-sizeof(long), 0);
#endif
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../vedio.html\">");
	fprintf(cgiOut, "<H2>抓拍<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>您抓拍了%d张</H2>", buf[0] - '0');
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;

err:
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../vedio.html\">");
	fprintf(cgiOut, "<H2>抓拍<font color=\"#FF0000\" size=\"+3\">失败！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>您抓拍了0张</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;	
}
