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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port", argv[0]);
        return EXIT_FAILURE;
    }

    const int port = atoi(argv[1]);
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid port number\n");
        return EXIT_FAILURE;
    }

    // Création du socket
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Created socket\n");

    struct sockaddr_in6 serv_addr;
    serv_addr.sin6_port = htons(port);
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;

    // Bind the socket
    int res = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (res == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Socket bound\n");

    return EXIT_SUCCESS;
}
