#ifndef _FS210_GPIO_H_ 
#define _FS210_GPIO_H_

#define FS210_GPIO_MAJOR	260
#define FS210_GPIO_MINOR	0
#define DEVICE_NAME			"ds18b20"


#define GPH1CON  0xE0200C20
#define GPH1DAT  0xE0200C24
#define GPH1PUD  0xE0200C28

#define GPC0CON  0xE0200060
#define GPC0DAT  0xE0200064
#define GPC0PUD  0xE0200068


#define	GPIO_ON		_IOW('G', 0, int)
#define	GPIO_OFF	_IOW('G', 1, int)

#endif
