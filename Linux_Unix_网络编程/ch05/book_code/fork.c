//vfork.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid;
	int status;
	
	if((pid = fork()) == 0)
	{
		sleep(2);
		printf("child running . \n");
		printf("child sleeping . \n");
		sleep(5);
		printf("child dead . \n");
		exit(0);
	}
	else if(pid > 0)
	{
		printf("parent running . \n");
		printf("parent exit . \n");
	}
	else{
		printf("fork error . \n");
		exit(0);
	}
	
	
}
