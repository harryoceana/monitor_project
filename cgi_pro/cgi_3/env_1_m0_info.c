
#include "/home/linux/intelligent_agriculture/server_head.h"
//#include "sem.h"
#include "cgic.h"
#include "data_global.h"

int sem_p(int semid,int semnu)
{
	struct sembuf semstruct;
	semstruct.sem_num=semnu;
	semstruct.sem_op=-1;
	semstruct.sem_flg=0;
	return semop(semid,&semstruct,1);
}
int sem_v(int semid,int semnu)
{
	struct sembuf semstruct;
	semstruct.sem_num=semnu;
	semstruct.sem_op=1;
	semstruct.sem_flg=0;
	return semop(semid,&semstruct,1);
}

// 共享内存结构体，用来实时显示环境信息
/*
struct shm_addr
{
	char cgi_status;	//暂时就当做扩展吧！这两个变量不用进行检测
	char qt_status;     //暂时就当做扩展吧！
	struct env_info_clien_addr rt_status;
};
*/

struct _rt_info{							//用来控制风扇，灯等
	unsigned char farm_num;
	
	unsigned char soil_tem;			//土壤温度和上下限（一共7个环境变量）
	unsigned char soil_temMIN;		
	unsigned char soil_temMAX;
	unsigned char soil_hum;			//土壤湿度和上下限
	unsigned char soil_humMIN;
	unsigned char soil_humMAX;
	unsigned char air_tem;			//空气温度和上下限
	unsigned char air_temMIN;		
	unsigned char air_temMAX;
	unsigned char air_hum;			//空气湿度和上下限
	unsigned char air_humMIN;
	unsigned char air_humMAX;	
	unsigned char ill;			//光照和上下限
	unsigned char illMIN;
	unsigned char illMAX;
	unsigned char co2;			
	unsigned char co2MIN;		//co2浓度和上下限
	unsigned char co2MAX;
	unsigned char water;		//水位和上下限
	unsigned char waterMIN;
	unsigned char waterMAX;
	
};

int cgiMain()
{
//	key_t key;
	int semid;
	int shmid;
//	struct shm_addr *shm_buf = NULL;
 	void *addr = NULL; 
	
	struct envir_mes_ envir_mes;
	
	struct _rt_info rt_buf =      
	{
		1,    	
		40,10,50,		//土壤    可以设置不同的上下限看看结果是否能正常显示！！！！
		40,10,50,
		20,10,50,		//空气
		20,10,50,
		
		30,10,50,	//光照
		35,10,50,	//co2浓度
		40,10,50,	//水位
	};
	if((semid  = semget(KEY_VAL_SEM, 1, 0666)) < 0) //获得信号量，最终是为了解决不同进程的互斥。
	{
		perror("semget");
		exit(1);
	}

	if((shmid = shmget(KEY_VAL_SHM,SHM_SIZE, IPC_EXCL|0666 )) == -1) 	//打开共享内存
	{
		perror("shmget"); //多个进程通过同一个key来访问同一个共享内存。
		exit(1);
	}
	
	if(ERR_MEM == (addr = shmat(shmid,SHM_ADDR,0))){
		error_exit("shmat");
	}

	//	sem_p(semid,1); 	//pv操作（原子操作）   pv操作中间是临界资源，可以解决进程间的互斥。
		memcpy(&envir_mes,addr,sizeof(envir_mes));
	//	sem_v(semid, 0);

	

	   rt_buf.farm_num = envir_mes.farm_num;		//农场号 读共享内存能否跳过进行读取？？
	   rt_buf.soil_tem = envir_mes.air_tempe;			//土壤温度
	   rt_buf.soil_hum = envir_mes.air_humid;		//土壤湿度	
	   rt_buf.air_tem  = envir_mes.soil_tempe;			//空气温度
	   rt_buf.air_hum  = envir_mes.soil_humid;			//空气湿度
	   rt_buf.ill      = envir_mes.co2_concen;			//光照
	   rt_buf.co2      = envir_mes.light;			//co2浓度
	   rt_buf.water    = envir_mes.water_lev;			


	cgiHeaderContentType("text/html"); 
	fprintf(cgiOut, "<HTML><HEAD>"); 
	fprintf(cgiOut, "<TITLE>当前实时数据</TITLE>");  
	fprintf(cgiOut, "<style type=\"text/css\"> body,td,th {font-size: 12px;margin-left: 0px;margin-top: 0px;}</style></HEAD>"); 
	fprintf(cgiOut, "<BODY BGCOLOR=\"#F8F9FA\">"); 

	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1\">");
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
		
		// 土壤温度采集	
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">土壤温度</td>");
		if(rt_buf.soil_tem > rt_buf.soil_temMAX)
		{	
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d℃</font></td>", rt_buf.soil_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.soil_temMIN, rt_buf.soil_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.soil_tem < rt_buf.soil_temMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d℃</font></td>", rt_buf.soil_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.soil_temMIN, rt_buf.soil_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d℃</td>", rt_buf.soil_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.soil_temMIN, rt_buf.soil_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		// 土壤湿度采集
		fprintf(cgiOut, 	"<tr>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">土壤湿度</td>");
		if(rt_buf.soil_hum > rt_buf.soil_humMAX)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d％</font></td>", rt_buf.soil_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.soil_humMIN, rt_buf.soil_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.soil_hum < rt_buf.soil_humMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d％</font></td>", rt_buf.soil_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.soil_humMIN, rt_buf.soil_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}	
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d％</td>", rt_buf.soil_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.soil_humMIN, rt_buf.soil_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		// 空气温度采集	
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">空气温度</td>");
		if(rt_buf.air_tem > rt_buf.air_temMAX)
		{	
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d℃</font></td>", rt_buf.air_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.air_temMIN, rt_buf.air_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.air_tem < rt_buf.air_temMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d℃</font></td>", rt_buf.air_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.air_temMIN, rt_buf.air_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d℃</td>", rt_buf.air_tem);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d℃</td>", rt_buf.air_temMIN, rt_buf.air_temMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		// 空气湿度采集
		fprintf(cgiOut, 	"<tr>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">空气湿度</td>");
		if(rt_buf.air_hum > rt_buf.air_humMAX)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d％</font></td>", rt_buf.air_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.air_humMIN, rt_buf.air_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.air_hum < rt_buf.air_humMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%d％</font></td>", rt_buf.air_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.air_humMIN, rt_buf.air_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}	
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d％</td>", rt_buf.air_hum);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%d％</td>", rt_buf.air_humMIN, rt_buf.air_humMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		
		// 光照采集
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">环境光照</td>");
		if(rt_buf.ill > rt_buf.illMAX)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.ill);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.illMIN, rt_buf.illMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.ill < rt_buf.illMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.ill);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.illMIN, rt_buf.illMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}	
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%dlux</td>", rt_buf.ill);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.illMIN, rt_buf.illMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		// co2浓度采集
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">co2浓度</td>");
		if(rt_buf.co2 > rt_buf.co2MAX)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.co2);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.co2MIN, rt_buf.co2MAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.co2 < rt_buf.co2MIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.co2);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.co2MIN, rt_buf.co2MAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}	
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%dlux</td>", rt_buf.co2);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.co2MIN, rt_buf.co2MAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		// 液位采集
		fprintf(cgiOut, 	"<tr bgcolor=\"#f2f2f2\">");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\"><img src=\"../images/on.png\" width=\"16\" height=\"16\" /></td>");
		fprintf(cgiOut, 		"<td height=\"30\" align=\"center\" valign=\"middle\">液位信息</td>");
		if(rt_buf.water > rt_buf.waterMAX)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.water);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.waterMIN, rt_buf.waterMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过高！</td>");
		}
		else if(rt_buf.water < rt_buf.waterMIN)
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\"><font color=\"#FF0000\">%dlux</font></td>", rt_buf.water);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.waterMIN, rt_buf.waterMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">过低！</td>");
		}	
		else
		{
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%dlux</td>", rt_buf.water);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">%d-%dlux</td>", rt_buf.waterMIN, rt_buf.waterMAX);
			fprintf(cgiOut, 	"<td height=\"30\" align=\"center\" valign=\"middle\">正常！</td>");
		}
		fprintf(cgiOut, 	"</tr>");
		
		fprintf(cgiOut, "</table>");
	}
	else
	{
		fprintf(cgiOut, "<H2>网络异常！</H2>");	
	}
	fprintf(cgiOut, "<script type=\"text/jscript\">");
	fprintf(cgiOut, "setTimeout(\"self.close()\", 3000)");
	fprintf(cgiOut, "</script>");
	fprintf(cgiOut, "</BODY>"); 
	fprintf(cgiOut, "</HTML>");
	fflush(stdout);

	return 0;
}






