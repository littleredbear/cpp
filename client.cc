
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <pthread.h>

pthread_t ntid;

void *sendto(void *arg) {

		char buff[1024];
		int connfd = *((int*)arg);
		while(fgets(buff, 1024, stdin)) {
			fputs(buff, stdout);
			printf("send to :%d\n", connfd);
			write(connfd, buff, strlen(buff));
		}
		return ((void*)0);
}

int main(int argc, char **argv) {
	int sockfd, n;
	char recvline[1024];
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(28888);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	pthread_create(&ntid, NULL, sendto, &sockfd);

	printf("wait for read\n");
	while((n=read(sockfd, recvline, 1024))>0) {
			printf("recv:%d\n",n);
			
			fputs(recvline, stdout);
	}
	exit(0);
}
