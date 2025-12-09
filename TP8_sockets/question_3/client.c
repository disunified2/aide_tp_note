//
// Created by Théo Delaroche on 14/11/2025.
//
#define DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFSIZE 10


int main(const int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <address_str> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[2]);
    if (port < 1 || port > 65536) {
        fprintf(stderr, "Invalid port number, port must be between 1 and 65536");
        return EXIT_FAILURE;
    }

    // Création du socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Created socket\n");

    // Création du sockaddr_in du serveur
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bzero(serv_addr.sin_zero,8);

    // Récupération de l'adresse et mise en forme correcte avec inet_aton(...)
    int err = inet_aton(argv[1], &serv_addr.sin_addr);
    if (err == -1) {
        perror("inet_aton");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Adress computed\n");

    // Connexion
    fprintf(stdout, "Attempting to connect to server\n");
    int connect_fd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect_fd == -1) {
        perror("connect");
        close(sockfd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Connected to server\n");

    // Lecture de ce qui arrive du serveur
    char buf[BUFSIZE];
    for (int i = 0; i < 10; i++) {
        ssize_t recvlen = recv(sockfd, buf, BUFSIZE, 0);
        if (recvlen == -1) {
            perror("recv");
            return EXIT_FAILURE;
        }

        // Affichage du buffer
        buf[recvlen] = '\0';
        printf("%s", buf);
    }

    // Fermeture du socket
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    fprintf(stdout, "Closing socket\n");

    return EXIT_SUCCESS;
}