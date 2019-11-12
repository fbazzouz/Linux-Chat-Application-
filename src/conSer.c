#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "menu.h"


struct user

{ 

    char user[20]; 

    char psw[20]; 

}; 



int main(int argc, char *argv[])

{



    if(argc != 3)

    {//Check if program is correclty called

        fprintf(stderr, "Usage: udpserver  ip [server_port]\n");

        return -1;

    }



    FILE * fp = fopen("users.dat", "r");

    if (fp == NULL)

    {//Check if the user file given exists

        fprintf(stderr, "Unable to open users file.\n");

        return -1;

    }

 

    int serverPort = atoi(argv[2]);

    if(serverPort<=0 || serverPort>65535)

    {//Checking if the port given is valid and withing the accepted values

        fprintf(stderr, "The port number given is wrong.\n");

        return -1;

    }

    int tailleBuffer=250;

    printf("Starting Authentifiation server...\n");

    //Initializing the socket variables

    int sockfd, donneeRecu;

    struct sockaddr_in serverAddr,clientAddr;

    socklen_t len;

    char buffer[tailleBuffer];

    //Creating a socket 

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)

    {

        perror("Erreur d'ouverture du socket");

        return -1;

    }



    //Preparing serverAddr

    bzero(&serverAddr,sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    serverAddr.sin_port = htons(serverPort);

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));



    //Binding on the socket

    if(bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)

    {

        perror("erreur de bind");

        close(sockfd);

        return -1;

    }

    fprintf(stderr, "Bind dans le port %d.\n", serverPort);



   int connectionCorrect;



    while(1)

    {



        int i;
        char username[20]="";

    	char userAndPassword[70];

        connectionCorrect = 0;

        // 3 authentification possible 

      
            len = sizeof(clientAddr);

	    //Entendre les clients 

            donneeRecu = recvfrom(sockfd, buffer, tailleBuffer, 0, (struct sockaddr *)&clientAddr, &len);

            struct user test;

            while(fread(&test, sizeof(struct user), 1, fp)){

            
            strcpy(userAndPassword,test.user);

            strcat(userAndPassword,";");

            strcat(userAndPassword, test.psw);

            if(strcmp(userAndPassword,buffer)==0) {
         strcpy(username,test.user);
         connectionCorrect=1;
            break;
            }
            
            }

        //verifier si les informations sont corrects 

        if(connectionCorrect==1)

        {

            printf("\n");
            printf("%s     %s est connecté au serveur. %s\n",KRED,username,KNRM);
            
            
            sendto(sockfd, "T", tailleBuffer, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
               close(sockfd);
               menu(argv[1],serverPort,"Serveur",username);
              return 0;

        }

        else

        {

            printf("Coordonnées invalides.\n");

            sendto(sockfd, "F", tailleBuffer, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
         rewind(fp);

        }

    }



    //close the socket

    close(sockfd);

    return 0;

}
