#include <sys/sem.h>
#include "sem_h.h"

int sem_getvalue(int semid)
{
    return semctl(semid, 0, GETVAL);
}
