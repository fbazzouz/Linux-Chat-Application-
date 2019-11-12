#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "menu.h"


int main(int argc, char *argv[])
{

/* Check parameters from command line */

    if(argc != 3)//check number of argc in command line
    {
        fprintf(stderr,"Syntaxe: udpclient [IP_server] [server_port]\n");
        return -1;
    }

    int serverPort = atoi(argv[2]);//check number of TCP server port
    if(serverPort<=0 || serverPort>65535)
    {
        fprintf(stderr, "le port donné est invalide.\n");
        return -1;
    }

    int sockfd, bytesReceived;
    struct sockaddr_in serverAddr, clientAddr;
    struct hostent *hp;
    socklen_t len;

    /* Create UDP socket */

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)//check UDP socket is created correctly
    {
        perror("Erreur d'ouverture de socket");
        return -1;
    }

    hp = gethostbyname(argv[1]);
    if (hp == 0)//check assigment of UDP server host
    {
        perror("Host inconnu ");
        close(sockfd);
        return -1;
    }
    
    /* Bind any port number */

    bzero((char *) &clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)//check UDP socket is bind correctly
    {
        perror("Erreur de bind ");
        close(sockfd);
        return -1;
    }


    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(serverPort);

    /* Insert credentials */
    int tailleBuffer=250;
    int i = 0;
    int connectionCorrect = 0;
    char username[30], password[30], buffer[tailleBuffer];
    for(i = 0; i < 3 ;i++)//check number of tries to login
    {
        printf("Entrez votre login:");
        scanf("%s",username);
        printf("Entrez votre password:");
        scanf("%s",password);
        strcpy(buffer, username);
        strcat(buffer, ";");
        strcat(buffer, password);
        //send credential request for confirming to UDP server
        sendto(sockfd, buffer, tailleBuffer, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        //read credential answer for confirming from UDP server
        bytesReceived = recvfrom(sockfd, buffer, tailleBuffer, 0, (struct sockaddr *)&clientAddr, &len);
        if(buffer[0] == 'T' )
        {
            connectionCorrect = 1;
printf("\n");
            printf("%s      Vous êtes connecté au serveur.%s\n\n\n",KRED,KNRM);
            menu(argv[1],serverPort,username,"Serveur");
close(sockfd);
            return 0;
        }
        else
            printf("Login ou mot de passe incorrect.\n\n");
    }
    printf("Vous avez dépassé 3 essai.\n");

    close(sockfd);
    return 0;
}
