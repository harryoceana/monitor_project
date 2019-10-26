#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "./cgic.h"
#include "./all.h"
#include "./sqlite3.h"

#define N   1024

typedef struct {
	float temp_max;
	float temp_min;
	float hum_max;
	float hum_min;
	float light_max;
	float light_min;
}new_st;

int errflag = 0;
new_st sdata; 

void db_update()
{
	sqlite3 *db;
	char *errmsg;
	char sql[N] = {0};
#if 1	
	if (sqlite3_open("/smartfarm.db", &db) != SQLITE_OK) {
		errflag = 1;
//		printf("open_err = %s\n", sqlite3_errmsg(db));
		return ;
	}
#endif

#if 0
	while (sqlite3_open("/smartfarm.db", &db) != SQLITE_OK) {
		printf("open_err = %s\n", sqlite3_errmsg(db));
		errflag = 1;
	}
#endif	
	errflag = 0;
#if 0 
	printf("update env set temperature_MAX=%f, temperature_MIN=%f, humidity_MAX=%f, humidity_MIN=%f,illumination_MAX=%f, illumination_MIN=%f where farm_no=1;", \
			sdata.temp_max, sdata.temp_min,sdata.hum_max, sdata.temp_min,sdata.light_max,sdata.light_min);
#endif

#if 1
	sprintf(sql,"update env set temperature_MAX=%f, temperature_MIN=%f, humidity_MAX=%f, humidity_MIN=%f,illumination_MAX=%f, illumination_MIN=%f where farm_no=1;", \
			sdata.temp_max, sdata.temp_min,sdata.hum_max, sdata.hum_min,sdata.light_max,sdata.light_min);
#endif

#if 0 
	sprintf(sql, "update env set temperature_MAX=%f, temperature_MIN=%f, humidity_MAX=%f, humidity_MIN=%f,illumination_MAX=%f, illumination_MIN=%f where farm_no=1;",
			sdata.temp_max, sdata.temp_min,sdata.hum_max, sdata.temp_min, sdata.light_max,sdata.light_min);
#endif
#if 0 
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
	{
		errflag = 1;
	}
#endif

#if 1 
	int err;
	printf("db = %p\n", db);
	if ((err = sqlite3_exec(db, sql, NULL, NULL, &errmsg)) != SQLITE_OK)
	{
		errflag = 1;
//		printf("**********errmsg = %s\n", errmsg);
	}
	printf("err = %d\n", err);
#endif

#if 0 
	if (sqlite3_exec(db, "update env set temperature_MAX=50.000000, temperature_MIN=5.000000, humidity_MAX=50.000000, humidity_MIN=5.000000,illumination_MAX=4000.000000, illumination_MIN=15.000000 where farm_no=1;", NULL, NULL, NULL) != SQLITE_OK)
	{
		errflag = 1;
	}
#endif

#if 0 
	sprintf(sql, "update env set temperature_MAX=%f where farm_no = 1", sdata.temp_max);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
	printf("************errflag = %d\n", errflag);
	sprintf(sql, "update env set temperature_MIN=%f where farm_no = 1", sdata.temp_min);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
	sprintf(sql, "update env set humidity_MAX=%f where farm_no = 1", sdata.hum_max);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
	sprintf(sql, "update env set humidity_MIN=%f where farm_no = 1", sdata.hum_min);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
	sprintf(sql, "update env set illumination_MAX=%f where farm_no = 1", sdata.light_max);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
	sprintf(sql, "update env set illumination_MIN=%f where farm_no = 1", sdata.light_min);
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
#endif

	sqlite3_close(db);
	return ;
}

int cgiMain()
{
	char buf[10] = {0};
	cgiFormString("temMAX", buf, 9);
	sdata.temp_max = atof(buf);	
	cgiFormString("temMIN", buf, 9);
	sdata.temp_min = atof(buf);
	cgiFormString("humMAX", buf, 9);
	sdata.hum_max = atof(buf);
	cgiFormString("humMIN", buf, 9);
	sdata.hum_min = atof(buf);
	cgiFormString("illMAX", buf, 9);
	sdata.light_max = atof(buf);
	cgiFormString("illMIN", buf, 9);
	sdata.light_min = atof(buf);

//错误判断
#if 0 	
	printf("Content- type: text/html\n\n");
	printf("************errflag = %d\n", errflag);
#endif
//处理数据
	db_update();
	
	
	if(errflag) //for debug
		goto err;

	errflag = 0;
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=./setting1.cgi\">");
	fprintf(cgiOut, "<H2>高级设置<font color=\"#FF0000\" size=\"+3\">成功！</font>本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;
err:
	errflag = 0;
	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>操作提示</TITLE></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#EEF2FB\">"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"3; url=./setting1.cgi\">");
	fprintf(cgiOut, "<H2>ok设置<font color=\"#FF0000\" size=\"+3\">失败！</font>未知错误，请重试！本页面3秒后自动关闭。</H2>");
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);	
	return 0;
}




