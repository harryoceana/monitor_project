#include "data_global.h"
/**********************************************************

  long msgtype;//具体的消息类型
  消息类型的分配：
1L: 		LED控制
2L:			蜂鸣器控制
3L:			四路LED灯模拟的数码管
4L:			风扇
5L:			温湿度最值设置
6L-7L-8L-9L,用于个人的扩展
10L: 		3G通信模块-GPRS 
switch(msgbuf.msgtype){
case 1L: ...  break;
....
default ....  break;
}
 **********************************************************/

extern int msgid;
extern key_t key;

extern pthread_mutex_t mutex_client_request,
	   mutex_led,
	   mutex_refresh,
	   mutex_transfer,
	   mutex_sms,
	   mutex_sqlite,
	   mutex_mf,
	   mutex_buzzer;

extern pthread_cond_t cond_client_request,
	   cond_led,
	   cond_refresh,
	   cond_transfer,
	   cond_sms,
	   cond_sqlite,
	   cond_mf,
	   cond_buzzer;

extern char recive_phone[12] ;
extern char center_phone[12] ;
extern cmd_led;
extern cmd_buzzer;
extern cmd_seg;
extern cmd_fan;
struct msg msgbuf;

void *pthread_client_request(void *arg)
{
	if((key = ftok("/tmp", 'g')) < 0)
	{
		perror("ftok");
		exit(-1);
	}

	msgid = msgget(key, IPC_CREAT|IPC_EXCL|0666);
	if(msgid == -1)
	{
		if(errno == EEXIST)
		{
			msgget(key, 0777);
		}else{
			perror("msgget");
			exit(-1);
		}
	}
#if GDB
	printf("pthread_client_request\n");
#endif
	while(1)
	{
		bzero(&msgbuf, sizeof(msgbuf));
		printf("wait for client request......\n");
		msgrcv(msgid, &msgbuf, sizeof(msgbuf) - sizeof(long), 1L, 0);

#if GDB
		printf("Get %dL msg\n", msgbuf.msgtype);
		printf("text[0] = %#x\n", msgbuf.text[0]);
#endif
		switch(msgbuf.msgtype)
		{
		case 1L:
			pthread_mutex_lock(&mutex_led);
#if GDB
			printf("hello led\n");
#endif
			cmd_led = msgbuf.text[0];
			pthread_mutex_unlock(&mutex_led);
			pthread_cond_signal(&cond_led);
			break;
		case 2L:
			pthread_mutex_lock(&mutex_buzzer);
#if GDB
			printf("hello beep\n");
#endif
			cmd_buzzer = msgbuf.text[0];
			pthread_mutex_unlock(&mutex_buzzer);
			usleep(50000);
			pthread_cond_signal(&cond_buzzer);
			break;
		case 3L:
			pthread_mutex_lock(&mutex_led);
#if GDB
			printf("hello seg\n");
#endif
			cmd_seg = msgbuf.text[0];
			pthread_mutex_unlock(&mutex_led);
			pthread_cond_signal(&cond_led);
			break;
		case 4L:
			pthread_mutex_lock(&mutex_mf);
#if GDB
			printf("hello fan\n");
#endif
			cmd_fan = msgbuf.text[0];
			pthread_mutex_unlock(&mutex_mf);
			usleep(50000);
			pthread_cond_signal(&cond_mf);

			break;
		case 5L:
			printf("set env data\n");
			printf("temMAX: %d\n",*((int *)&msgbuf.text[1]));
			printf("temMIN: %d\n",*((int *)&msgbuf.text[5]));
			printf("humMAX: %d\n",*((int *)&msgbuf.text[9]));
			printf("humMAX: %d\n",*((int *)&msgbuf.text[13]));
			printf("illMAX: %d\n",*((int *)&msgbuf.text[17]));
			printf("illMAX: %d\n",*((int *)&msgbuf.text[21]));
			break;
		case 6L:
		case 7L:
		case 8L:
		case 9L:
			printf("备用，可以用作扩展\n");
			break;
		case 10L:
			{
				int i = 0 , j = 0 ;
				for(i = 0 ; i < 11; i++){
					recive_phone[i] = msgbuf.text[i]; 	
				}
				recive_phone[i] = '\0';
				printf("recive:%s\n",recive_phone);
				for(j = 0 ;msgbuf.text[i] != '\0' && j < 12; i++, j++)
				{
					center_phone[j] =  msgbuf.text[i];
				}
				center_phone[j] = '\0';
				printf("center:%s\n",center_phone);
#if 0
				pthread_mutex_lock (&mutex_slinklist);
				sqlite_InsertLinknode (ENV_UPDATE, all_info_RT, sto_no, 0);//0,0鍒嗗埆鏄粨搴撳彿鍜岃揣鐗╃绫诲彿
				pthread_mutex_unlock (&mutex_slinklist);
				pthread_cond_signal (&cond_sqlite);
#endif 
			}
			break;
		default:
			break;
		}//end switch
	}//end while
}


