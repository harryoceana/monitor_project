

以下测试不完善，希望大家能充分发挥自己的能力对工程代码进程改进，如果因为我的代码
局限了大家的思维和想法，我感觉某种程度上就是一种失败。

fsadc测试记录：
[root@makeru drv]# insmod fsadc.ko 
[ 1694.775000] --->fs4412_adc_init--->151.
[ 1694.775000] --->fs4412_adc_probe--->94.
[ 1694.780000] res2->start,res2->end - res2->start :0x1f.
[ 1694.785000] res1->start :168.
[ 1694.790000] major :251.
[root@makeru drv]# ./adc_test 
[ 1699.390000] ---->fs4412_adc_open--->56
[ 1699.390000] fs4412_adc_open
[ 1699.395000] --->fs4412_adc_handler--->48.
adc value :0.00V.[ 1705.420000] --->fs4412_adc_handler--->48.
adc value :0.06V.[ 1706.425000] --->fs4412_adc_handler--->48.
adc value :0.61V.[ 1707.425000] --->fs4412_adc_handler--->48.
adc value :1.02V.[ 1708.430000] --->fs4412_adc_handler--->48.
adc value :1.18V.[ 1709.435000] --->fs4412_adc_handler--->48.
adc value :1.32V.[ 1710.440000] --->fs4412_adc_handler--->48.
adc value :1.57V.[ 1711.445000] --->fs4412_adc_handler--->48.
adc value :1.80V.[ 1712.450000] --->fs4412_adc_handler--->48.
[ 1713.450000] --->fs4412_adc_handler--->48.
adc value :1.80V.
adc value :1.58V.[ 1714.455000] --->fs4412_adc_handler--->48.
adc value :1.55V.[ 1715.460000] --->fs4412_adc_handler--->48.
adc value :1.48V.[ 1716.465000] --->fs4412_adc_handler--->48.
^C[ 1717.290000] ---->fs4412_adc_release--->81
[ 1717.290000] fs4412_adc_release



fsbeeper测试记录：
[root@makeru drv]# insmod fspwmbeeper.ko 
[  836.565000] --->demo_init--->164.
[  836.565000] major :252.
[root@makeru drv]# ./music_test 
[  839.410000] --->demo_open--->65.
[  839.415000] ---->demo_ioctl--->109
[  839.415000] ---->demo_ioctl--->109
fd  :3.
[  840.060000] ---->demo_ioctl--->109
[  840.705000] ---->demo_ioctl--->109



fsled测试记录：

[root@makeru drv]# insmod fsled.ko 
[ 1383.595000] -----demo_init-----169.
[ 1383.595000] major :251.
[root@makeru drv]# ./led_test 
[ 1387.130000] -----demo_open-----38.
[ 1387.130000] -----demo_read-----53.
[ 1387.135000] copy success bytes :0.
[ 1387.140000] -----demo_write-----69.
[ 1387.140000] buf[0] :1.
[ 1387.145000] buf[1] :2.
[ 1387.145000] buf[2] :99.
[ 1387.150000] buf[3] :4.
[ 1387.150000] copy success bytes :0.
[ 1387.155000] -----demo_ioctl-----88.
[ 1387.160000] fsled 2 on.
fd :3.
rbytes :0.      rbuf[2] :3.
----------------------37.
-on---------------------47.
[ 1387.660000] -----demo_ioctl-----88.
[ 1387.665000] fsled 2 off.
-off---------------------52.
[ 1388.165000] -----demo_ioctl-----88.
[ 1388.165000] fsled 3 on.
-on---------------------47.
[ 1388.670000] -----demo_ioctl-----88.
[ 1388.670000] fsled 3 off.
-off---------------------52.
[ 1389.175000] -----demo_ioctl-----88.
[ 1389.175000] fsled 4 on.
-on---------------------47.
[ 1389.680000] -----demo_ioctl-----88.
[ 1389.680000] fsled 4 off.
-off---------------------52.
[ 1390.185000] -----demo_ioctl-----88.
[ 1390.185000] fsled 5 on.
-on---------------------47.
[ 1390.690000] -----demo_ioctl-----88.
[ 1390.690000] fsled 5 off.
-off---------------------52.
[ 1391.195000] -----demo_ioctl-----88.
[ 1391.195000] fsled 2 on.
-on---------------------47.
[ 1391.700000] -----demo_ioctl-----88.
[ 1391.700000] fsled 2 off.
-off---------------------52.
^C[ 1392.120000] -----demo_close-----45.

[root@makeru drv]# rmmod fsled
[ 1396.880000] -----demo_exit-----199.




fsmpu6050部分的测试记录： 

[root@makeru drv]# insmod  mpu6050.ko 
[  383.185000] ---->mpu6050_init--->243
[  383.190000] ---->mpu6050_probe--->184
[  383.190000] MPU6050 probe name.....(null)
[  383.195000] MPU6050 probe addr.....0x68
[  383.200000] register_chrdev success .major: 252
[  383.220000] who am i:0x68
[root@makeru drv]# ./mpu6050_test 
[  392.555000] ---->mpu6050_open--->90
[  392.555000] ---->dev_mpu6050_read--->97
[  392.560000] ---->dev_mpu6050_write--->110
[  392.565000] copy_from_user usrbuf:9
open device success! fd: 3
usrbuf[2] : 3
----------------------------------accel-------------------
accel data: x = 000a3, y = 0ffc2, z = 0079d
***********************************gyro*******************
gyro data: x = 002a9, y = 00000, z = 00002
===================================temp===================
temp data: z = 0f367
----------------------------------accel-------------------
accel data: x = 0008e, y = 0ffc8, z = 007a0
***********************************gyro*******************
gyro data: x = 0028f, y = 00000, z = 00002
===================================temp===================
temp data: z = 0f36d
----------------------------------accel-------------------
accel data: x = 00171, y = 001b2, z = 00745
***********************************gyro*******************
gyro data: x = 002d5, y = 00000, z = 00002
===================================temp===================
temp data: z = 0f379
----------------------------------accel-------------------
accel data: x = 0014b, y = 0ff8e, z = 00790
***********************************gyro*******************
gyro data: x = 002a9, y = 00000, z = 00002
===================================temp===================
temp data: z = 0f383
----------------------------------accel-------------------
accel data: x = 00097, y = 0ffd0, z = 00799
***********************************gyro*******************
gyro data: x = 002a8, y = 00000, z = 00002
===================================temp===================
temp data: z = 0f392
^C[  406.775000] ---->mpu6050_close--->168

[root@makeru drv]# ls
adc_test        fsinputkey.ko   fspwmbeeper.ko  led_test        mpu6050_test
fsadc.ko        fsled.ko        inputkey_test   mpu6050.ko      pwmbeeper_test
[root@makeru drv]# ls /dev/fsmpu60500 
/dev/mpu60500
