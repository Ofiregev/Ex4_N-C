#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#define PORT 3448
#define SA struct sockaddr
#define MBSIZE 1048576


int main()
{
	int sockfd = -1;
	unsigned int len;
	struct sockaddr_in servaddr, cli;
	char buffer[100];

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	
	printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	
	printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 100)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
		printf("Server listening..\n");
		len = sizeof(cli);
		int i = 0;
		while(i <= 1)
		{
			double t = 0.0; //Time variable
			int file_number = 1;
			int sum_of_bytes = 0;
			while(file_number <=5)
			{
				bzero(&cli,len);
				int clientSocket = -1;
				clientSocket = accept(sockfd, (SA*)&cli, &len);
				if (clientSocket < 0)
				{
					printf("server accept failed...\n");
					exit(0);
				}
				printf("server accept the client...\n");
				char res[5]; // buffer for the response of the server
				strcpy(res,"GET");
				write(clientSocket, res, sizeof(res));//working until here

				struct timeval stop, start;
            	gettimeofday(&start, NULL); //start of process
				int curBytes = -1; //var for the recieved bytes from the client
				while(curBytes != 0)
				{
					curBytes = recv(clientSocket, buffer, 100, 0);
					sum_of_bytes = sum_of_bytes + curBytes;
				}
				if(sum_of_bytes == MBSIZE)
				{
					printf("Received all 1mb , file number %d \n", file_number);
					file_number++;
				}
				else
				{
					printf("Received only %d bytes\n", sum_of_bytes);
					file_number++;
				}
				gettimeofday(&stop, NULL); //end of process

				double dt = (double) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
				t +=dt;
				printf("Time to receive the file from the client was %f second\n", dt);
				

				buffer[curBytes] = '\0';
				sum_of_bytes = 0;
				sleep(1);
			}

				//The average of the sampled times:
			double avg_time = t / 5;
			char CC_algo[100]; // buffer for the CC algorithm
			if (i != 0){ 
				strcpy(CC_algo, "reno");
				}
			else{ 
				strcpy(CC_algo, "cubic");
				}
			
			printf("\nThe average time to get 5 files in %s CC algorithm is: %f\n\n", CC_algo, avg_time);
			
			i++;
		}

			// 10. Closing a connection.

				close(sockfd);

    		return 0;
		}
