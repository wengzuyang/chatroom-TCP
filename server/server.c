/*chatroom server function
 *想法一(服务端转发)：服务端每接受一个客户端的连接就把它的sockfd和sockaddr记录(可用数组记录)下来。
 *                    当客户端发来消息时，服务端将消息转发给除了该客户端外的所有已连接客户端;
 *                    当有客户端退出，则把它的sockfd和sockaddr从数组中移除；
 *必须包含的功能:可群聊，也可私聊(发送消息时以<IPaddr>或<port>或其<sockfd>开头) (port或者sockfd就会成匿名聊天了)
 *考虑拓展的功能:服务端也可发送消息
 * */



void sig_child(int signo)
{/*等待子进程结束并处理善后工作*/
    pid_t pid;
    int stat;

    /*WNOHANG项设置在有尚未终止的子进程在运行前不要堵塞*/
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    /*do something*/
    }
    return;
}

int servint( )
{/*完成服务器端套接字创建初始化、端口绑定及监听*/
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

/*需要全局变量struct sockinfo env*/
int msg_explain(char *msg, int len)
{/*解析消息，判断是否是私聊*/
    char ip[16];
    char *pbuff;
    int index;

    if(mag[0] != '<')  /*没有<ipaddr>，说明群聊*/
        return -1;

    for(index = 1; index <= 16 && index < len; index++) {
        if(msg[index] != '>')
        ip[index - 1] = msg[index];
        else
            break;
    }
    ip[index - 1] = '\0';

    if(msg[index] != '>')  /*只有"<"标志而没有">"*/
        return -1;

    for(index = index + 1; index < len; index++ )
        *pbuff++ = msg[index];
    *pbuff = '\0';
    memset(msg, 0, sizeof(msg));
    strncpy(msg, pbuff, strlen(pbuff));

    for(index = 0; index < MAXCONNQ; index++)  /*目标IP和已连接客户端IP对比，如果存在则返回其sockfd*/
        if ( strncmp(ip, inet_ntoa(env[index].ipaddr), strlen(ip)) == 0)
            return env[index].sockfd;

    return -1;
}

/*cliaddr用于客户端IP地址传递*/
void childproc(int sockfd, struct in_addr cliaddr)
{/*对已连接的客户端的数据收发处理*/
    char buff[BUFSIZ]={0};
    char msg[BUFSIZ] = {0};
    int len = 0;
    int index;
    int chatfd;

    while(1) {
        memset(msg, 0, sizeof(msg));
        memset(buff, 0, sizeof(buff));
        if((len = read(sockfd, buff, BUFSIZ)) >= 0) {
            buff[len]= '\0';
            snprintf(msg, sizeof(msg), "<%s>:%s\n", inet_ntoa(cliaddr), buff);
            if(len == 0)
                break;   /*客户端关闭*/
            else if ((chatfd = msg_explain(buff, len)) == -1) {
                for(index = 0; index < MAXCONNQ && env[index].sockfd != sockfd; index++ ) { /*把消息发送给所有客户端，除了sockfd客户端*/
                    write(env[index].sockfd, msg, strlen(msg)); /*需要全局变量struct sockinfo env*/
                }
            } else {/*私聊*/
                write(chatfd, msg, strlen(msg));
            }
        } else {
            printf("read error\n");
            continue;
        }
    }
    close(sockfd);
    exit(0);
}


