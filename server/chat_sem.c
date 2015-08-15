#include "chat_sem.h"

int chat_sem_getvalue(int semid)
{
    return semctl(semid, 0, GETVAL);
}

int chat_sem_open(const char *pathname, int nsems, int flag)
{
     key_t key;
    int semid;
    union semun semop;

    if ((key = ftok(pathname, PROJ_ID)) == (key_t)-1)
        return -1;

    /*创建信号量时nsems=1,flag=IPC_CREAT|IPC_EXCL ，打开信号量时nsems=0, flag=IPC_CREAT*/
    if ((semid = semget(key, nsems, flag | 0666)) == -1)
        return -1;

    /**/
    if (nsems != 0) {
        semop.val = 1;  /*设置信号的值，创建信号量时为1，打开信号量时为0 */
        if (semctl(semid, 0, SETVAL, semop) == -1)
            return -1;
    }

    return semid;
}

void chat_sem_post(int semid)
{
     struct sembuf sem_buf;

    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = SEM_UNDO;

    semop(semid, &sem_buf, 1);
}

void chat_sem_wait(int semid)
{
    struct sembuf sem_buf;

    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = SEM_UNDO;

     semop(semid, &sem_buf, 1);
}
