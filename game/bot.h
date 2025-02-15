#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

#include <stdlib.h>

#include "tictactoe.h"

struct Bot {
    char *moves;
    char size;
};


// constructs an array for the bot
int init_bot(struct Bot* bot, char size);

// returns the coordinate of the next play
char play(struct Bot *bot, struct TTT_board *board);

// frees the bot
void delete_bot(struct Bot* bot);


#endif