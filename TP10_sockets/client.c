//
// Created by Théo Delaroche on 28/11/2025.
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
#include <net/if.h>

int main(int argc, char *argv[]) {
    if (argc > 4 || argc < 3) {
        fprintf(stderr, "Usage : %s adress port [Interface Name]", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[2]);
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Port number must be between 1 and 65535");
        return EXIT_FAILURE;
    }

    // Création du socket
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Created socket\n");

    // Création du sockaddr_in du serveur
    struct sockaddr_in6 serv_addr;
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_scope_id = if_nametoindex(argv[3]);
    serv_addr.sin6_port = htons(port);

    int err = inet_pton(AF_INET6, argv[1], &serv_addr.sin6_addr);
    if (err == -1) {
        perror("inet_pton");
        close(sockfd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Address computed\n");

    // Connexion
    fprintf(stdout, "Attempting to connect to server\n");
    int connect_fd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connect_fd == -1) {
        perror("connect");
        close(sockfd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Connected to server\n");

    // Fermeture du socket
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    return EXIT_SUCCESS;
}