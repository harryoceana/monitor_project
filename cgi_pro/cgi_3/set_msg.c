#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "./cgic.h"
#include "./struct.h"
#include "./sqlite3.h"
#include "./all.h"
#include <termios.h>
#include <fcntl.h>

#define DATAFILE "C:data.txt"
#define CENTER_DATAFILE "C:cen_data.txt"
#define N 32
#define M 1024 //2015116 lyf modify
#if 0
struct msg
{
	long type;
	long msgtype;
	char text[N];
};
#endif
struct setMsg{
	char phone_num[12];
	char center_num[12];
};

struct setMsg phone = {{0}, {0}};
int nophonenum(const char s[])
{
	int i = 0;
	if(s[11] != '\0')
		return 1;
	if(s[0] != '1')
		return 1;
	for(i = 1; i < 11; i++)
	{
		if(s[i] < '0' || s[i] > '9')
			return 1;
	}
	return 0;
}
#if 1 //20150116 lyf modify
	int errflag = 0;
	void db_update()
	{
		sqlite3 *db;
		char *errmsg;
		char sql[M] = {0};
		if(sqlite3_open("/smartfarm.db",&db) != SQLITE_OK)
		{
			errflag = 1;
			return ;
		}
		errflag = 0;
		sprintf(sql,"update env set telephoneNUM=%s, centernumber=%s where farm_no=1;", \
			phone.phone_num,phone.center_num);
		int err;
		if((err = sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
		{
			errflag = 1;
		}
		sqlite3_close(db);
		return ;
	}
#endif

int cgiMain()
{
	key_t key; 
	int msgid; 

	cgiFormString("phonenum", phone.phone_num, 12);
	cgiFormString("centernum", phone.center_num, 12);

	if(nophonenum(phone.phone_num))
		goto errphone;
	if(nophonenum(phone.center_num))
		goto errcenter;	
#if 1
//	if((key = ftok("/app", 'g')) < 0)
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

	struct message msgbuf;
	msgbuf.type = 1L;
	msgbuf.msg_type = 9L;
//	msg_buf.type = 1L;
//	msg_buf.msgtype = 10L;
//	strcpy(msg_buf.text, phone.phone_num);
//	strcat(msg_buf.text, phone.center_num);
	strcpy(msgbuf.text.phone, phone.phone_num);
	strcat(msgbuf.text.phone, phone.center_num);

	msgsnd(msgid, &msgbuf, sizeof(msgbuf)-sizeof(long), 0);
#if 1 
	FILE *fp;
	fp = fopen(DATAFILE, "w");
	if (NULL != fp)
	{
		fputs(phone.phone_num, fp);
	}
	fclose(fp);
#if 1 	
	FILE *cfp;
	cfp = fopen(CENTER_DATAFILE, "w");
	if (NULL != cfp)
	{
		fputs(phone.center_num, cfp);
	}
	fclose(cfp);
#endif
#endif
#if 1  //20150116 lyf modify
	db_update();
#endif
#if 0
	FILE *fp;
	char s[20]="00";
	fp = fopen(DATAFILE, "r");
	if (NULL != fp)
	{
		fgets(s, 12, fp);
	}
	fclose(fp);
#endif
#endif
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=./setting1.cgi\">");
	fprintf(cgiOut, "<H2>提交<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>报警电话：%s</H2>", phone.phone_num);
	fprintf(cgiOut, "<H2>中心号码：%s</H2>", phone.center_num);
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;

errphone:
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=./setting1.cgi\">");
	fprintf(cgiOut, "<H2>提交<font color=\"#FF0000\" size=\"+3\">失败！</font>请仔细检查电话号码！本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>报警电话：<font color=\"#FF0000\">%s</font></H2>", phone.phone_num);
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;

errcenter:
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=../setting1.html\">");
	fprintf(cgiOut, "<H2>提交<font color=\"#FF0000\" size=\"+3\">失败！</font>请仔细检查中心号码！本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<H2>中心号码：<font color=\"#FF0000\">%s</font></H2>", phone.center_num);
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;
}
