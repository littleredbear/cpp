#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>


int main (int argc, char **argv) 
{
	int fd;
	struct sockaddr_in servaddr, cliaddr;
	struct sockaddr_in6 sa1;
	struct addrinfo cli1, *res, *cur;
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	memset(&sa1, 0, sizeof(sa1));
	memset(&cli1, 0, sizeof(cli1));
	
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(9000);
	inet_pton(AF_INET, "10.0.9.67", &cliaddr.sin_addr);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
//	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	sa1.sin6_family = AF_INET6;
	sa1.sin6_port = htons(8080);
//	inet_pton(AF_INET6, argv[1], &sa1.sin6_addr);
//	for (int i=0;i<16;++i)
//		printf("adr:%d,", sa1.sin6_addr.s6_addr[i]);
	
	cli1.ai_family = AF_UNSPEC;
	cli1.ai_socktype = SOCK_STREAM;
	
	char host[] = "10.0.9.67";
	char port[] = "8080";
	int r = getaddrinfo(host, port, &cli1, &res);
	if (r != 0) {
		printf("g e\n");	
	}

//	fd = socket(AF_INET6, SOCK_STREAM, 0);
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd == -1)
		printf("f e\n");
//	bind(fd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		
//	connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//	int n = connect(fd, (struct sockaddr *)&sa1, sizeof(sa1));
	
	int n = connect(fd, res->ai_addr, res->ai_addrlen);
	if (n != 0)
		printf("c error\n");
	
	char buff[1024];
	n = read(fd,buff, 1024);
	printf("%s", buff);	

	freeaddrinfo(res);

}
