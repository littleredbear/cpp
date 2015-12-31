
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) 
{
	char buf[] = "test good\n";
	int size = sizeof(buf)/sizeof(buf[0]);
	write(STDOUT_FILENO,buf,size);	

}
