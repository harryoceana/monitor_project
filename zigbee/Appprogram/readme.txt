

串口控制风扇操作流程：

	1、打开ZStack-CC2530-2.3.0-1.4.0\Projects\zstack\Samples\07SampleApp-协调器和终端FAN和LED控制（串口透传）\CC2530DB代码
	2、采用smartRF04EB下载器，下载选择带风扇的终端节点模块，在工程中选择 EndDeviceEB终端，然后make编译工程，下载代码到风扇模块上
	3、然后将smartRF04EB下载器插入到电位计模块上，在工程中选择CordinatorEB协调器工程，然后make编译工程，下载代码到电位器模块上
	4、拷贝编译：
		fengjunhui@ubuntu:~/securitymonitor/Appprogram$ ls
			app-zigbeea9.c  data_global.h  Makefile  zigbeefantest
		fengjunhui@ubuntu:~/securitymonitor/Appprogram$ make
			arm-none-linux-gnueabi-gcc app-zigbeea9.c -o zigbeefantest
	5、拷贝到nfs的网络文件系统上； zigbeefantest 应用测试程序
		[root@makeru ~]# ls
			......  mjpg-streamer       sbin   zigbeefantest
	6、执行： 输入1控制风扇转动，可以自己修改应用层的逻辑使其实现更复杂的功能。
		[root@makeru ~]# ./zigbeefantest 
		please input 1
		1





