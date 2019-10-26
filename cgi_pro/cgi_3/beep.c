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
	char beepbuf[16];
	msg_s msg_buf;
	int storage_no;

	memset(&msg_buf,0,sizeof(msg_buf));
	
	cgiFormString("store", sto_no, sizeof(sto_no));
	cgiFormStringNoNewlines("beep", beepbuf, sizeof(beepbuf));
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

	if (strncmp(beepbuf,"beep_on", 7) == 0)
	{
		msg_buf.text[0] = storage_no << 6 | MSG_M0_BEEP_ON;
	}else if(strncmp(beepbuf,"beep_off", 8) == 0)
	{
		msg_buf.text[0] = storage_no << 6 | MSG_M0_BEEP_OFF;
	}

	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_UART_SND;
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#fffffffff\">\n");
	//fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"0;url=../main1.html\">");
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"0;url=../main%d.html\">", storage_no);
	fprintf(cgiOut, "</BODY></HTML>\n");
	return 0;
}
