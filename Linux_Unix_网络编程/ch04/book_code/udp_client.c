#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 1234

#define MAXDATASIZE 100	

int main(int argc,char *argv[])
{
	int sockfd,num;	/*files descriptiors*/
	char buf[MAXDATASIZE];	/*buf will store received text*/
	struct hostent *he;	/*structure that will get information about remote host*/
	struct sockaddr_in server,peer;
	
	if(argc != 3)
	{
		printf("Usage: %s <IP Address><Message>\n",argv[0]);
		exit(1);
	}
	
	if((he = gethostbyname(argv[1])) == NULL)
	{
		printf("gethostbyname error\n");
		exit(1);
	}
	
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		printf("socket() error\n");
		exit(1);
	}
	
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr *)he->h_addr);
	
	sendto(sockfd,argv[2],strlen(argv[2]),0,(struct sockaddr *)&server,sizeof(server));
	
	socklen_t len;
	len = sizeof(server);
	
	while(1)
	{
		if((num = recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr *)&peer,&len)) == -1)
		{
			printf("recvfrom() error\n");
			exit(1);
		}
		
		if(len != sizeof(server) || memcmp((const void *)&server,(const void *)&peer,len) !=0)
		{
			printf("Receive message from other server \n");
			continue;
		}
		
		buf[num-1]='\0';
		printf("server message : %s\n",buf);
		
	}
	
	close(sockfd);
}
