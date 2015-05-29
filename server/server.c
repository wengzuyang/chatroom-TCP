/*chatroom server function*/

void sig_child(int signo)
{
    pid_t pid;
    int stat;

    /*WNOHANG项设置在有尚未终止的子进程在运行前不要堵塞*/
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {

    }
    return;
}

int servint( )
{
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(servadrr, sizeof(sockaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(4507);

   if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
       perror("bind error");
       exit(EXIT_FAILURE);
   }

   if(listen(listenfd, 5) == -1) {
       perror("listen error");
       exit(EXIT_FAILURE);
   }

   return listenfd;
}

void childchat(int sockfd)
{

}
