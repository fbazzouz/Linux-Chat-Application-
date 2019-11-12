#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "color.h"

void error(const char* msg){
       perror(msg);
       exit(1);	
}


void serveur(char *ip,int port,char *nom)
{
	
int sock, sock1, portnum, clientlen;
float n;
char buffer[255];
portnum=port;
struct sockaddr_in servaddr;
struct sockaddr_in client_addr;
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=INADDR_ANY;
servaddr.sin_port=htons(portnum);
sock=socket(AF_INET, SOCK_STREAM,0);
if(sock<0){
error("création du socket echouée");
}
if(bind(sock,(struct sockaddr *) &servaddr, sizeof(servaddr))<0){
error("binding echoué");
}
printf("Waiting for communication ");
printf("\n");
listen(sock, 5);
clientlen=sizeof(client_addr);
sock1=accept(sock,(struct sockaddr *) &client_addr, &clientlen);
if(sock1<0){
error("communication non acceptée");
}

//beginig of the chat
while(1){
int j;
bzero(buffer, 255);
n=read(sock1, buffer, 255);
//Decryptage du msg
char msg[255]="";
 for(j = 0; j < (strlen(buffer)) + 1; j++)
  {
   msg[j] = buffer[j] ^ portnum;
  }
int i=strncmp("bye", msg , 3);
if(i==0){
shutdown(sock1,2);
shutdown(sock,2);
break;
}
if(n<0){
error("erreur de lecture");
}
//afficher  le message decrypté
printf("%sMessage du %s :%s %s \n",KGRN,nom,KNRM,msg);
bzero(buffer,255);
msg[255]="";
printf("%sMoi :%s",KBLU,KNRM);
scanf("%s",msg);
//cryptage du msg

 for(j = 0; j < (strlen(msg)) + 1; j++)
  {
   buffer[j] = msg[j] ^ portnum;
  }
//envoyer le message crypté
n=write(sock1 , buffer , strlen(buffer));
if(n<0){
error("echec d ecriture");
}

}
close(sock1);
close(sock);
}
