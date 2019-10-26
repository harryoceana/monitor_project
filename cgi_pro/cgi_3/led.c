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

int cgiMain()
{
	key_t key;
	int msgid;	
	char sto_no[2];
	char ledbuf[16];
	int cmd, storage_no;
	msg_s msg_buf;

	cgiFormString("store", sto_no, sizeof(sto_no));
	cgiFormStringNoNewlines("led", ledbuf, sizeof(ledbuf));

	cmd = atoi(ledbuf);
	storage_no = atoi(sto_no);

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

	if (cmd == FORM_LED_ON)
	{
		msg_buf.text[0] = MSG_LED_ON;
	}else if(cmd == FORM_LED_OFF)
	{
		msg_buf.text[0] = MSG_LED_OFF;
	}

	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_LED;
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<TITLE>led</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#fffffffff\">\n");
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"0;url=../main%d.html\">",storage_no);
	fprintf(cgiOut, "</BODY></HTML>\n");

	return 0;
}
