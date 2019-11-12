    #include <stdio.h>
#include "serveur.h"
#include "client.h"
#include "testfileclient.h"
#include "testfileserver.h"
#include "listerclient.h"
#include "listerserver.h"

void menu(char *ip,int port, char *nom,char* clientnom)

{
	int stop=0,option;
    while(stop==0){

    printf("%s=========================================================================%s\n",KCYN,KNRM);
    printf("|        APPLICATION POUR LE CHAT ET LE TRANSFERT DES FICHIERS          |\n");
    printf("|                                                                       |\n");
    printf("|         %sBY: AZZOUZ FADI && ZINEB AKHRIF && HOUYAM EL OTMANI%s           |\n",KRED,KNRM);
    printf("%s=========================================================================%s\n",KCYN,KNRM);
printf("                 Bienvenu %s                                         \n",nom);
    printf("%s=========================================================================%s\n",KCYN,KNRM);
    printf("| Options Possibles:                                                    |\n");
    printf("|        1. Ouvrir une Session du Chat                                  |\n");
    printf("|        2. Rejoindre une Session du Chat                               |\n");
    printf("|        3. Recevoir un fichier                                         |\n");
    printf("|        4. Envoyer un fichier                                          |\n");
    printf("|        5. Envoyer la liste des fichiers                               |\n");
    printf("|        6. Choisir un fichier à partir d'une liste                     |\n");
    printf("|        7. Exit                                                        |\n");
    printf("|        Selectioner une Option                                         |\n");
    printf("%s=========================================================================%s\n",KCYN,KNRM);
   
   printf("\n========> ");
    scanf("%d",&option);
    switch(option) {

   case 1  :{
serveur(ip,port,clientnom);

}break; 
	
   case 2  :{
client(ip,port,clientnom);
}break; 
   case 3  :{
fileserver(ip,port);

}break;

      
   case 4  :{
fileclient(ip,port);

}break;
 
   case 5 :{
      listerclient(ip,port);
} break; 
case 6 :{
      listerserver(ip,port);
} break;  
   case 7 :{
      printf("Good Bye \n");
      stop=1;
} break; 
       
   }
}
}
