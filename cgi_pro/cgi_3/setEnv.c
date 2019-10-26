#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

#include "cgic.h"
#include "msgq.h"

struct setEnv
{
	int temMAX;
	int temMIN;
	int humMAX;
	int humMIN;
	int illMAX;
	int illMIN;
};

int cgiMain()
{
	key_t  key;
	char sto_no[2];
	char buf[20];
	struct setEnv new;
	int msgid;
	msg_s msg_buf;
	
	memset(&msg_buf,0,sizeof(msg_buf));
	cgiFormString("store", sto_no, sizeof(sto_no));

	cgiFormString("temMAX", buf, sizeof(buf));
	new.temMAX = atoi (buf);
	cgiFormString("temMIN", buf, sizeof(buf));
	new.temMIN = atoi (buf);
	cgiFormString("humMAX", buf, sizeof(buf));
	new.humMAX = atoi (buf);
	cgiFormString("humMIN", buf, sizeof(buf));
	new.humMIN = atoi (buf);
	cgiFormString("illMAX", buf, sizeof(buf));
	new.illMAX = atoi (buf);
	cgiFormString("illMIN", buf, sizeof(buf));
	new.illMIN = atoi (buf);



	if((key = ftok("/dev", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}

	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	memcpy (msg_buf.text+1, &new, sizeof(new));
	
	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_SETENV;
	msg_buf.text[0] = sto_no[0];
	
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);

	sto_no[0] -= '0';
	
	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 

	fprintf(cgiOut, "<H2>send sucess</H2>");

	//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../main%d.html\">", sto_no[0]);
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 


	return 0; 

}
