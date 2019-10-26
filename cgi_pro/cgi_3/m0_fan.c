#include <stdio.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "cgic.h" 
#include "msgq.h"

int cgiMain() 
{ 
	key_t key;
	char buf[2];
	char sto_no[2];
	int msgid;
	int cmd, storage_no;
	msg_s msg_buf;

	memset(&msg_buf,0,sizeof(msg_buf));
	
	cgiFormString("fan", buf, sizeof(buf));
	cgiFormString("store", sto_no, sizeof(sto_no));
	cmd = atoi(buf);
	storage_no = atoi(sto_no);

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

	switch (cmd)
	{
		case FORM_FAN_OFF:
			{
				msg_buf.text[0] = storage_no << 6 | MSG_M0_FAN_OFF;
				break;
			}
		case FORM_FAN_ON1:
			{
				msg_buf.text[0] = storage_no << 6 | MSG_M0_FAN_ON1;
				break;
			}
		case FORM_FAN_ON2:
			{
				msg_buf.text[0] = storage_no << 6 | MSG_M0_FAN_ON2;
				break;
			}
		case FORM_FAN_ON3:
			{
				msg_buf.text[0] = storage_no << 6 | MSG_M0_FAN_ON3;
				break;
			}
	}
	
	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_UART_SND;
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);

	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 

	fprintf(cgiOut, "<H2>send sucess</H2>");

	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../m0_%d.html\">", storage_no);
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 

	return 0; 
} 
