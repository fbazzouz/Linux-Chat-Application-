#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>




void client (char *ip, int port,char *clientnom)

{
int sock,portnum,n;
struct sockaddr_in servaddr;
struct hostent *server;
char buffer[255];

portnum=port;
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock<0){
error("erreur lors de la création de la socket");
}
server=gethostbyname(ip);
if(server==NULL){
fprintf(stderr,"y a pas de host de ce nom");
}
servaddr.sin_family=AF_INET;
bcopy((char *) server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length );
servaddr.sin_port=htons(portnum);
if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr))<0){
error("connexion echouée");
}
while(1){
int j;
char msg[255]="";
bzero(buffer, 255);
printf("%sMoi :%s",KGRN,KNRM);
scanf("%s",msg);
int i=strncmp("bye", msg , 3);

//cryptage du msg le cle = " port" 

 for(j = 0; j < (strlen(msg)) + 1; j++)
  {
   buffer[j] = msg[j] ^ portnum;
  }
// envoyer le message crypté
n=write(sock,buffer , strlen(buffer));
if(i==0){
shutdown(sock,2);
break;
}
if(n<0){
error("erreur d'ecriture");
}
bzero(buffer,255);
msg[255]="";
n=read(sock,buffer,255);
if(n<0){
error("erreur de lecture");
}
//Decryptage du msg
 for(j = 0; j < (strlen(buffer)) + 1; j++)
  {
   msg[j] = buffer[j] ^ portnum;
  }
printf("%sMessage du %s %s: %s \n",KBLU,clientnom,KNRM,msg);
}
close(sock);

}
