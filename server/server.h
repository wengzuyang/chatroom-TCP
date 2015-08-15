#ifndef __CHATROOM_SERVER_H
#define __CHATROOM_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>


#define SERVER_PORT 4507
#define MAXLINE     1024

#define MAX_LISTEN 10
#define MAXCONNQ 30
#define SHMPATH  "."
#define SEMPATH  ".."
#define SHM_SIZE sizeof(struct envinfo)


struct sock{
    int sockfd;
    struct in_addr ipaddr;
};

struct envinfo{
    int counter;
    struct sock sockinfo[MAXCONNQ];
};

void error_delt(const char *err, int line);
void sig_child(int signo);
int servinit();
int msg_explain(char *msg, int len, struct envinfo *env);
void childproc(int sockfd, struct in_addr cliaddr, struct envinfo *env);



#endif
