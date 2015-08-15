#ifndef __shm_h
#define __shm_h
#include <sys/ipc.h>
#include <sys/shm.h>


#define PROJID 123456
int chat_shm_open(const char *pathname, size_t size, int flag);

#endif

