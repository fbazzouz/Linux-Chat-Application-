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
 int sockfd, newsockfd, serverPort, clientLength, n;
    char buffer[255];
    char option=' ';
    int sent=0;
    struct sockaddr_in serverAddr, clientAddr;

/* Check parameters from command line */

    if(argc != 2){//check number of argc in command line
        fprintf(stderr,"Syntaxe: %s numero_de_port \n",argv[1]);
    }

    printf("Starting TCP server...\n");

    serverPort = atoi(argv[1]);

    /* Create a TCP socket */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)//check TCP socket is created correctly
    {
        perror("Erreur d'ouverture de socket");
    }

    /* Initialize TCP socket structure */

    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    /* Bind the host address using bind() call */

    if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)//check TCP socket is bind correctly
    {
        perror("Erreur de bind ");
    }

    /* Start listening for the clients, TCP server wait for the incoming TCP client connection */

    listen(sockfd, 6);
    clientLength = sizeof(clientAddr);

    /* Accept actual connection from the TCP client */

    newsockfd = accept(sockfd, (struct sockaddr *) &clientAddr, (socklen_t *) &clientLength);
    if (newsockfd < 0)//check actual connection is created correctly
    {
        perror("Error on accepting actual TCP client");
    }
//Read Picture Size
printf("Reading Picture Size\n");
int size;
read(newsockfd, &size, sizeof(int));

//Read Picture Byte Array
printf("Reading Picture Byte Array\n");
char p_array[size];
read(newsockfd, p_array, size);

//Convert it Back into Picture
printf("Converting Byte Array to Picture\n");
FILE *image;
image = fopen("c1.png", "wb");
fwrite(p_array,1, sizeof(p_array), image);
fclose(image);
return 0;
}
