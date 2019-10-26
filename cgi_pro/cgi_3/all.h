#ifndef __ALL_H__
#define __ALL_H__

#include <linux/fs.h>
#include <linux/ioctl.h>

#define LED_MAGIC 'k'
#define DRV_LED_ON  _IO(LED_MAGIC, 1)
#define DRV_LED_OFF _IO(LED_MAGIC, 2)

#define LED_PATH      "/dev/led" 
#define ZIGBEE_PATH   "/dev/ttyUSB0"




#endif
