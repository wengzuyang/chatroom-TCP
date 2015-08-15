/**
 * chatroom client
 */
#include "client.h"

int main(int argc, char **argv)
{
    int conn_fd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        printf ("Usage: %s <IP>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT); 
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

   if ( connect(conn_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
       error_delt("connect failed", __LINE__);
   }
   str_cli(stdin, conn_fd);
} 
