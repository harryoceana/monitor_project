#include "data_global.h"
#include "chrdev_led.h"
/************************************************************
  5.	open(dev_led,  )
  6.	pthread_cond_wait (cond_led,  );
  7.	获取dev_led_mask（控制标志）
  8.	通过ioctl（）控制led
 *************************************************************/

int fsled_on(int fd);
int fsled_off(int fd);
int fsled_blink(int fd);
int fsled_seg(int fd, int cmd);
extern pthread_mutex_t mutex_led;
extern pthread_cond_t cond_led;
extern cmd_led;
led_desc_t led;

void *pthread_led(void *arg)
{
	int led_fd = 0;
	printf("pthread_led\n");

	//fd = open("/dev/demo0", O_RDWR,0777);
	led_fd = open(LED_DEV, O_RDWR, 0777);
	if(led_fd == -1){
		printf("open failed .\n");
	}
	printf("led_fd :%d.\n",led_fd);
	while(1)
	{
		pthread_mutex_lock(&mutex_led);
		pthread_cond_wait(&cond_led, &mutex_led);
		if(cmd_led == 0x41) //LED全部打开
		{
			fsled_on(led_fd);
		}

		if(cmd_led == 0x40) //LED全部关闭
		{
			fsled_off(led_fd);
		}
		if(cmd_led == 0x50) //打开流水灯
		{
			fsled_blink(led_fd);
		}
		int tmp = cmd_seg & 0xf0; //数码管显示
		if(!(tmp ^ 0x70)){
			fsled_seg(led_fd, cmd_seg);
		}
		cmd_seg = 0;
		pthread_mutex_unlock(&mutex_led);
	}
	close(led_fd);
}

int fsled_on(int fd)
{
	int i = 0;
	for(i = 2;i < 6;i ++){
		led.which = i;
		ioctl(fd,FSLEDON,&led);
		printf("-on---------------------%d.\n",__LINE__);
		//	usleep(500000);
	}
	return 0;
}
int fsled_off(int fd)
{
	int i = 0;
	for(i = 2; i < 6; i ++)
	{
		led.which = i;
		ioctl(fd,FSLEDOFF,&led);
		printf("-off---------------------%d.\n",__LINE__);
		//	usleep(500000);
	}
	return 0;
}

int fsled_blink(int fd)
{
	int i = 0;
	int j = 0;
	for(j = 0;j < 2;j ++)
	{
		for(i = 2;i < 6;i ++){
			led.which = i;
			ioctl(fd,FSLEDON,&led);
			printf("-on---------------------%d.\n",__LINE__);
			usleep(500000);

			led.which = i;
			ioctl(fd,FSLEDOFF,&led);
			printf("-off---------------------%d.\n",__LINE__);
			usleep(500000);

		}
	}
	return 0;
}

int fsled_seg(int fd, int cmd)
{
	int led_control_cmd = cmd & 0x0f;
#if GDB
	printf("led_control_cmd :%d\n", led_control_cmd);
#endif
	int count = 1;//led_num
	fsled_off(fd);
	usleep(500000);
	while(led_control_cmd)
	{
		if(count >= 5)
			break;
		if(led_control_cmd & 0x01 == 1)
		{
			count ++;
#if GDB
			printf("count = %d\n", count);
#endif
			led.which = count;
			ioctl(fd,FSLEDON,&led);
			usleep(500000);
		}
		else
		{
			count ++;
#if GDB
			printf("count = %d\n", count);
#endif
			led.which = count;
			ioctl(fd,FSLEDOFF,&led);
			usleep(500000);

		}
		led_control_cmd >>= 1;
	}
	return 0;
}
