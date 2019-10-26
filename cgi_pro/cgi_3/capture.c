#include <stdio.h> 
#include <string.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msgq.h"
#include "cgic.h" 

int cgiMain() 
{ 
	key_t key;
	char buf[2];
	int msgid;
	char sto_no[2];
	struct msg msg_buf;
	memset(&msg_buf,0,sizeof(msg_buf));
//	int camera_fd;
//	int len = buf[0] - 48;
	cgiFormString("mode",buf, sizeof(buf));
	cgiFormString("store", sto_no, 2);

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
	
	msg_buf.text[0] = buf[0] - '0';

	msg_buf.type = MSGTYPE;
	msg_buf.mtype = MSG_CAMERA;
	msgsnd(msgid, &msg_buf, MSGSIZE, 0);


#if 0
	if ((camera_fd  =  open( "/tmp/webcom", O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		perror("Can't Open Camer Serial Port");
		exit(-1);
	}
#endif

	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 
//	cgiFormString("name", ctr_info, 241); 
#if 0
	while( (len--) == 0)
	{
		write(camera_fd,"danger",10);
	}
#endif

	fprintf(cgiOut, "<H2>capture sucess</H2>");

	//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../video%d.html\">",sto_no[0]-48);
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 


	return 0; 
} 
