/*main.c
 * chatroom程序sever端
 * */
#include "server.h"

const char welcome[] = "------welcome to lean's chatroom------\n";
int main ()
{
	int listenfd, connfd, maxfd, maxi, i, n;
	int nready, client[FD_SETSIZE] = {0};
    fd_set rset, allset;
    char buf[MAXLINE] = {0};

    struct clientinfo clients_info[FD_SETSIZE];
	struct sockaddr_in cliaddr;
	socklen_t clilen;

	/*初始化，完成创建绑定监听等工作 */
	listenfd = servinit ();
    maxfd = listenfd;
    maxi = -1;
    n = -1;

    for (i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
        clients_info[i].isempty = 0;   /*零为空*/
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

	while (1)
	{
        rset = allset;
        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) == 0)
            continue;

        if (FD_ISSET(listenfd, &rset))
        {
            clilen = sizeof (cliaddr);
            if ((connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0 || connfd == listenfd)
            {
                if (errno == EINTR)
                    continue;
                else
                    error_delt ("accept error", __LINE__);
            }

            for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            if (i == FD_SETSIZE)
                error_delt("too many clients", __LINE__);
            
            write(connfd, welcome, strlen(welcome));

            printf ("client:%s:%d login\n", inet_ntoa (cliaddr.sin_addr), ntohs(cliaddr.sin_port));

            FD_SET(connfd, &allset);  /*add new client to set */
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;

            if (clients_info[connfd].isempty == 0 )
            {
                clients_info[connfd].port = ntohs(cliaddr.sin_port);
                strncpy(clients_info[connfd].ip, inet_ntoa(cliaddr.sin_addr), strlen(inet_ntoa(cliaddr.sin_addr)));
                clients_info[connfd].isempty = -1;
            } else
                continue;

            if (--nready <= 0)
                continue;
        }

        for (i = 0; i <= maxi; i++)
        {
             if ((connfd = client[i]) < 0)
                continue;
            if (FD_ISSET(connfd, &rset))
            {
                memset(buf, 0, sizeof(buf));           /*清空buffer，防止读取长字符串后再读取短字符串时，短字符串覆盖不了多出来的字符串*/
                if ((n = read(connfd, buf, MAXLINE)) == 0)  /*读取客户端发送的消息*/
                {
                    close(connfd);
                    FD_CLR(connfd, &allset);
                    client[i] = -1;
                    
                    printf("client:%s:%d logout!\n", clients_info[connfd].ip, clients_info[connfd].port);
                    clients_info[connfd].isempty = 0;
                } else  /*转发*/
                    childproc(connfd, maxfd, clients_info, buf, n);

                if (--nready <= 0)     /*no more readable messgae*/
                     break;
            }
        }
	}
    return 0;
}
