#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE	1000

void process_cli(int confd,struct sockaddr_in client);

int main()
{
	int listenfd,connectfd;		/*socket descriptors*/
	pid_t pid;
	struct sockaddr_in server;	/*server's address information*/
	struct sockaddr_in client;	/*client's address information*/
	socklen_t addrlen;
	int len;
	
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
	
	len = sizeof(client);
	
	while(1){
		if((connectfd = accept(listenfd,(struct sockaddr *)&client,&addrlen)) == -1)
		{
			/*handle exception*/
			perror("accept() error");
			exit(1);
		}
		
		if((pid = fork()) > 0)
		{
			close(connectfd);
			continue;
		}
		else if(pid == 0)
		{
			close(listenfd);
			process_cli(connectfd,client);
			exit(0);
		}
		else{
			printf("fork() error \n");
		}
	}
	
	// printf("You got a connection from client's ip is %s,port is %d \n",inet_ntoa(client.sin_addr),htons(client.sin_port));
	// /*printf client's IP and port*/
	// send(connectfd,"Welcome \n",8,0);	/*send to the client welcome message*/
	// close(connectfd);	/*close connectfd*/
	
	close(listenfd);	/*close listenfd*/
}

void process_cli(int confd,struct sockaddr_in client)
{
	int num;
	char recvbuf[MAXDATASIZE],sendbuf[MAXDATASIZE],cli_name[MAXDATASIZE];
	
	printf("You got a connection from client's ip is %s\n",inet_ntoa(client.sin_addr));
	num = recv(confd,cli_name,MAXDATASIZE,0);
	if(num == 0)
	{
		close(confd);
		printf("Client disconnected . \n");
		return ;
	}
	
	cli_name[num - 1] = '\0';
	printf("Client's name is %s .\n",cli_name);
	
	while(num = recv(confd,recvbuf,MAXDATASIZE,0))
	{
		recvbuf[num] = '\0';
		printf("Received client (%s) message : %s",cli_name,recvbuf);
		int i = 0; 
		for(i = 0;i < num -1;i++)
		{
			if((recvbuf[i]='a' &&recvbuf[i]<'z') || (recvbuf[i]='A' &&recvbuf[i]<'Z'))
			{
				recvbuf[i]= recvbuf[i]+3;
				if((recvbuf[i] > 'Z' && recvbuf[i]<'Z'+3) || (recvbuf[i] > 'z' ))
					recvbuf[i]=recvbuf[i] - 26;
			}
			sendbuf[i] = recvbuf[i];
		}
		
		sendbuf[num -1] = '\0';
		send(confd,sendbuf,strlen(sendbuf),0);
	}
	
	close(confd);
}