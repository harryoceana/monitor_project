#include "data_global.h"
#include "linuxuart.h"

extern pthread_mutex_t mutex_mf;
extern pthread_cond_t cond_mf;
extern cmd_fan;

void *pthread_fan(void *arg)
{
	int fan_fd = 0;

	fan_fd = open_port(FAN_DEV); //打开设备
	if(fan_fd < 0){
		printf("open failed\n");
	}
	set_com_config(fan_fd, 115200, 8, 'N', 1); //设置串口参数
#if GDB
	printf("fan_fd : %d\n", fan_fd);
#endif
	char cmdbuf[4] = {0};
	printf("1_fan on\n");
	while(1)
	{
			printf("fan on\n");
		pthread_mutex_lock(&mutex_mf);
		pthread_cond_wait(&cond_mf, &mutex_mf);
		if(cmd_fan == 0x21){
			strcpy(cmdbuf,"1\n"); //注意点
			write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));
			sleep(2);
			printf("fan on\n");
		}
		if(cmd_fan == 0x20){
			strcpy(cmdbuf,"0\n");
			write(fan_fd,cmdbuf,sizeof(cmdbuf)/sizeof(cmdbuf[0]));
			sleep(2);
		}
		char buf[32] = {0};
		//get data from zigbee
		read(fan_fd,&buf,sizeof(buf));
		printf("sizeof(buf) = %d.\n",sizeof(buf));
		printf(">>>>>>%s\n",buf);
		sleep(1);
		pthread_mutex_unlock(&mutex_mf);
	}
	close(fan_fd);
}
