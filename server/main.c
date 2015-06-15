/*main.c
 * chatroom程序sever端
 * */
#include "../include/chatroom.h"
#include "server.h"
#include "sem_h.h"
#include "shm_h.h"

int main ()
{
	int listenfd, connfd, shmid, semid, value;
	int tmpcount = 0;
    pid_t childpid;
	struct sockaddr_in cliaddr;
    struct envinfo * env;
	socklen_t clilen;

	signal (SIGCHLD, sig_child);	/*捕捉SIGCHLD信号 */
    signal (SIGINT, sig_child);
/*
    env = (struct envinfo *)malloc(sizeof(struct envinfo));  
	memset (env, 0, sizeof (struct envinfo));
*/
    if((shmid = shm_open(SHMPATH, SHM_SIZE, IPC_CREAT | 0666)) == -1)
        error_delt("shm_open error", __LINE__);

    if((env = shmat(shmid, (struct envinfo *)0, 0)) == (struct envinfo *)-1)
        error_delt("shmat falied", __LINE__);

    if((semid = sem_open(SEMPATH, 1,  IPC_CREAT | 0666)) == -1)
        error_delt("sem_open error", __LINE__);

    if ((value = sem_getvalue(semid)) == -1)
        error_delt("sem_getvalue error", __LINE__);

    printf("sem_value %d\n", value);

	/*初始化，完成创建绑定监听等工作 */
	listenfd = servinit ();
	while (1)
	{
		clilen = sizeof (cliaddr);
		if ((connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
		{
			if (errno == EINTR)
				continue;
			else
				error_delt ("accept error", __LINE__);
		}
		if (env->counter >= MAXCONNQ)
			continue;

        sem_wait(semid);

		env->sockinfo[env->counter].sockfd = connfd;
		memcpy (&(env->sockinfo[env->counter].ipaddr), &cliaddr.sin_addr, sizeof (cliaddr.sin_addr));
        env->counter++;

        sem_post(semid);
        
        for(tmpcount = 0; tmpcount < env->counter; tmpcount++)
        printf("env: counter %d sockfd %d ipaddr %s\n", env->counter, env->sockinfo[tmpcount].sockfd, inet_ntoa(env->sockinfo[tmpcount].ipaddr));

		printf ("client:%s login\n", inet_ntoa (cliaddr.sin_addr));
		
        childpid = fork ();
		switch (childpid)
		{
		case 0:  /*子进程*/ 
			close (listenfd);
	/*		sleep (60); */  /*用于调试子进程*/
            struct envinfo *shmaddr = (struct envinfo *)shmat(shmid, NULL, 0);
		
            childproc (connfd, cliaddr.sin_addr, shmaddr);
           
            shmdt(shmaddr);
			exit (EXIT_SUCCESS);
			break;
		case -1:
			error_delt ("fork", __LINE__);
			break;
		default:
            
          /*  if (env->counter - 1 != 0)  */ /*延迟关闭connfd，避免新的连接时会出现相同的套接字描述符*/
          /*      close (env->sockinfo[env->counter - 1].sockfd); */
            
            break;
		}
	}

    if (setjmp(jmpbuffer) != 0)
    {
        for (tmpcount = 0; tmpcount < env -> counter ; tmpcount++)
            close(env->sockinfo[tmpcount].sockfd);
/*        free(env); */
        semctl(semid, 0, IPC_RMID);
        shmctl(shmid, IPC_RMID, 0);
    }
	return 0;
}
