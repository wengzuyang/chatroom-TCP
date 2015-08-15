/**
 * chatroom client
 */
#include "client.h"

int max(int num1, int num2)
{
    return num1 > num2 ? num1 : num2;
}

void error_delt(const char *err, int line)
{
     fprintf(stderr, "line %d ", line);
    perror(err);
    exit(EXIT_FAILURE);
}

void str_cli(FILE *fp, int sockfd)
{
     int maxfdp1, stdineof;
    fd_set rset;
    char buf[MAXLINE];
    int n;

    stdineof = 0;
    FD_ZERO(&rset);

    while (1)
    {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;

        select(maxfdp1, &rset, NULL, NULL, NULL);

        /*---------------------read message----------------------------------*/
        if (FD_ISSET(sockfd, &rset))
        {
            if ((n = read(sockfd, buf, MAXLINE)) == 0)
            {
                if (stdineof  == 1)
                    return;
                else
                    error_delt("str_cli: server terminated", __LINE__);
            }
            write(fileno(stdout), buf, n);
        }
        /*-------------------send message----------------------------------*/
        if (FD_ISSET(fileno(fp), &rset)) /* input is readable*/
        {
            memset(buf, 0, sizeof(buf));
            if ((n = read(fileno(fp), buf, MAXLINE)) == 0)
            {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);  /*send FIN*/
                FD_CLR(fileno(fp), &rset);
                continue;
            }
            if (strncmp(buf, "exit", 4) == 0)
            {
                write(sockfd, "Bye\n", strlen("Bye\n"));
                return;
            }
            write(sockfd, buf, n);
        }
    }
}

