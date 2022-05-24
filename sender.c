
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


#define SA struct sockaddr

#define FILE_NAME "1mb.txt"
#define PORT 3448

int fd; //file descirptor
struct stat f_stat; //struct that provides detailed information about a file


int main()
{
    // in this program we will send a file to measure.c 5 times
    //we will compare between 2 CC algorithms - "reno" and "cubic"

    int FILE_NUM = 1; //This variable will help us to supervise on which round we are on
    int t = 1; //this var presents which type of CC we runnig
    //will 1 it's coubic in 2 it's reno

    char buffer[256]; // we will use this to pass the type of the CC algorithm
    socklen_t size;

    while (t != 3)
    {
        if(t == 1)
        {
            strcpy(buffer, "cubic");
        }
        else
        {
            strcpy(buffer, "reno");
        }
        int i = 0;
        while(i<=4) //we are running 5 times on each CC alforithm
        {
            int sockfd;
            sockfd = socket(AF_INET, SOCK_STREAM , 0); //opening socket eith ipv4 and TCP protocol 
            if (sockfd == -1)
            {
                perror("Socket didn't opened successfuly");
                return -1;
            }
            else
            {
                printf("opening socket!\n");
                size = sizeof (buffer);
                //in the setsockopt function we are going to define with which CC algorithm the socket will use
                int rv_setSocket; // The return value of the function
                rv_setSocket = setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buffer, size);
                if ( rv_setSocket != 0)
                {
                    perror("setsockopt error");
                    return -1;
                }
                int rv_getSockOp; // The return value of the getsocketopt function
                rv_getSockOp = getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buffer, &size);
                if(rv_getSockOp !=0 )
                {
                    perror("getsockopt error");
                    return -1;
                }
                printf("The currnt CC algorithm is: %s\n", buffer);
                struct sockaddr_in servaddr;
                bzero(&servaddr, sizeof(servaddr));
                servaddr.sin_family = AF_INET;
                servaddr.sin_port = htons(PORT);

                int connection ; //This var presents the return value of the connect function
                connection = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
                if (connection != 0)
                {
		            printf("connection with the server failed...\n");
		            exit(0);
	            }
	            else
                {
		            printf("connected to the server..\n");
                    //now after the connection we can send the file
                    FILE* fptr; // saving the pointer to the file
                    fptr = fopen(FILE_NAME, "rb");
                    if (fptr == NULL) //The file openning didn't succeeded
                    {
                        fprintf(stderr, "can't open the file");
                        return -1 ;
                    }
                    else
                    {
                        fd = open(FILE_NAME , O_RDONLY); // O_RDONLY - read only way to open the file

                        if (fd == -1)
                        {
                            fprintf(stderr, "can't open the file");
                            return -1;
                        }

                        int file_state;
                        file_state = fstat(fd, &f_stat);
                        if(file_state < 0)
                        {
                            fprintf(stderr, "fstat error: %s", strerror(errno));
                            return -1;
                        }

                        //now after the openning of the file we want to send it to measure
                        char reply[10];// buffer for the permmision of the server
                       
                        bzero(reply, sizeof(reply));
                      
                        read(sockfd, reply, sizeof(reply));
                        

                        if(strcmp(reply, "GET") != 0)
                        {
                            printf("NO ANSWER");
                        }
                        else
                        {
                            printf("GOT A PERMISSION FROM THE SERVER\n");
                        }
                        
                        char sendB[200]; // Buffer to send the file to the server
                        int r; // The return value of the fread function
                        int sum = 0;
                        int rv_send; //The return value of the send function
                        while (!feof(fptr))
                            {
                                r = fread(sendB, 1, sizeof(sendB), fptr);
                                rv_send = send(sockfd, sendB, r , 0);
                                sum = rv_send + sum; //summing the number of bites that were sent 
                            }
                        printf("%d bytes were sent\nfile number %d \n", sum, FILE_NUM++);
                        sleep(1);
                        close(sockfd);//closing the socket
                        i++;//starting the next turn
                            
                    }
                } 
            }
            
        }
        t++;
    }
    return 0;
}

