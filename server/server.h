#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

#include "../game/bot.h"
#include "colours.h"

#define PROTOCOL 0

#define SERVER_PRINTING

struct Server {
    int socket_descriptor;
    struct sockaddr_in address;
};

// Initializes the struct server, does not start listening for connections!
struct Server init_server(const char *ip, const char *port);

#endif // SERVER_H_INCLUDED