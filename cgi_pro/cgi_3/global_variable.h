#ifndef __GLOBAL_VARIABLE__H__
#define __GLOBAL_VARIABLE__H__

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "struct.h"
#include "sqlite3.h"

#define REFRESH 0
#define TEST1 0 
#define DEBUG 1 
#define DEV_GPRS 			"/dev/ttyUSB1"
#define DEV_ZIGBEE 			"/dev/ttyUSB0"
#define DEV_LED				"/dev/led"
#define DEV_BUZZER			"/dev/buzzer"
#define DEV_CAMERA			"/tmp/webcom"
#define SQLITE_OPEN			"./smartfarm.db"

#define TEMPERATURE_MAX   1
#define TEMPERATURE_MIN   2
#define HUMIDITY_MAX      3
#define HUMIDITY_MIN      4
#define ILLUMINATION_MAX  5
#define ILLUMINATION_MIN  6

 
/*message queue type */
#define REQUEST 1L

/*message queue msg_type*/
#define BUZZER 1L
#define CAMERA 2L
#define FAN 3L
#define LED 4L
#define RELAY 5L
#define SET_PARAMETER 6L
#define SMS 7L
//#define SQLITE 7L
#define UART 8L
#define MODIFY_PHONE_NUM 9L
#define BEEP 10L
//#define LEN 6
#define LEN 12

/*驱动命令字定义*/
#define LED_MAGIC 'k'
#define LED2_ON _IO(LED_MAGIC,1)
#define LED2_OFF _IO(LED_MAGIC,2)
#define LED5_ON _IO(LED_MAGIC,3)
#define LED5_OFF _IO(LED_MAGIC,4)

#define LED_ON 				(0x11)
#define LED_OFF 			0
#define LED1 1
#define LED2 2
#define LED3 4
#define LED4 8
//#define LED_ON 			_IO('L', 10)
//#define LED_OFF 			_IO('L', 11)
//#define BEEP_OFF			_IO('B', 11)	
//#define BEEP_ON			_IO('B', 10)
#define BUZZER_ON				1
#define BUZZER_OFF			0
//#define FAN_ON 0x21
#define FAN_ON 0x31
#define FAN_OFF 0x30
#define BEEP_ON 0x61
#define BEEP_OFF 0x60
#define RELAY_ON (0x41)
#define RELAY_OFF 0x40
#define ON 1
#define OFF 0

#define LED_STATUS (0x1<<0)
#define FAN_STATUS (0x1<<1)
#define BEEP_STATUS (0x1<<2)
#define RELAY_STATUS (0x1<<3)
#define SMS_STATUS (0x1<<4)


/*save the locate of zigbee_fan */
extern unsigned char locate_fan[3];

extern pthread_cond_t cond_abnormal;
extern pthread_cond_t cond_analysis;
extern pthread_cond_t cond_buzzer;
extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_client_request;
extern pthread_cond_t cond_led;
extern pthread_cond_t cond_receive_info;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata; 	//
extern pthread_cond_t cond_sms;
extern pthread_cond_t cond_sqlite;
extern pthread_cond_t cond_uart_cmd;
 
extern pthread_mutex_t mutex_analysis;
extern pthread_mutex_t mutex_abnormal;
extern pthread_mutex_t mutex_buzzer;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_client_request;
extern pthread_mutex_t mutex_global; 			//
extern pthread_mutex_t mutex_led;
extern pthread_mutex_t mutex_linklist; 		//
extern pthread_mutex_t mutex_receive_info;
extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata; 	//
extern pthread_mutex_t mutex_slinklist; 		//
extern pthread_mutex_t mutex_sms;
extern pthread_mutex_t mutex_sqlite;
extern pthread_mutex_t mutex_uart_cmd;

extern int dev_buzzer_fd;
extern int dev_led_fd;
extern int dev_camera_fd;
extern int dev_sms_fd;
extern int dev_uart_fd;

/*对设备进行操作掩码*/
extern unsigned char dev_buzzer_mask;
extern unsigned char dev_camera_mask;
extern unsigned char dev_fan_mask;
extern unsigned char dev_led_mask;
//extern unsigned char dev_relay_mask;
extern unsigned char dev_sms_mask;
extern unsigned char dev_uart_mask;
extern unsigned char dev_status;

extern char high_temp[256];
extern char low_temp[256];
extern char high_humi[256];
extern char low_humi[256];
extern char high_ill[256];
extern char low_ill[256];

extern int msgid;
extern int shmid;
extern int semid;
extern char receive_phone[256];
extern char send_phone[256];
extern char center_phone[256];

extern struct message msg;
extern struct control_parameter limit_parameter;
extern struct env_info_clien_addr all_info_RT;
extern slinklist slink_head, slink_tail;
extern uart_cache_list uart_send_cache_h, uart_send_cache_t;
extern struct env_info_clien_addr env_info_clien_addr_s;
extern linklist linkHead, linkTail;

extern void *pthread_sqlite (void *);			//数据库线程
extern void *pthread_analysis (void *);			//数据解析线程
extern void *pthread_abnormal (void *);	//数据异常处理线程		
extern void *pthread_uart_cmd (void *);			//命令发送线程
extern void *pthread_client_request (void *);	//接收CGI，QT请求
extern void *pthread_receive_info (void *);		//数据接收线程
extern void *pthread_buzzer (void *);			//蜂鸣器控制线程
extern void *pthread_led (void *);				//LED控制线程
extern void *pthread_camera (void *);			//摄像头线程
extern void *pthread_sms (void *);				//发送短信线程
extern void *pthread_refresh (void *);	//共享内存数据刷新线程
extern int main_init(void);
extern int Create_table (void);
extern void sendMsgQueue (long, unsigned char);
extern slinklist sqlite_CreateEmptyLinklist(void);
extern slinklist sqlite_GetLinknode(slinklist);
extern int sqlite_EmptyLinklist(slinklist);
extern int sqlite_InsertLinknode(int, farm_info, int farm_num);
extern void setLimit (int , float , float , float , float , float , float );

/*uart_receive,analysis*/
extern linklist CreateEmptyLinklist();
extern int EmptyLinklist (linklist h);
extern linklist GetLinknode (linklist h);
extern int InsertLinknode (link_datatype x);

/*uart_cache*/
extern uart_cache_list CreateEmptyCacheList(void);
extern uart_cache_list GetCacheNode(uart_cache_list, uart_cache_list);
extern int EmptyCacheList(uart_cache_list);
//extern int InsertCacheNode(uart_cache_list, unsigned char);
extern int InsertCacheNode(uart_cache_list, uart_cache_data);

/*pthread_uart_cmd*/
extern void serial_init(int);

/*sms*/
extern int set_com_config(int ,int ,int ,char ,int );
extern int UART_open(const char *);
/*
 *typedef struct _snode_
 *{
 *	struct sqlite_operation data;
 *	struct env_info_clien_addr data_link;
 *	int farm_num;
 *	struct _snode_ *next;
 *}slinknode, *slinklist;
 */


#endif
