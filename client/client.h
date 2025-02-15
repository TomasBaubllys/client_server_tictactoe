#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <limits.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../game/tictactoe.h"

// #define DEBUG 0

int read_cords(char size, char *x, char *y);

char read_size();

#endif // CLIENT_H_INCLUDED