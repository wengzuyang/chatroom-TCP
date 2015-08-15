#ifndef __CHAT_H
#define __CHAT_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PROJ_ID 654321
union semun {
    int               val;
    struct semid_ds  *buf;
    unsigned short   *array;
    struct seminfo   *__buf;
};

int chat_sem_getvalue(int semid);
int chat_sem_open(const char *pathname, int nsems, int flag);
void chat_sem_post(int semid);
void chat_sem_wait(int semid);




#endif
