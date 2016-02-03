
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv) 
{
	int listenfd, clifd;
	struct sockaddr_in6  servaddr;
	struct sockaddr_in sa1;
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&sa1, 0, sizeof(sa1));

	sa1.sin_family = AF_INET;
	sa1.sin_port = htons(8080);
	sa1.sin_addr.s_addr = htonl(INADDR_ANY);

	servaddr.sin6_family = AF_INET6;	
	servaddr.sin6_port = htons(8080);
//	inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr);
	servaddr.sin6_addr = in6addr_any;
	for (int i=0;i<16;++i)
		printf("s:%d,", servaddr.sin6_addr.s6_addr[i]);
	printf("\n");

	listenfd = socket(AF_INET6, SOCK_STREAM, 0);
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//	bind(listenfd, (struct sockaddr *)&sa1, sizeof(sa1));
	listen(listenfd, 10);
	struct sockaddr *cliaddr;
	cliaddr = (struct sockaddr *)calloc(1, sizeof(servaddr));
	socklen_t len = sizeof(servaddr);
	for (;;) {
		clifd = accept(listenfd, cliaddr, &len);
		if (clifd == -1)
			continue;
		char ad[INET6_ADDRSTRLEN];
		printf("len:%d,ad:%s\n", len,inet_ntop(AF_INET6, &((struct sockaddr_in6 *)cliaddr)->sin6_addr, ad, sizeof(ad)));		
		for (int i=0;i<16;i++) 
			printf("adr:%d,",(((struct sockaddr_in6 *)cliaddr)->sin6_addr.s6_addr)[i]);
		printf("\n");
		printf("p:%d,f:%d\n",ntohs(((struct sockaddr_in6 *)cliaddr)->sin6_port),((struct sockaddr_in6 *)cliaddr)->sin6_family);
		write(clifd, "ttttt\n", 7);
		close(clifd);
	}
}
