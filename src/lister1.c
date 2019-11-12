#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <fcntl.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct dirent* file = NULL;
    DIR *rep = NULL;
    int fd = 0;
    char all_files[1024];
    char chemin[100];
    char chemin1[100];
    char buffer[1024];
    char file_name[100];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    printf("\n###############~TRANSFER FILES~###############\n");
    
    printf("User1: Enter the folder's path: ");
    gets(chemin);

    rep = opendir(chemin);
    if (rep == NULL)
         error("ERROR opening folder");
    readdir(rep);
    readdir(rep);
    while((file = readdir(rep)) != NULL){
          strcat(all_files," - ");
          strcat(all_files,file->d_name);
    }
    printf("\n%s: %s\n",chemin,all_files);
    n = send(sockfd,all_files,strlen(all_files),0);
    if (n < 0) 
         error("ERROR sending to socket");
    sleep(5);
    while(1){
    bzero(file_name,100);
    bzero(buffer,1024);
    bzero(chemin1,100);
    n = read(sockfd,file_name,100);    
    if (n < 0) 
         error("ERROR reading from socket");
    strcat(chemin1,chemin);
    strcat(chemin1,"/");
    strcat(chemin1,file_name);
    fd = open(chemin1,O_RDONLY,0644);
    if (fd < 0)
         error("ERROR opening file : could not find it"); 
    n = read(fd,buffer,1024);
    if (n < 0)
         error("ERROR reading the file");   

    n = send(sockfd,buffer,strlen(buffer),0);
    if (n < 0) 
         error("ERROR sending to socket");
    printf("User1: sending the file\n");
    sleep(5);
    }
    return 0;
}