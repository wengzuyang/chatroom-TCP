#ifndef _CHATROOM_CLIENT_H
#define _CHATROOM_CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define SERVER_PORT 4507 

int max(int num1, int num2);
void str_cli(FILE *fp, int sockfd);
void error_delt(const char *err, int line);
#endif
