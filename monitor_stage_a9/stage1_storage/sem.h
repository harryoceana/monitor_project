#ifndef __SEM_H__
#define __SEM_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };
//初始化semid
int init_sem(int semid, int num, int val)
{
	union semun myun;
	myun.val = val;
	if(semctl(semid, num, SETVAL, myun) < 0)
	{
		perror("init_sem semctl");
		exit(-1);
	}

	return 0;
}

int sem_p(int semid, int num)
{
	struct sembuf mybuf;
	mybuf.sem_num = num;
	mybuf.sem_flg = SEM_UNDO;
	mybuf.sem_op = -1;

	if(semop(semid, &mybuf, 1) < 0)
	{
		perror("sem_p semop");
		exit(-1);
	}

	return 0;
}

int sem_v(int semid, int num)
{
	struct sembuf mybuf;
	mybuf.sem_num = num;
	mybuf.sem_flg = SEM_UNDO;
	mybuf.sem_op = 1;

	if(semop(semid, &mybuf, 1) < 0)
	{
		perror("sem_v semop");
		exit(-1);
	}

	return 0;
}

#endif
