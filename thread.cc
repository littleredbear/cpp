#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

pthread_t ntid;

void printids(const char *s) 
{
		pid_t	pid;
		pthread_t tid;

		pid = getpid();
		tid = pthread_self();
		printf("%s pid %ld tid %ld (0x%ld)\n", s, (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
		printids("new thread:");
		
		return ((void *)0);
}

void *waiteformsg(void *arg) 
{
	char buff[1024];
	int n;
	int connfd = *((int *)arg);
	while((n=read(connfd, buff, 1024))>0) {

			printf("get msg:%d\n", n);
			fputs(buff, stdout);
			write(connfd, buff, strlen(buff));
	}

	return ((void *)0);
}

void *connectfd(void *arg)
{
	char buff[1024];
	int connfd = *((int*)arg);
	while(fgets(buff, 1024, stdin)) {
			fputs(buff, stdout);
			printf("send to %d\n", connfd);
		write(connfd, buff, strlen(buff));
	}
	return ((void *)0);
}

int main(int argc, char **argv) {


	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[1024];
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(28888);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 100);


	for (;;) {
			connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
			if (connfd < 0) {
					printf("connect error\n");
					exit(0);
			}
			else {
					printf("connect fd:%d\n", connfd);
			}
			pthread_create(&ntid, NULL, connectfd, &connfd);
			pthread_create(&ntid, NULL, waiteformsg, &connfd);
	}


	sleep(1);
	exit(0);
}
