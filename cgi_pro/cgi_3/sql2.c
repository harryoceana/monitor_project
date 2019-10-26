#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "sem.h"
#include "cgic.h"
#include "data_global.h"

#define N 1024
#define STO_NO 1

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_clien_addr rt_status;
};

int GOODSKINDS[7] = {2, 3, 4, 5, 6, 7, 9};
char goods_Name[8][20] = {"", "apple", "banana", "pear", "orange", "mango", "pitaya", "orange"};

int cgiMain()
{
	key_t key;
	int shmid,semid;
	struct shm_addr *shm_buf;
	
	if((key = ftok("/dev",'i')) <0)
	{
		perror("ftok");
		exit(1);
	}
	printf("key = %x\n",key);

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

	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1 )
	{
		perror("shmat");
		exit(1);
	}

	sem_p(semid,0);

	cgiHeaderContentType("text/html");
	fprintf (cgiOut, "<head><meta http-equiv=\"refresh\" content=\"1\"></head>");
	fprintf (cgiOut, "<HTML>\n");
	fprintf (cgiOut, "<BODY bgcolor=\"#3C85B2\">\n");

	fprintf (cgiOut, "<h4><font face=\"Broadway\"><font color=\"#EABD93\">仓库二物品信息 </font></font></H4>\n ");	
	//fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[1]);
//	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_1].goods_count);
//	fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[2]);
//	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_2].goods_count);
	fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[4]);
	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", shm_buf->rt_status.storage_no[STO_NO].goods_info[GOODSKINDS_3].goods_count);
//	fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[4]);
	//fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_4].goods_count);
//fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[5]);
//	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_5].goods_count);
//	fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[6]);
//	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_6].goods_count);
////	fprintf (cgiOut, "<h5>物品名称:%s</H5>\n ", goods_Name[7]);
//	fprintf (cgiOut, "<h5>物品数量:\t%d</H5>\n ", info_CGI.storage_no[STORAGE_NO].goods_info[GOODSKINDS_7].goods_count);
	//	fprintf(cgiOut, "<h3>:</H3>\n ");	
	fprintf(cgiOut, "</BODY></HTML>\n");	
	sem_v (semid, 0);
	shmdt(shm_buf);

	return 0;
}


