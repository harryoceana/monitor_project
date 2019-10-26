#include "data_global.h"
#include "chrdev_beep.h"
/************************************************************
  1.	open(dev_buzzer,  )
  2.	pthread_cond_wait (cond_buzzer,  );
  3.	获取dev_buzzer_mask(控制标志)
  4.	通过ioctl（）控制buzzer
 *************************************************************/
extern cmd_buzzer;
extern pthread_mutex_t mutex_buzzer;
extern pthread_cond_t cond_buzzer;

void *pthread_buzzer(void *arg)
{
	int i = 0;
	int beep_fd = 0;
	beep_desc_t beeper;
#if GDB
	printf("pthread_buzzer\n");
#endif
	beep_fd = open(BEEPER_DEV,O_RDWR | O_NONBLOCK);
	if ( beep_fd == -1 ) {
		perror("open beeper failed.\n");
	}
	printf("beep_fd  :%d.\n",beep_fd);
	while(1)
	{
		pthread_mutex_lock(&mutex_buzzer);

		pthread_cond_wait(&cond_buzzer, &mutex_buzzer);
		if(cmd_buzzer == 0x51)
		{
			ioctl(beep_fd,BEEP_ON,&beeper);
			sleep(10);
			ioctl(beep_fd,BEEP_OFF,&beeper);
            usleep(50000);
#if 0
			for(i = 0;i < sizeof(MUSIC)/sizeof(MUSIC[0]); i += 2)
			{
				beeper.tcnt = MUSIC[i];
				beeper.tcmp = MUSIC[i]/2;
				ioctl(beep_fd,BEEP_FREQ,&beeper);
				usleep(MUSIC[i+1] * 20000);
			}
#endif
		}
		if(cmd_buzzer == 0x50)
		{
			ioctl(beep_fd,BEEP_OFF,&beeper);
			sleep(10);
			ioctl(beep_fd,BEEP_ON,&beeper);
            usleep(50000);
		}
		pthread_mutex_unlock(&mutex_buzzer);
	}
	close(beep_fd);
}
