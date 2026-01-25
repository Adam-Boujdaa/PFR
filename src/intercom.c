#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 1234

void envoyer_commande(char *commande) {
    int sockfd;
    struct sockaddr_in serveur;

    // Creer un socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configurer le struct serveur
    memset(&serveur, 0, sizeof(serveur));
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(PORT);
    serveur.sin_addr.s_addr = inet_addr(IP);

    // Envoyer la commande
    sendto(sockfd, commande, strlen(commande), 0, 
           (const struct sockaddr *)&serveur, sizeof(serveur));

    printf("Commande envoyée: %s\n", commande);
    
    close(sockfd);
}