// Adresse de socket IPv4
struct sockaddr_in {
    sa_family_t sin_family;     /* AF_UNIX | AF_INET | AF_INET6 */
    in_port_t  sin_port;        /* 16 bits, big-endian */
    struct in_addr sin_addr;    /* adresse IP 32 bits, big-endian */
    char sin_zero[0];           /* non utilisé */
};
struct in_addr {                /* adresse IP 32 bits */
    in_addr_t s_addr;           /* big endian */
}

struct sockaddr {
    sa_family_t sa_family;      /* famille d'adresse AF_xxx */
    char sa_data[14];           /* données de l'adresse */
};

// commande et résultat pour savoir de quel type est in_addr_t
find /usr/include -name '*.h' | xargs grep 'typedef.*in_addr_t'
/usr/include/netinet/in.h:typedef uint32_t in_addr_t;

struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    socklen_t ai_addrlen;
    struct sockaddr* ai_addr;
    char* ai_canonname;
    struct addrinfo* ai_next;
};

/* Fonctions de conversion */
htons // host to network short
htonl // host to network long

ntohs // network to host short
ntohl // network to host long

/* Fonctions communes */
int socket(int family, int type, int protocol); // renvoie le descripteur du socket ou -1 en cas d'erreur

int bind (int sockfd, const struct sockaddr* addr, socklen_t addrlen); // renvoie 0 si succès, -1 en cas d'erreur

int close(int filedes); // renvoie 0 si succès, -1 en cas d'erreur

/* Fonctions client */
int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen); // renvoie 0 si succès, -1 en cas d'erreur

int inet_aton(const char* cp, struct in_addr* inp); // renvoie valeur non nulle si succès, 0 en cas d'erreur

// Obtenir une adresse de socket
int getaddrinfo (const char* node, const char* service, const struct addrinfo* hints, struct addrinfo** res);
// renvoie 0 si succès, valeur non nulle sinon

/* Fonctions serveur */
int listen(int sockfd, int backlog); // renvoie 0 si succès, -1 en cas d'erreur

int accept (int sockfd, struct sockaddr* ad, socklen_t* adlen); // renvoie un entier non négatif représentant le descripteur de la socket créée si succès, -1 en cas d’erreur

/* Fonction d'envoi client/serveur avec TCP */
ssize_t send (int sockfd, const void* buf, size_t len, int flags); // renvoie nombre d’octets réellement envoyés si succès, 0 si déconnexion de l’autre extrémité, ou -1 en cas d’erreur

ssize_t recv (int sockfd, void* buf, size_t len, int flags); // renvoie le nombre d’octets réellement reçus si succès, 0 si déconnexion de l’autre extrémité, ou -1 en cas d’erreur

int shutdown (int sockfd, int how); // 0 si succès, -1 en cas d’erreur

/* Fonctions émetteur/récepteur avec UDP */
ssize_t sendto (int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen);
// renvoie nombre d’octets réellement envoyés, si succès, -1 en cas d’erreur

ssize_t recvfrom (int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen);
// renvoie nombre d’octets réellement reçus si succès, -1 en cas d’erreur
