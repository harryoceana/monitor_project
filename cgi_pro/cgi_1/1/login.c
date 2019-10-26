#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include "sqlite3.h"
#include "cgic.h"

#define N	64

char name[N] = {0};
char passwd[N] = {0};
	
int cgiMain()
{
	cgiFormStringNoNewlines("username", name, N);
	cgiFormStringNoNewlines("password", passwd, N);

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<TITLE>登录提示</TITLE></HEAD>\n");

	if((strcmp(name,"chuangke") == 0)&&(strcmp(passwd,"123") == 0))
	{
		fprintf(cgiOut, "<BODY>");
		fprintf(cgiOut, "<H1>%s<Hi>","Login OK!");	
		fprintf(cgiOut, "<H1>Welcome to %s<Hi>",name);			
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=http://192.168.1.10/index.html\">"); //成功跳转的html
	}
	else{
		fprintf(cgiOut, "<H1>%s<Hi>","Login error!");	
	}
	return 0;
}
