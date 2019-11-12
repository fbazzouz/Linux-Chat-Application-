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


    /* Start communicating between TCP client and server */

    bzero(buffer,255);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
        perror("Error reading from TCP socket");
    }
    printf("This is the file to transffer: %s\n",buffer);

    /* Searching the file or creating file */
    
    char sample[10];
    if (fopen(buffer, "r") != NULL)
    {
        n = write(newsockfd,"1",1);
        if (n < 0)
        {
            perror("Error writing to TCP socket");
        }
    }
    else
    {
        n = write(newsockfd,"0",1);
        if (n < 0)
        {
            perror("Error writing to TCP socket");
            
        }
    }


    n = read( newsockfd, sample,1 );
    if (n < 0)
    {
        perror("Error reading from TCP socket");
    }
 FILE *file;
   char c = sample[0];
    char readBuffer[512];
    switch (c)
    {
    case '1': //write or overwrite file
        if ((file = fopen(buffer, "w")) == NULL)
        {
            fprintf(stderr, "Cannot write on file.");
        }
        break;
    case '0': //append file
        if ((file = fopen(buffer, "a+")) == NULL)
        {
            fprintf(stderr, "Cannot append to file.");
        }
    }


while(read(newsockfd, readBuffer , 255)>0){
 

fwrite(readBuffer, sizeof(char), strlen(readBuffer), file);
//fprintf(file,"\n");
if(readBuffer=="")
break;
}

printf("received");
close(newsockfd);
close(sockfd);


/*
    while((n = read(newsockfd, readBuffer, 512)) > 0)
    {
        if(n < 0)
        {
            fprintf(stderr, "Error while receiving the file.\n");
        }
        fprintf(file, readBuffer);
        if(readBuffer == "")
            break;
    }
    fclose(file);
    sent=1;


    /* Write a response to the TCP client */
/*
    if (sent)
    {
        n = write(newsockfd,"Transfer completed and successful",33);
    }
    else
    {
        printf("Error writting the file in TCP server");
    }
    if (n < 0)
    {
        perror("Error writing to TCP socket");
        
    }
*/
    return 0;



}
