#include "data_global.h"
/************************************************************
1.	open(dev_buzzer,  )
	2.	pthread_cond_wait (cond_buzzer,  );
	3.	获取dev_buzzer_mask(控制标志)
	4.	通过ioctl（）控制buzzer
*************************************************************/
void *pthread_buzzer(void *arg)
{
	printf("pthread_buzzer\n");
}
