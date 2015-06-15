#ifndef __sem_h
#define __sem_h

#define PROJ_ID 12345

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int sem_open(const char *filename, int nsems, int flag);
int sem_getvalue(int semid);
void sem_wait(int semid);
void sem_post(int semid);

#endif
