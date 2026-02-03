/* ============================================================
   Fichier  : intercom.c
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : SAHLI Aziz
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce module permet la communication entre le programme
   principal et la simulation. Il permet juste d'envoyer des
   commandes via UDP su un port spécifié. La simulation va
   écouter ce port pour recevoir les commandes à executer.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// Adresse locale
#define IP "127.0.0.1"

// Port choisi. Doit être le même sur la simulation
#define PORT 1234

void envoyer_commande(char *commande) {
    int sockfd;
    struct sockaddr_in serveur;

    // Creer un socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("La création du socket a échoué");
        exit(EXIT_FAILURE);
    }

    // Configurer le struct serveur
    memset(&serveur, 0, sizeof(serveur));
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(PORT);
    serveur.sin_addr.s_addr = inet_addr(IP);

    // Envoyer la commande
    sendto(sockfd, commande, strlen(commande), 0, (const struct sockaddr *)&serveur, sizeof(serveur));

    printf("Commande envoyée: %s\n", commande);
    
    close(sockfd);
}