#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define KGRN  "\x1B[32m"

void error(const char* msg){
       perror(msg);
       exit(1);	
}


int main(int argc,char *argv[])
{
	if(argc<2){
         fprintf(stderr,"Numéro de port non donné");
	 exit(1);
        }
int sock, sock1, portnum, clientlen;
float n;
char buffer[255];
portnum=atoi(argv[1]);
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
listen(sock, 5);
clientlen=sizeof(client_addr);
sock1=accept(sock,(struct sockaddr *) &client_addr, &clientlen);
if(sock1<0){
error("communication non acceptée");
}
FILE *fp;
int ch=0;
fp=fopen("test_received.txt" , "w");
int words;
read(sock1 , &words , sizeof(int));
while(ch != words){
 
read(sock1, buffer , 255);
fprintf(fp , "%s" , buffer);
fprintf(fp , " ");
ch++;

}

printf("received");
close(sock1);
close(sock);


return 0;
}
