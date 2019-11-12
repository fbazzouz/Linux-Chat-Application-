#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define KBLU  "\x1B[34m"
#define KGRN  "\x1B[32m"


void error (char *msg){
   perror(msg);
   exit(1);
}

int main (int argc, char *argv[])

{
int sock,portnum,n;
struct sockaddr_in servaddr;
struct hostent *server;
char buffer[255];
if(argc<3)
{
     fprintf(stderr,"syntaxe : %s hostname numero_de_port",argv[0]); 
     exit(1);
}
portnum=atoi(argv[2]);
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock<0){
error("erreur lors de la création de la socket");
}
server=gethostbyname(argv[1]);
if(server==NULL){
fprintf(stderr,"y a pas de host de ce nom");
}
servaddr.sin_family=AF_INET;
bcopy((char *) server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length );
servaddr.sin_port=htons(portnum);
if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr))<0){
error("connexion echouée");
}
bzero (buffer , 255);
FILE *f;
int words=0;
char c;
f=fopen("test.txt" , "r");
while((c = getc(f)) != EOF){

fscanf(f , "%s" , buffer);
if(isspace(c) || c=='\t'){ words++;}

}
write(sock , &words , sizeof(int));
rewind(f);
char ch;
while(ch != EOF){
fscanf(f , "%s" , buffer);
write(sock , buffer , 255);
ch=fgetc(f);


}
printf("fichier envoyé");
close(sock);
return 0;
}
