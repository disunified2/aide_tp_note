//
// Created by Théo Delaroche on 21/11/2025.
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

#define BUFSIZE 256

#include <stdio.h>

int main (const int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s port", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65536) {
        fprintf(stderr, "Invalid port number, port must be between 1 and 65536");
        return EXIT_FAILURE;
    }

    // Création du socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Created socket\n");

    // Création du sockaddr_in du serveur
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    bzero(addr.sin_zero,8);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (ret == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Bind successful\n");

    char buf[BUFSIZE];

    ssize_t bytes_recv = recvfrom(sockfd, buf, BUFSIZE, 0, NULL, NULL);
    if (bytes_recv == -1) {
        perror("sendto");
        close(sockfd);
        return EXIT_FAILURE;
    }

    buf[bytes_recv] = '\0';

    fprintf(stdout, "Message received : %s\n", buf);

    close(sockfd);
    return EXIT_SUCCESS;
}
