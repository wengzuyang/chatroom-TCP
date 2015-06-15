#include <sys/shm.h>
#define PROJID 98765 

int shm_open(const char *filename, size_t size, int flag)
{
    key_t key;
    int shmid;

    if (( key = ftok(filename, PROJID)) == (key_t)-1)
        return -1;

    if ((shmid = shmget(key, size, flag)) == -1)
        return -1;

    return shmid;
}

