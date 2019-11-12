#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int fd = 0;
     char chemin[100]={0};
     char all_files[1024];
     char file_name[1024];
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     printf("WAITING FOR YOUR FRIEND\n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     printf("\n##############~RECIEVE FILES~##############\n");
     sleep(5);
     n = read(newsockfd,all_files,1024);
     if (n < 0) 
          error("ERROR reading from socket");
     printf("User1: %s\n",all_files);
     while(1){     
     printf("User2: Enter the file that you want to recieve: ");
     gets(file_name);
     
     printf("User2: Enter the path of the recieved file: ");
     gets(chemin);

     n = send(newsockfd,file_name,strlen(file_name),0);
     if (n < 0) 
          error("ERROR sending to socket");
     sleep(5);

     n = read(newsockfd,buffer,1024);
     if (n < 0) error("ERROR reading from socket");

     fd = open(chemin,O_WRONLY|O_CREAT|O_TRUNC,0644);
     if (fd < 0) error("ERROR opening file");
     n = write(fd,buffer,strlen(buffer));
     if (n < 0) error("ERROR writing in the file");
     printf("The file is successfully recieved\n");

     bzero(buffer,1024);
     bzero(file_name,1024);
     }
     return 0; 
}
