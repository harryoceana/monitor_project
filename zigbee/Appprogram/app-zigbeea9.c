#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <termios.h>
#include <string.h>
#include "data_global.h"

int set_com_config(int fd, int baud_rate, int data_bits, char parity, int stop_bits)
{
	struct termios new_cfg, old_cfg;
	int speed;
	/*保存原有串口配置*/
	if (tcgetattr(fd, &old_cfg) != 0){
		perror("tcgetattr");
		return -1;
	}

	new_cfg =old_cfg;

	/*配置为原始模式*/
	cfmakeraw(&new_cfg);
	new_cfg.c_cflag &= ~CSIZE;

	/*设置波特率*/
	switch (baud_rate)
	{
		case 2400:{
					  speed = B2400;
					  break; 
				  }
		case 4800:{
					  speed = B4800;
					  break;
				  }
		case 9600:{
					  speed = B9600;
					  break;
				  }
		case 19200:{
					   speed = B19200;
					   break;
				   }
		case 38400:{
					   speed = B38400;
					   break;
				   }

		default:
		case 115000:{
						speed = B115200;
						break;
					}
	}

	cfsetispeed(&new_cfg, speed);
	cfsetospeed(&new_cfg, speed);

	/*设置数据位*/
	switch (data_bits)
	{
		case 7:{
				   new_cfg.c_cflag |= CS7;
				   break;
			   }   
		default:	
		case 8:{
				   new_cfg.c_cflag |= CS8;
				   break;
			   }
	}

	/*设置奇偶校验位*/
	switch (parity)
	{
		default:
		case 'n':
		case 'N':{
					 new_cfg.c_cflag &= ~PARENB;
					 new_cfg.c_iflag &= ~INPCK;
					 break;
				 }
		case 'o':
		case 'O':{
					 new_cfg.c_cflag |= (PARODD |PARENB);
					 new_cfg.c_iflag |= INPCK;
					 break;
				 }
		case 'e':
		case 'E':{
					 new_cfg.c_cflag |= PARENB;
					 new_cfg.c_cflag &= ~PARODD;
					 new_cfg.c_iflag |= INPCK;
					 break;
				 }
		case 's':
		case 'S':{
					 new_cfg.c_cflag &= ~PARENB;
					 new_cfg.c_cflag &= ~CSTOPB;
					 break;
				 }
	}

	/*设置停止位*/
	switch (stop_bits)
	{
		default:
		case 1:{
				   new_cfg.c_cflag &= ~CSTOPB;
				   break;
			   }   	
		case 2:{
				   new_cfg.c_cflag |= CSTOPB;
				   break;
			   }
	}

	/*设置等待时间和最小接收字符*/
	new_cfg.c_cc[VTIME] = 0;
	new_cfg.c_cc[VMIN] = 1;
	tcflush(fd, TCIFLUSH);
	if ((tcsetattr(fd, TCSANOW, &new_cfg)) != 0)
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;
}



int open_port(char *com_port)
{
	int fd;

	/*打开串口*/
	fd = open(com_port, O_RDWR|O_NOCTTY|O_NDELAY);
	if (fd < 0){
		perror("open serial port");
		return -1;
	}

	/*恢复串口阻塞状态*/
	if (fcntl(fd, F_SETFL, 0) < 0){
		perror("fcntl F_SETFL\n");
	}

	/*判断是否为终端设备*/
	if (isatty(fd) == 0){
		perror("This is not a terminal device");
	}

	return fd;
}


/*--------------------CH340Ƥ׃---------------------------*/
void USB_UART_Config(char* path, int baud_rate)
{
	int fd;
	fd = open_port(path);
	if(fd < 0){
		printf("open %s failed\n",path);
		return ;
	}
	if (set_com_config(fd, baud_rate, 8, 'N', 1) < 0)
	{
		perror("set_com_config");
		return ;
	}
	close(fd);
	return ;
}



int recvDats(int fd,unsigned char *pBuf,int datalen)
{   
	int curlen= 0;   
	int reallen= 0;   
	while(curlen < datalen)    
	{        
		reallen = read(fd,pBuf+curlen, datalen-curlen);          
		if(reallen >= 0){           
			curlen+=reallen;        
		}else{          
			//printf("%s %d:%s\n",__FUNCTION__,__LINE__,"uart recv err");       
			return -1;       
		}  
	}  
	return 0;
}


int main(int argc,const char *argv[])
{
	int fd = open_port("/dev/ttyUSB0");
	if(fd < 0){
		printf("open failed\n");
		return -1;
	}
	set_com_config(fd, 115200, 8, 'N', 1);

	/*	sleep(2);
		unsigned char fan_ch = 0x43;
		write(fd,&fan_ch,1); sleep(2);
		unsigned char seg_ch = 0x76;
		write(fd,&seg_ch,1); sleep(2);
		unsigned char led_ch = 0x61;
		write(fd,&seg_ch,1); sleep(2);
		*/

	while(1)
	{
#if 1

		char fan[2];
		printf("*********************************************.\n");
		printf("please input your choice:1 on fan***0 off fun.\n");
		printf("*********************************************.\n");
		scanf("%c",&fan);
		char buf[32];

		write(fd,&fan,sizeof(fan)); sleep(2);
		read(fd,&buf,sizeof(buf));
		printf("sizeof(buf) = %d.\n",sizeof(buf));
		printf("%s\n",buf);
#else	
		//get data from zigbee
		read(fd,&buf,sizeof(buf));
		printf("sizeof(buf) = %d.\n",sizeof(buf));
		printf(">>>>>>%s\n",buf);
		sleep(1);
#endif

	}
	close(fd);

	return 0;
}


