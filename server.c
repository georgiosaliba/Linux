#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;
	int newSocket1;
	struct sockaddr_in newAddr1;

	socklen_t addr_size;
	socklen_t addr_size1;

	char buffer1[1024];
	char buffer2[1024];
	char msg1[10];
	char msg2[10];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

	int clientport;
	int clientport1;
	int n=0;

	while(n<2){
		if(n==0)
		{	
			addr_size = sizeof newAddr;
			newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
			n=n+1;
			if(newSocket < 0){
			exit(1);
			}
			printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			clientport = ntohs(newAddr.sin_port);
			if((childpid = fork()) == 0){
			close(sockfd);
			
			while(1)
			{
				*msg1=recv(newSocket, buffer1, 1024, 0);	
				printf(msg1,buffer1);
					if(strcmp(buffer1, ":exit") == 0)
					{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
					}
					else
						{
							printf("Client1: %s\n", buffer1);
							send(newSocket, buffer1, strlen(buffer1), 0);
							bzero(buffer1, sizeof(buffer1));
						}
			}

			
		}
		close(newSocket);
		}
		if(n==1)
		{
			addr_size1 = sizeof newAddr1;
			newSocket1 = accept(sockfd, (struct sockaddr*)&newAddr1, &addr_size1);
			if(newSocket1 < 0){
			exit(1);
			}
			printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr1.sin_addr), ntohs(newAddr1.sin_port));
			clientport1 = ntohs(newAddr1.sin_port);
			n=n+1;
			if((childpid = fork()) == 0){
			close(sockfd);
			
			while(1)
			{
			*msg2=recv(newSocket1, buffer2, 1024, 0);
			printf(msg2,buffer2);
					if(strcmp(buffer2, ":exit") == 0)
					{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr1.sin_addr), ntohs(newAddr1.sin_port));
					break;
					}
					else
						{
							if (buffer1==buffer2)
								{
									printf("Great");
								}
							printf("Client2: %s\n", buffer2);
							send(newSocket1, buffer2, strlen(buffer2), 0);
							bzero(buffer2, sizeof(buffer2));
						}
			
			}

		}
			
		close(newSocket1);
		}
		
	}


	return 0;
}