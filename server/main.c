/*main.c
 * chatroom程序sever端
 * */
#include "server.h"

int main ()
{
	int listenfd, connfd, value, maxfd, maxi, i, n;
	int nready, client[FD_SETSIZE] = {0};
    fd_set rset, allset;
    char buf[MAXLINE] = {0};

	struct sockaddr_in cliaddr;
	socklen_t clilen;

	/*初始化，完成创建绑定监听等工作 */
	listenfd = servinit ();
    maxfd = listenfd;
    maxi = -1;

    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

	while (1)
	{
        rset = allset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &rset))
        {
            clilen = sizeof (cliaddr);
            if ((connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
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

            FD_SET(connfd, &allset);  /*add new client to set */
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;

            if (--nready <= 0)
                continue;

            printf ("client:%s login\n", inet_ntoa (cliaddr.sin_addr));
        }

        for (i = 0; i <= maxi; i++)
        {
            if ((connfd = client[i]) < 0)
                continue;
            if (FD_ISSET(connfd, &rset))
            {
                if ((n = read(connfd, buf, MAXLINE)) == 0)
                {
                    close(connfd);
                    FD_CLR(connfd, &allset);
                    client[i] = -1;
                } else
                    write(connfd, buf, n);

                if (--nready <= 0)     /*no more readable messgae*/
                    break;
            }
        }
	}
    return 0;
}
