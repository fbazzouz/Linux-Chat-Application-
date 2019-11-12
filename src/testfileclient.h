#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


void fileclient(char *ip,int port){
int sock, conn , n;
struct sockaddr_in servaddr,localAddr;
struct hostent *server;
char buffer[4000];
char c=' ';
char FNT='0';
char s;	



//verifier l existence du host 
server=gethostbyname(ip);
int numport=port;
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
//envoi des fichiers au serveur

printf("Entrez le nom du fichier que vous voulez envoyer au serveur \n");
scanf("%s",buffer);
n=write(sock,buffer , strlen(buffer));
if(n<0){
error("erreur d'ecriture");
}
//recevoir la reponse du serveur
n=read(sock,&c,1);
if(n<0){
error("erreur de lecture");
}
if(c==FNT){//si le fichier nest pas trouvé
  do
        {
            printf("Voulez-vous envoyer le fichier? [o] Oui  [n] Non\n");
            do
            {
                c = getchar();
            }while (isspace(c));
            if (c == 'o')
            {
                n = write(sock, "1", 1);//envoyer confirmation de creation du fichier
                if (n < 0)
                {
                    perror("Erreur d'ecriture");
                }
            }else if (c == 'n')//annuler la creation du fichier 
            {
                  return;
            }else{
                  printf("entrez o ou n !");
            }
        }while((c != 'o')&&(c != 'n'));

}else{// si le fichier est trouvé
        do
        {
            printf("le fichier existe , voulez-vous ?\n[e] l'écraser [a] ajouter [c] Cancel: \n");
            do
            {
                c = getchar();
            }while (isspace(c));
            if (c == 'e')
            {
               n = write(sock,"1",1);//envoyer confirmation pour l ecraser
               if (n < 0)
               {
                  perror("Erreur d ecriture");
               }
            }else if (c == 'a')
            {
                  n = write(sock,"1",1);//envoyer confirmation pour l ajout
                  if (n < 0)
                  {
                     perror("Erreur d ecriture");
                  }
            }else if (c == 'c')
            {
                  return ;
            }else
            {
                     printf("Entrez e,a ou c");
            }
        }while((c != 'e')&&(c != 'a')&&(c != 'c'));
    }

    // Lire depuis le fichier et envoyer au serveur
    FILE * fp;
    fp = fopen(buffer, "r");
    if(fp == NULL)
    {

        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
    }

char sh;
sh=getc(fp);

while(sh!=EOF){
//x=fscanf(fp , "%[^\n]" , buffer);
write(sock ,&sh ,1);
sh=getc(fp);
}

printf("fichier envoyé \n");
fclose(fp);
shutdown(sock,2);
return;
/*

    do
    {
        s = fgets(buffer,4000, fp);//lire depuis le fichier
        if(s != NULL && buffer[0] != EOF){
            n = write(sock, buffer, strlen(buffer));}
          //send the information to TCP server
    }while(s != NULL);

    printf("Envoi du fichier au serveur a été éffectué avec succés.");
    /* Read the TCP server response about the transfer of the file and show*/
/*
    bzero(buffer,4000);//clean buffer
    n = read( sock,buffer,4000);
    if (n< 0)
    {
        perror("Erreur de lecture depuis TCP socket");
    }
    printf("%s\n",buffer);

    close(sock);

*/
    

}
