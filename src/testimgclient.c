#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error(char *msg){
perror(msg);
exit(1);
}

int main (int argc, char *argv[]){
int sock, conn , n;
struct sockaddr_in servaddr,localAddr;
struct hostent *server;
char buffer[4000];
char c=' ';
char FNT='0';
char s;	


if (argc <3){
fprintf(stderr,"syntaxe : %s hostname numero_de_port",argv[0]);
exit(1);
}
//verifier l existence du host 
server=gethostbyname(argv[1]);
int numport=atoi(argv[2]);
if (server==NULL){
fprintf(stderr,"il n y a pas de host de ce nom");
}
//create TCP 
servaddr.sin_family = server->h_addrtype;
    memcpy((char *) &servaddr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    servaddr.sin_port = htons(numport);
sock=socket(AF_INET,SOCK_STREAM,0);
if(sock<0){
error("erreur de la création de la socket");
exit(1);
}
//bind
localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(numport);

    conn= bind(sock, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if(conn< 0)//check TCP socket is bind correctly
    {
        perror("Cannot bind on TCP port ");
    }

//connect
conn=connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr));
if(conn<0){
perror("connexion échouée");
exit(1);
}
//Get Picture Size
printf("Getting Picture Size\n");
FILE *picture;
picture = fopen("test.JPG", "r");
int size;
fseek(picture, 0, SEEK_END);
size = ftell(picture);
fseek(picture, 0, SEEK_SET);

//Send Picture Size
printf("Sending Picture Size\n");
write(sock, &size, sizeof(int));

//Send Picture as Byte Array
printf("Sending Picture as Byte Array\n");
char send_buffer[size];
while(!feof(picture)) {
    fread(send_buffer,sizeof(send_buffer),1, picture);
    write(sock, send_buffer, sizeof(send_buffer));
    bzero(send_buffer, sizeof(send_buffer));
}
return 0;
}
