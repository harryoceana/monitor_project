#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include "./ds18b20.h"
#include "./sqlite3.h"
#include "./cgic.h"
#include "./data_global.h"
#include "./sem.h"
#define N   64
#define ENV 1 

// 共享内存结构体
struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_clien_addr rt_status;
};

struct _rt_info{
	unsigned char sto_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char beep_status;
	unsigned char fan_status;
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	float tem;
	float temMIN;
	float temMAX;
	float hum;
	float humMIN;
	float humMAX;
	float ill;
	float illMIN;
	float illMAX;
	float bat;
	float adc;
};

char temp_max[10]  = {0}, 
	 temp_min[10]  = {0},
	 hum_max[10]   = {0},
	 hum_min[10]   = {0},
	 light_max[10] = {0}, 
	 light_min[10] = {0};
char temp[10] = {0}, hum[10] = {0}, light[10] = {0};
int errflag = 0;

float temp_old = 0;
#if 0
float get_ds18b20()
{
#if 1
	int dev_fd;
	unsigned int temp[2];
	float temperature = 0.0;

	dev_fd = open("/dev/ds18b20", O_RDWR | O_NONBLOCK);
	if (dev_fd < 0)
	{
		perror("open");
		exit(1);
	}

	temp[1] = ioctl(dev_fd, GPIO_ON, temp); 
	sleep(1);
	if(temp[1]&0x8000)
	{
		temp[1]= ~temp[1]+1;
		temp[1]&=0xffff;
		temperature = (float)(temp[1])*0.0625;
//		printf("temp[1] =  - %d \r\n",temp[1]>>4);
//		printf("tempvalue= - %f\r\n",temperature);
	}
	else
	{
		temp[1] &= 0xffff;
		temperature = (float)(temp[1])*0.0625;
//		printf("temp[1] =  %d \r\n",temp[1]>>4);
//		printf("tempvalue = %f\r\n",temperature);
	}
	close(dev_fd);
	
	while (temperature > 50)
	{
		temperature = get_ds18b20();
		temp_old = temperature;
	}
	

	return temperature;
#endif
}
#endif
void get_env()
{
	sqlite3 *db;
	char sql1[N] = {0}, sql2[N] = {0};
	char **result1, **result2;
	int row1, colunm1, row2, colunm2;
	if	(sqlite3_open("/smartfarm.db", &db) != SQLITE_OK) {
		fprintf(cgiOut, "<H2>sqlit smartfarm.db open err</H2>");	
		errflag = 1;
		return ;
	}
	sprintf(sql1, "select * from env where farm_no=1");
	if (sqlite3_get_table(db, sql1, &result1, &row1, &colunm1, NULL) != SQLITE_OK) {
		fprintf(cgiOut, "<H2>sqlite3_get_table  err</H2>");	
		errflag = 1;
		return ;
	}
	strncpy(temp_max, result1[colunm1 + 1], 9);
	strncpy(temp_min, result1[colunm1 + 2], 9);
	strncpy(hum_max, result1[colunm1 + 3], 9);
	strncpy(hum_min, result1[colunm1 + 4], 9);
	strncpy(light_max, result1[colunm1 +5], 9);
	strncpy(light_min, result1[colunm1 + 6], 9);

	sprintf(sql2, "select * from collect_env where farm_no=-1");
	if (sqlite3_get_table(db, sql2, &result2, &row2, &colunm2, NULL) != SQLITE_OK) {
		errflag = 1;
		return ;
	}
#if 0
	strncpy(temp, result2[colunm2 + 1], 9);
	strncpy(hum, result2[colunm2 + 2], 9);
	strncpy(light, result2[colunm2 + 3], 9);
#endif

	sqlite3_close(db);
}

int cgiMain()
{
	char status[10] = "正常";
	get_env();

	//线程共享内存 数据刷新
	key_t key;
	int semid;
	int shmid;
	struct shm_addr *shm_buf = NULL;

	struct _rt_info rt_buf = 
	{
		1,1,1,1,1,
		1,2,3,
		50.6,10.0,50.0,
		9.3,10.0,50.0,
		300.6,10.0,500.0,
		3.40,2.30,
	};

	if((key = ftok("/app", 'i')) < 0)
	{
		perror("ftok");
		exit(1);
	}
	
	if((semid  = semget(key, 1, 0666)) < 0)
	{
		perror("semget");
		exit(1);
	}

	if((shmid = shmget(key, N, 0666 )) == -1)
	{
		perror("shmget");
		exit(1);
	}
	
	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1)
	{
		perror("shmat");
		exit(1);
	}
	
	sem_p(semid,0);

	rt_buf.tem = shm_buf->rt_status.storage_no[0].temperature;
	rt_buf.hum = shm_buf->rt_status.storage_no[0].humidity;
	rt_buf.ill = shm_buf->rt_status.storage_no[0].illumination;
	sem_v(semid, 0);


	cgiHeaderContentType("text/html"); 
	//cgiHeaderContentType("Content- type: text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>当前实时数据</TITLE>");  
	fprintf(cgiOut, "<style type=\"text/css\"> body,td,th {font-size: 12px;margin-left: 0px;margin-top: 0px;}</style></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#F8F9FA\">"); 

	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1\">");

	//errflag = 0;
	
	if (1)  
	{
		fprintf(cgiOut, "<table width=\"100%%\" border=\"0\" cellpadding=\"5\" cellspacing=\"0\">");
		// 实时数据表头
		fprintf(cgiOut, 	"<tr bgcolor=\"#DEE4E6\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">链接</td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">名称</td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">最新数值</td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">正常范围</td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">状态</td>");
		fprintf(cgiOut, 	"</tr>");
		


		if (rt_buf.hum != 0)
//		if (1)
		{
#if ENV 

			if (rt_buf.tem < atof(temp_min)) {
				strcpy(status, "过低");
			}else if (rt_buf.tem > atof(temp_max)) {
				strcpy(status, "过高");
			}else 
				strcpy(status, "正常");
#endif
			// 空气温度采集	
			fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">空气温度</td>");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f ℃</font></td>", rt_buf.tem);
			//	fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%s ℃</font></td>", temp);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s - %s ℃</td>",temp_min, temp_max );
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s</td>", status);

#if ENV
		//	if (atof(hum) < atof(hum_min)) {
			if (rt_buf.hum < atof(hum_min)) {
				strcpy(status, "过低");
			}else if (rt_buf.hum > atof(hum_max)) {
				strcpy(status, "过高");
			}else 
				strcpy(status, "正常");
#endif

			// 空气湿度采集	
			fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">空气湿度</td>");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f %%</font></td>", rt_buf.hum);
			// fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f %%</font></td>", get_ds18b20());
			//	fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%s %%</font></td>", hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s - %s %%</td>", hum_min, hum_max);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s</td>", status);
		}
		else
		{
#if 0 
 	
			float local_temp , flag_first;
			flag_first = get_ds18b20();
			if ((flag_first < temp_old + 8) && (flag_first > temp_old - 8))
			{
				temp_old = flag_first;
			}
			else
			{
				flag_first = get_ds18b20();
			}
#endif
			if (rt_buf.tem < atof(temp_min)) {
				strcpy(status, "过低");
			}else if (rt_buf.tem > atof(temp_max)) {
				strcpy(status, "过高");
			}else 
				strcpy(status, "正常");
			// 空气温度采集	
			fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">空气温度</td>");
		//	fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f ℃</font></td>", rt_buf.tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f ℃</font></td>", rt_buf.tem);
		//	fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f ℃</font></td>", get_ds18b20());
			//	fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%s ℃</font></td>", temp);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s - %s ℃</td>",temp_min, temp_max );
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s</td>", status);

		}
		if (rt_buf.ill < atof(light_min)) {
			strcpy(status, "过低");
		}else if (rt_buf.ill > atof(light_max)) {
			strcpy(status, "过高");
		}else 
			strcpy(status, "正常");

		// 光照
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">光照强度</td>");
//		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%s Lux</font></td>", light);
		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%0.2f Lux</font></td>", rt_buf.ill);
		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s - %s Lux</td>", light_min, light_max);
		fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%s</td>", status);
	}
/*
	if(errflag);	
	{
		fprintf(cgiOut, "<H2>网络异常！</H2>");	
	}

*/
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);

	return 0;
}






