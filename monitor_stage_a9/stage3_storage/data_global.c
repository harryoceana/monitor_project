#include "data_global.h"

/*********************************************************
  data_global.c :
  全局的互斥体声明
  全局的条件锁声明声明
  全局的id和key信息声明
  全局的消息队列发送函数声明
 *********************************************************/
pthread_mutex_t mutex_client_request,
        		mutex_refresh,
	        	mutex_transfer,
	        	mutex_sms,
	        	mutex_buzzer,
				mutex_sqlite,
				mutex_mf,
				mutex_fan,
	         	mutex_led;

pthread_cond_t  cond_client_request,
        		cond_refresh,
	        	cond_transfer,
	        	cond_sms,
	        	cond_buzzer,
				cond_sqlite,
				cond_mf,
				cond_fan,
	         	cond_led;
//思考：用一把锁好还是多把锁好

int msgid;
int shmid;
int semid;

key_t key; //msg_key
key_t sem_key;
key_t shm_key;

char recive_phone[12] =  {0};
char center_phone[12] =  {0};

struct env_info_client_addr sm_all_env_info;

int send_msg_queue(long type, long msgtype,unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = type;
	msgbuf.msgtype = msgtype;
    msgbuf.text[0] = text;

	if(msgsnd(msgid, &msgbuf, sizeof(struct msg) - sizeof(long), 0) < 0)
	{
		perror("send_msg_queue.msgsnd");
		exit(-1);
	}

	return 0;
}
