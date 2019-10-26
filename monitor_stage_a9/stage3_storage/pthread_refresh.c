#include "data_global.h"
#include "sem.h"

#define N 1024

extern int semid;
extern int shmid;

extern key_t sem_key;
extern key_t shm_key;


extern pthread_cond_t  cond_transfer;
	  
extern struct env_info_client_addr  sm_all_env_info;

int file_env_info_struct(struct env_info_client_addr  *rt_status,int home_id);//模拟数据刷新的函数


extern struct shm_addr *shm_buf;

void *pthread_refresh(void *arg)
{
 if((sem_key = ftok("/tmp", 'i')) < 0)
 {
perror("sem_key ftok");
exit(-1);
 }

 semid = semget(sem_key, 1, IPC_CREAT|IPC_EXCL|0666);
 if(semid == -1)
 {
if(semid == EEXIST)
semid = semget(sem_key, 1, 0777);
else{
perror("semget");
exit(-1);
}
 }else{
	 init_sem(semid, 0, 1); 
 }

if((shm_key = ftok("/tmp", 'i')) < 0)
{
	perror("shm_key ftok");
	exit(-1);
}

shmid = shmget(shm_key, N, IPC_CREAT|IPC_EXCL|0666);
if(shmid == -1)
{
	if(shmid ==EEXIST)
		shmid = shmget(shm_key, N, 0777);
	else{
	perror("shmget");
	exit(-1);
	}
}

if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void *)-1)
{
	perror("shmat");
	exit(-1);
}
#if GDB
printf("pthread_refresh ......>>>>>>>\n");
#endif
	bzero (shm_buf, sizeof (struct shm_addr));
	while(1){
		sem_p(semid,0); //P操作
		shm_buf->shm_status = 1;
#if 1
        int home_id = 0;
		home_id = shm_buf->shm_status;
		shm_buf->sm_all_env_info.monitor_no[home_id] = sm_all_env_info.monitor_no[home_id];  //真实数据上传
#else
		file_env_info_struct(&shm_buf->sm_all_env_info,shm_buf->shm_status);//模拟数据上传
#endif
		sleep(1);
		sem_v(semid,0); //v操作
		pthread_cond_signal(&cond_transfer);
	}
}

int file_env_info_struct(struct env_info_client_addr *rt_status,int home_id)
{
	int env_info_size = 0;

	env_info_size = sizeof(struct env_info_client_addr);

    rt_status->monitor_no[home_id].zigbee_info.temperature = 10.0;
	rt_status->monitor_no[home_id].zigbee_info.tempMIN = 2.0;
	rt_status->monitor_no[home_id].zigbee_info.tempMAX = 20.0;
	rt_status->monitor_no[home_id].zigbee_info.humidity  = 20.0;
	rt_status->monitor_no[home_id].zigbee_info.humidityMIN  = 10.0;
	rt_status->monitor_no[home_id].zigbee_info.humidityMAX  = 30.0;
	rt_status->monitor_no[home_id].zigbee_info.reserved[0]  = 0.01;
	rt_status->monitor_no[home_id].zigbee_info.reserved[1]  = -0.01;


	rt_status->monitor_no[home_id].a9_info.adc  = 9.0;
	rt_status->monitor_no[home_id].a9_info.gyrox  = -14.0;
	rt_status->monitor_no[home_id].a9_info.gyroy  = 20.0;
	rt_status->monitor_no[home_id].a9_info.gyroz  = 40.0;
	rt_status->monitor_no[home_id].a9_info.aacx  = 642.0;
	rt_status->monitor_no[home_id].a9_info.aacy  = -34.0;
	rt_status->monitor_no[home_id].a9_info.aacz  = 5002.0;
	rt_status->monitor_no[home_id].a9_info.reserved[0]  = 0.01;
	rt_status->monitor_no[home_id].a9_info.reserved[1]  = -0.01;

	//添加stm32部分的数据、arduino数据，

	return 0;
}
