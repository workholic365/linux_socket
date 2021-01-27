#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234

#define BACKLOG 1

int main()
{
	int listenfd,connectfd;		/*socket descriptors*/
	struct sockaddr_in server;	/*server's address information*/
	struct sockaddr_in client;	/*client's address information*/
	socklen_t addrlen;
	
	/*Create TCP socket*/
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		/*handle exception*/
		perror("socket() error");
		exit(1);
	}
	
	/*set socket option*/
	int opt = SO_REUSEADDR;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	bzero(&server,sizeof(server));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(listenfd,(struct sockaddr *)&server,sizeof(server)) == -1)
	{
		/*handle exception*/
		perror("Bind() error");
		exit(1);
	}
	
	if(listen(listenfd,BACKLOG) == -1)
	{
		/*handle exception*/
		perror("Listen() error");
		exit(1);
	}
	
	int len = sizeof(client);
	if((connectfd = accept(listenfd,(struct sockaddr *)&client,&addrlen)) == -1)
	{
		/*handle exception*/
		perror("accept() error");
		exit(1);
	}
	
	printf("You got a connection from client's ip is %s,port is %d \n",inet_ntoa(client.sin_addr),htons(client.sin_port));
	/*printf client's IP and port*/
	send(connectfd,"Welcome \n",8,0);	/*send to the client welcome message*/
	close(connectfd);	/*close connectfd*/
	close(listenfd);	/*close listenfd*/
}