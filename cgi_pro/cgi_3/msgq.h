#ifndef __MSGQ_HHHHHHHHHHHHHH__
#define __MSGQ_HHHHHHHHHHHHHH__

#define MSGTYPE			1L
#define MSG_LED			1L
#define MSG_CAMERA		2L
#define MSG_UART_SND	3L
#define MSG_SETENV		4L
#define MSG_CHGNUM		5L
#define MSG_WIFI		6L

#define MSG_LED_ON		0
#define MSG_LED_OFF		1

#define MSG_SIZE		32

#define PHONE_NUM_SIZE	11

typedef struct msg{
	long type;
	long mtype;
	char text[MSG_SIZE];
}msg_s;

#define MSGSIZE			sizeof(msg_s)-sizeof(long)

//往M0发送的消息类型
#define MSG_M0_FAN_OFF			0x00
#define MSG_M0_FAN_ON1			0x01
#define MSG_M0_FAN_ON2			0x02
#define MSG_M0_FAN_ON3			0x03

#define MSG_M0_BEEP_OFF			0x10
#define MSG_M0_BEEP_ON			0x11
#define MSG_M0_BEEP_AU_OFF		0x12
#define MSG_M0_BEEP_AU_ON		0x13

#define MSG_M0_LED_OFF			0x20
#define MSG_M0_LED_ON			0x21

#define MSG_M0_SEG_ON			0x30
#define MSG_M0_SEG_OFF			0x3f

//form value
#define FORM_LED_ON			1
#define FORM_LED_OFF		0
#define FORM_BEEP_ON		1
#define FORM_BEEP_OFF		0
#define FORM_BEEP_AU_ON		2
#define FORM_BEEP_AU_OFF	3
#define FORM_SEG_ON			1
#define FORM_SEG_OFF		0
#define FORM_FAN_OFF		0
#define FORM_FAN_ON1		1
#define FORM_FAN_ON2		2
#define FORM_FAN_ON3		3

#endif
