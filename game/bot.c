#include "bot.h"

int init_bot(struct Bot* bot, char size) {
    bot -> size = size;

    char abs_size = size * size;

    bot -> moves = calloc(abs_size, sizeof(char));
    if(!bot -> moves) {
        fprintf(stderr, "Failed to initialize the bot\n");
        return -1;
    }

    // generate an array of 0,...,size - 1
    for(int i = 0; i < abs_size; ++i) {
        *(bot -> moves + i) = i;
    }

    // random sort
    for(int i = 0; i < abs_size; ++i) {
        int j = rand() % (abs_size + 1);

        char temp = *(bot -> moves + i);
        *(bot -> moves + i) = *(bot -> moves + j);
        *(bot -> moves + j) = temp;
    }

    return 0;
}

char play(struct Bot *bot, struct TTT_board *board) {
    if(board -> edge_len != bot -> size) {
        fprintf(stderr, "Bot setup and board sizes differ\n");
        return 0;
    }

    char abs_size = board -> capacity;
    for(int i = 0; i < abs_size; ++i) {
        if(*(bot -> moves + i) != -1 && board -> array[*(unsigned char*)(bot -> moves + i)] == 0) {
            char ret = *(bot -> moves + i);
            *(bot -> moves + i) = -1;
            return ret;
        }
    }

    return OUT_OF_MOVES;
}

void delete_bot(struct Bot* bot) {
    if(!bot) {
        return;
    }

    if(!bot -> moves) {
        bot -> size = 0;
        return;
    }

    free(bot -> moves);
    bot -> moves = NULL;
    bot -> size = 0;
}