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

struct From_to_send{
	char to_number[12];
	char center_number[12];
}phone_NUM;

int cgiMain()
{
	key_t key;
	int msgid;
	char sto_no[2];	
	msg_s msg_buf;

	cgiFormString("store", sto_no, 2);
	cgiFormStringNoNewlines("to",phone_NUM.to_number,sizeof(phone_NUM.to_number));
	cgiFormStringNoNewlines("center",phone_NUM.center_number,sizeof(phone_NUM.center_number));

	memset(&msg_buf,0,sizeof(msg_buf));

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

	strcpy(msg_buf.text, phone_NUM.to_number);
	strcat(msg_buf.text, phone_NUM.center_number);
	
	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_CHGNUM;
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY bgcolor=\"#fffffffff\">\n");
	fprintf(cgiOut, "<H2>Set success!</H2>");
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../main%d.html\">",sto_no[0]-'0');
	fprintf(cgiOut, "</BODY></HTML>\n");
	return 0;
}
