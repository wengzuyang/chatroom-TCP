#include <setjmp.h>
#include "../include/chatroom.h"
#define MAXCONNQ 30
#define SHMPATH  "."
#define SEMPATH  ".."
#define SHM_SIZE 1024 

jmp_buf jmpbuffer;

struct sock{
    int sockfd;
    struct in_addr ipaddr;
};

struct envinfo{
    int counter;
    struct sock sockinfo[MAXCONNQ];
};

void sig_child(int signo);
int servinit();
int msg_explain(char *msg, int len, struct envinfo *env);
void childproc(int sockfd, struct in_addr cliaddr, struct envinfo *env);
