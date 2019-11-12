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



void listerserver(char *ip, int port)
{
     int sockfd, newsockfd, portno, clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     char list[2000];
     char nom_fichier[250];
    
     printf("WAITING ...\n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = port;
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
 n = read(newsockfd,list,2000);
     if (n < 0) 
          error("ERROR reading from socket");
printf("Liste des fichiers ");
    printf("%s\n",list);

printf("Entrez le nom du fichier que vous voulez recevoir \n");
scanf("%s",nom_fichier);
write(newsockfd,nom_fichier,strlen(nom_fichier));
FILE *fp=NULL;
fp=fopen("tst.txt","w");
char readBuffer[512];
while(read(newsockfd, readBuffer , 512)>0){
fwrite(readBuffer, sizeof(char), strlen(readBuffer), fp);
//fprintf(file,"\n");
if(readBuffer=="")
break;
}
printf("Fichier recu \n");
close(newsockfd);
close(sockfd);
return ;
}




