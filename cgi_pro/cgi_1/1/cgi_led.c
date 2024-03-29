/*************************************************************************
	#	 FileName	: test.c
	#	 Author		: fengjunhui 
	#	 Email		: 18883765905@163.com 
	#	 Created	: 2017年07月03日 星期一 15时48分02秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
 #include <sys/ioctl.h>
#include <linux/limits.h>
#include <errno.h>
#include "chardev.h"


#define LED_DEVICE "/dev/chrdev0"
#define MAX_BUFFER_SIZE PIPE_BUF
int cgiMain()
{
#if 0
	int i = 0,j = 3;
	int nread;
	int led_control,led_state;
	int led_fd,fifo_fd;
	led_desc_t led;
	char *data;

	led_fd = open(LED_DEVICE,O_RDWR);
	if(led_fd < 0){
		printf("open failed !\n");
	}
	printf("open device success! led_fd: %d\n",led_fd);

    printf("Content-type: text/html;charset=utf-8\n\n");
    printf("<html>\n");
    printf("<head><title>cgi control led web</title></head>\n");
    printf("<body>\n");
    printf("<p>led is setted successful! you can watch the led's change</p>\n");
    //printf("<p><a herf=http://192.168.1.100/led.html>go back</a></p>\n");
	printf("<a href=\"/led.html\">go back led control page </a>");
	printf("</body>\n");

    data = getenv("QUERY_STRING");   //getenv()读取环境变量的当前值的函数 

    if(sscanf(data,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
    {   //利用sscnaf（）函数的特点将环境变量分别提取出led_control和led_state这两个值
        printf("<p>please input right"); 
        printf("</p>");
    } 
    printf("<p>led_control = %d,led_state =  %d</p>", led_control, led_state);
    if(led_control < 2 || led_control > 5) { 
        printf("<p>Please input 2<=led_control<=5!"); 
        printf("</p>");
    } 
    if(led_state>1) {
        printf("<p>Please input 0<=led_state<=1!"); 
        printf("</p>"); 
    }

	led.led_num   = led_control;
	led.led_state = led_state;
	
	if(led.led_state == 0){
		ioctl(led_fd,FS_LED_OFF,&led);
	}else if(led.led_state == 1){
		ioctl(led_fd,FS_LED_ON,&led);
	}else if(led.led_state == 2){
		while(j --){
			for(i = 2; i <= 5; i ++ ){
				led.led_num = i;
				led.led_state = 0;
				ioctl(led_fd,FS_LED_OFF,&led);
				usleep(500000);
				led.led_state = 1;
				ioctl(led_fd,FS_LED_ON,&led);
				usleep(500000);
			}
		}
	}

	close(led_fd);
    printf("</html>\n");

	return 0;
#endif
	int nread;
	int led_control,led_state;
	int led_fd,fifo_fd;
	led_desc_t led;
	char *data;

	led_fd = open(LED_DEVICE,O_RDWR);
	if(led_fd < 0){
		printf("open failed !\n");
	}
	printf("open device success! led_fd: %d\n",led_fd);

    printf("Content-type: text/html;charset=utf-8\n\n");
    printf("<html>\n");
    printf("<head><title>cgi control led web</title></head>\n");
    printf("<body>\n");
    printf("<p>led is setted successful! you can watch the led's change</p>\n");
    printf("<p><a herf=led.html><button>get back</button></a></p>\n");
    printf("</body>\n");

    data = getenv("QUERY_STRING");   //getenv()璇诲彇鐜鍙橀噺鐨勫綋鍓嶅€肩殑鍑芥暟 

    if(sscanf(data,"led_control=%d&led_state=%d",&led_control,&led_state)!=2)
    {   //鍒╃敤sscnaf锛堬級鍑芥暟鐨勭壒鐐瑰皢鐜鍙橀噺鍒嗗埆鎻愬彇鍑簂ed_control鍜宭ed_state杩欎袱涓€?        printf("<p>please input right"); 
        printf("</p>");
    } 
    printf("<p>led_control = %d,led_state =  %d</p>", led_control, led_state);
    if(led_control < 2 || led_control > 5) { 
        printf("<p>Please input 2<=led_control<=5!"); 
        printf("</p>");
    } 
    if(led_state>1) {
        printf("<p>Please input 0<=led_state<=1!"); 
        printf("</p>"); 
    }

	led.led_num   = led_control;
	led.led_state = led_state;
	
	if(led.led_state == 0){
		ioctl(led_fd,FS_LED_OFF,&led);
	}else{
		ioctl(led_fd,FS_LED_ON,&led);
	}

	close(led_fd);
    printf("</html>\n");

	return 0;
}
