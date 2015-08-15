/*chatroom server function
 *想法一(服务端转发)：服务端每接受一个客户端的连接就把它的sockfd和sockaddr记录(可用数组记录)下来。
 *                    当客户端发来消息时，服务端将消息转发给除了该客户端外的所有已连接客户端;
 *                    当有客户端退出，则把它的sockfd和sockaddr从数组中移除；
 *必须包含的功能:可群聊，也可私聊(发送消息时以<IPaddr>或<port>或其<sockfd>开头) (port或者sockfd就会成匿名聊天了)
 *考虑拓展的功能:服务端也可发送消息
 * */
#include "server.h"
#include "chat_sem.h"
#include "chat_shm.h"

void error_delt(const char *err, int line)
{
    fprintf(stderr, "line %d ", line);
    perror(err);
    exit(EXIT_FAILURE);
}

void sig_child (int signo)
{								/*等待子进程结束并处理善后工作 */
	pid_t pid;
	int stat;

    if (signo == SIGINT)
	/*WNOHANG项设置在有尚未终止的子进程在运行前不要堵塞 */
	while ((pid = waitpid (-1, &stat, WNOHANG)) > 0)
	{
		/*do something */
	}
	return;
}

int servinit ()
{								/*完成服务器端套接字创建初始化、端口绑定及监听 */
	int listenfd;
	struct sockaddr_in servaddr;

	listenfd = socket (AF_INET, SOCK_STREAM, 0);

	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons (SERVER_PORT);

	if (bind (listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) == -1)
	{
		error_delt ("bind error", __LINE__); 
    } 
	if (listen (listenfd, MAX_LISTEN) == -1)
	{
		error_delt ("listen error", __LINE__);
	}

	return listenfd;
}

/* msg_explain
 * 函数功能：解析消息
 * 详细：判断群聊还好是私聊，如果为私聊则把消息头去掉
 * 并返回要私聊的客户端sockfd
 * */
int msg_explain (char *msg, int len, struct clientinfo *clients, int maxfd)
{								/*解析消息，判断是否是私聊 */
 	char ip[16] = {0}, port[8] = {0};
    char buff[MAXLINE] = {0};
	int index, i;

	if (msg[0] != '<')			/*没有<ipaddr>，说明群聊 */
		return -1;

	for (index = 1; index <= 16 && index < len; index++)
	{
 		if (msg[index] != ':')      /*ip地址结束标记*/
			ip[index - 1] = msg[index];
		else
			break;
	}
    if (msg[index] != ':')
        return -1;
	ip[index - 1] = '\0';

    i = 0;
    for (index = index + 1; index < len; index++)
    {
        if (msg[index] != '>')
            port[i++] = msg[index];
        else
            break;
    }
    port[i] = '\0';
	if (msg[index] != '>')		/*只有"<"标志而没有">" */
		return -1;

    /*获取除去地址后的消息*/
    i = 0;
	for (index = index + 1; index < len; index++)
		buff[i++] = msg[index];
	buff[i] = '\0';

    //i = sizeof(msg);  /*避免产生警告*/
    bzero(msg, sizeof(char)*MAXLINE);
	strncpy (msg, buff, strlen (buff));

	for (index = 0; index < FD_SETSIZE; index++)	/*目标IP、port和已连接客户端对比，如果存在则返回其sockfd */
    {
        if (clients[index].isempty != 0 && strncmp(ip, clients[index].ip, strlen(ip)) == 0 && atoi(port) == clients[index].port)
            return index;
        if (index == maxfd)
            break;
    }

	return -1;
}

/* childproc
 * 函数功能：转发消息
 * 详细：将消息群发或者发到特定的sockfd客户端中
 * */
void childproc (int sockfd, int maxfd, struct clientinfo *clients, char *message, int msg_len)
{								/*对已连接的客户端的数据收发处理 */
	char msg[MAXLINE] = { 0 };
	int index;
	int chatfd = 0;

    if ((chatfd = msg_explain(message, msg_len, clients, maxfd)) == -1)
    {
        memset(msg, 0, sizeof(msg));
        snprintf (msg, sizeof (msg), "<%s:%d>%s", clients[sockfd].ip, clients[sockfd].port, message);      
        for (index = 0; index < FD_SETSIZE ; index++)
        {				/*把消息发送给所有客户端，除了sockfd客户端 */
            if (clients[index].isempty == 0 || index == sockfd)
                continue;
            write (index, msg, strlen (msg));
            if (index == maxfd)
                break;
        }
    }else
    {	/*私聊 */
        memset(msg, 0, sizeof(msg));
        snprintf (msg, sizeof (msg), "<%s:%d>%s", clients[sockfd].ip, clients[sockfd].port, message);      
        write (chatfd, msg, strlen (msg));
    }
}  

