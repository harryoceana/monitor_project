#include "data_global.h"
/************************************************************
	5.	open(dev_led,  )
	6.	pthread_cond_wait (cond_led,  );
	7.	获取dev_led_mask（控制标志）
	8.	通过ioctl（）控制led
*************************************************************/
void *pthread_led(void *arg)
{
	printf("pthread_led\n");
}
