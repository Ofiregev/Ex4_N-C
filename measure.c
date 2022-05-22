#include<stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#define PORT 3456
#define SA struct sockaddr
#define MBSIZE 1048576


int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	char buffer[100];

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
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
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 100)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
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
				else
				printf("server accept the client...\n");
				char res[20]; // buffer for the response of the server
				strcpy(res,"GET");
				write(clientSocket, res, sizeof(res));
				struct timeval stop, start;
            	gettimeofday(&start, NULL); //start of process
				int curBytes; //var for the recieved bytes from the client
				while(curBytes != 0)
				{
					curBytes = recv(clientSocket, buffer, 100, 0);
					sum_of_bytes = sum_of_bytes + curBytes;
				}
				if(curBytes == MBSIZE)
				{
					printf("Received all 1mb , file number %d \n", file_number);
					file_number++;
				}
				else
				{
					printf("Received %d bytes\n", curBytes);
				}
				gettimeofday(&stop, NULL); //end of process

				double dt = (double) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
				t +=dt;
				printf("Time to receive the file from the client was %f second\n", dt);
				

				buffer[curBytes] = '\0';
				sum_of_bytes = 0;
				sleep(3);
				}

				//The average of the sampled times:
				    double avg_time = t * 0.25;
        			char CC_algo[100]; // buffer for the CC algorithm
					if (i != 0){ 
						strcpy(CC_algo, "reno");
						}
					else{ 
						strcpy(CC_algo, "cubic");
						}
					
					printf("\nThe average time to get 5 files in TPC CC %s is: %f\n\n", CC_algo, avg_time);
					
					i++;
				}

			// 10. Closing a connection.

				close(sockfd);

    		return 0;
		}
