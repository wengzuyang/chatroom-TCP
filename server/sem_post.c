#include <sys/sem.h>
#include "sem_h.h"

void sem_post(int semid)
{
    struct sembuf sem_buf;

    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = SEM_UNDO;

    semop(semid, &sem_buf, 1);
}
