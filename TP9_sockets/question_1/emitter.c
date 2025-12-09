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

int main (const int argc, const char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage : %s address port_dest local_port", argv[0]);
        return EXIT_FAILURE;
    }

    int port_dest = atoi(argv[2]);
    if (port_dest < 1 || port_dest > 65536) {
        fprintf(stderr, "Invalid port number, port must be between 1 and 65536");
        return EXIT_FAILURE;
    }

    int local_port = atoi(argv[3]);
    if (local_port < 1 || local_port > 65536) {
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

    // Création du sockaddr_in
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(local_port);
    bzero(addr.sin_zero,8);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Bind successful\n");

    char buf[BUFSIZE] = "Message to send";

    struct sockaddr_in recv_addr;
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(port_dest);
    inet_aton(argv[3], &recv_addr.sin_addr);
    bzero(recv_addr.sin_zero,8);

    ssize_t bytes_sent = sendto(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &recv_addr, sizeof(recv_addr));
    if (bytes_sent == -1) {
        perror("sendto");
        close(sockfd);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Sent %ld bytes\n", bytes_sent);

    close(sockfd);
    return EXIT_SUCCESS;
}
