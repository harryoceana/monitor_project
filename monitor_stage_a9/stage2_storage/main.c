#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "data_global.h"

void release_pthread_resource(int signo);

extern pthread_mutex_t mutex_client_request,
	                   mutex_led,
	                   mutex_refresh,
	                   mutex_sqlite,
	                   mutex_transfer,
	                   mutex_sms,
	                   mutex_buzzer;

extern pthread_cond_t cond_client_request,
	                  cond_led,
	                  cond_refresh,
	                  cond_sqlite,
	                  cond_transfer,
	                  cond_sms,
	                  cond_buzzer;

pthread_t id_client_request,
	             id_led,
	             id_refresh,
	             id_sqlite,
	             id_transfer,
	             id_sms,
	             id_buzzer;

extern int msgid;
extern int semid;
extern int shmid;

int main(int argc, const char *argv[])
{
	//初始化锁
	pthread_mutex_init(&mutex_client_request, NULL);
	pthread_mutex_init(&mutex_led, NULL);
	pthread_mutex_init(&mutex_refresh, NULL);
	pthread_mutex_init(&mutex_sqlite, NULL);
	pthread_mutex_init(&mutex_transfer, NULL);
	pthread_mutex_init(&mutex_sms, NULL);
	pthread_mutex_init(&mutex_buzzer, NULL);

	//等待接收信号，信号处理函数
	signal(SIGINT, release_pthread_resource);

	//初始化条件锁
	pthread_cond_init(&cond_client_request,NULL);
	pthread_cond_init(&cond_led,NULL);
	pthread_cond_init(&cond_refresh,NULL);
	pthread_cond_init(&cond_sqlite,NULL);
	pthread_cond_init(&cond_transfer,NULL);
	pthread_cond_init(&cond_sms,NULL);
	pthread_cond_init(&cond_buzzer,NULL);

	//创建线程
	pthread_create(&id_client_request, NULL,pthread_client_request ,NULL);
	pthread_create(&id_led, NULL,pthread_led ,NULL);
	pthread_create(&id_refresh, NULL,pthread_refresh ,NULL);
	pthread_create(&id_sqlite, NULL,pthread_sqlite ,NULL);
	pthread_create(&id_transfer, NULL,pthread_transfer ,NULL);
	pthread_create(&id_sms, NULL,pthread_sms ,NULL);
	pthread_create(&id_buzzer, NULL,pthread_buzzer ,NULL);

	//等待线程退出
	pthread_join(id_client_request, NULL);
	pthread_join(id_led, NULL);
	pthread_join(id_refresh, NULL);
	pthread_join(id_sqlite, NULL);
	pthread_join(id_transfer, NULL);
	pthread_join(id_sms, NULL);
	pthread_join(id_buzzer, NULL);

	return 0;
}

void release_pthread_resource(int signo)
{
	pthread_mutex_destroy(&mutex_client_request);
	pthread_mutex_destroy(&mutex_led);
	pthread_mutex_destroy(&mutex_refresh);
	pthread_mutex_destroy(&mutex_sqlite);
	pthread_mutex_destroy(&mutex_transfer);
	pthread_mutex_destroy(&mutex_sms);
	pthread_mutex_destroy(&mutex_buzzer);

	pthread_cond_destroy(&cond_client_request);
	pthread_cond_destroy(&cond_led);
	pthread_cond_destroy(&cond_refresh);
	pthread_cond_destroy(&cond_sqlite);
	pthread_cond_destroy(&cond_transfer);
	pthread_cond_destroy(&cond_sms);
	pthread_cond_destroy(&cond_buzzer);

	pthread_detach(id_client_request);
	pthread_detach(id_led);
	pthread_detach(id_refresh);
	pthread_detach(id_sqlite);
	pthread_detach(id_transfer);
	pthread_detach(id_sms);
	pthread_detach(id_buzzer);

	printf("all pthread detached\n");

	msgctl(msgid, IPC_RMID, NULL);
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 1, IPC_RMID, NULL);
exit(0);
}
