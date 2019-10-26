#ifndef __S5PC100_PWM_HHHH
#define __S5PC100_PWM_HHHH

//need arg = 0/1/2/3
#define PWM_ON 	_IO('L', 0)
#define PWM_OFF _IO('L', 1)
#define SET_PRE _IOW('L', 2, int)
#define SET_CNT _IOW('L', 3, int)

#endif
