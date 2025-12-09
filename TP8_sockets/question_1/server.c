//
// Created by Théo Delaroche on 14/11/2025.
//
#define DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFSIZE 103
#define LISTENING_ADDRESS "127.0.0.1"

int main(const int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // récupération de ce qui est passé en paramètre
    const int port = atoi(argv[1]);
    if (port < 1 || port > 65536) {
        fprintf(stderr, "Invalid port number, the port should be between 1 and 65536");
        return EXIT_FAILURE;
    }

    // Création du socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Created socket\n");

    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_family = AF_INET;
    bzero(serv_addr.sin_zero, 8);
    int err = inet_aton(LISTENING_ADDRESS, &serv_addr.sin_addr);
    if (err == 0) {
        fprintf(stderr, "Error while parsing adress %s\n", LISTENING_ADDRESS);
        close(sockfd);
        return EXIT_FAILURE;
    }

    int enable = 1;
    err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if(err == -1) {
        perror("setsockopt");
        if (close(sockfd) == -1) {
            perror("close socket");
            return EXIT_FAILURE;
        }
    }

    // Bind the socket
    int res = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (res == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Socket bound\n");

    // listen to the socket
    res = listen(sockfd, 100);
    if (res == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    struct sockaddr_in peer_addr;
    socklen_t peer_address_size = sizeof(peer_addr);
    // Accept incoming connection
    fprintf(stdout, "Waiting for client connections...\n");
    int client_sock = accept(sockfd, (struct sockaddr *) &peer_addr, &peer_address_size);
    if (client_sock == -1) {
        perror("accept");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Connection established\n");

    // Reading of the first 100 values of pi
    FILE *f = fopen("pi.txt", "r");
    if (f == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    char *pi = malloc(BUFSIZE * sizeof(char));
    fread(pi, 1, BUFSIZE, f);
    pi[BUFSIZE] = '\0';
    fclose(f);

    fprintf(stdout, "Sending message...\n");
    ssize_t byte_sent = send(client_sock, pi, BUFSIZE, 0);
    if (byte_sent == -1) {
        perror("recv");
        close(client_sock);
        close(sockfd);
        free(pi);
        return EXIT_FAILURE;;
    }
    fprintf(stdout, "Message sent !\n");

    // Close the socket
    shutdown(client_sock, SHUT_RDWR);
    close(client_sock);
    close(sockfd);
    free(pi);

    return EXIT_SUCCESS;
}