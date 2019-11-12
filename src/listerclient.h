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


void listerclient(char *ip,int port)
{
    int sockfd, portno, n ,j=0 ,i=0;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct dirent* fichierlu = NULL;
    DIR *rep = NULL;
    FILE *file=NULL;
    char list[2000];
    char path[100];
    char path_completed[500];
    char nom_fichier[250];
  
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Erreur d'ouverture de socket");
    server = gethostbyname(ip);
    if (server == NULL) {
        perror("host inconnu");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("Erreur de connection");
printf("Entrez le chemin du dossier des fichiers à lister :\n");
scanf("%s",path);

rep=opendir(path);
 if (rep == NULL){
         printf("Erreur d'ouverture du dossier");
}
    for(i=0;i<2;i++){
readdir(rep);//pour ne pas afficher . et ..
}
    
    while((fichierlu= readdir(rep)) != NULL){
          strcat(list,fichierlu->d_name);
          strcat(list," == ");
    }
printf("Liste de fichiers envoyée au serveur \n");
n = send(sockfd,list,strlen(list),0);
if (n < 0) 
         error("Erreur d envoi au socket");


bzero(nom_fichier,250);
n=read(sockfd,nom_fichier,250);
if(n<0){
perror("erreur de lecture du socket");
exit(1);
}
strcat(path_completed,path);
strcat(path_completed,"/");
strcat(path_completed,nom_fichier);
file=fopen(path_completed,"r");
char sh;
sh=getc(file);

while(sh!=EOF){
//x=fscanf(fp , "%[^\n]" , buffer);
write(sockfd , &sh , 1);
sh=getc(file);

}

 printf("fichier envoyé \n");
fclose(file);
shutdown(sockfd,2);  

    return ;
}
    
