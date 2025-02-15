#include "tictactoe.h"
#include <stdio.h>

int main(void) {
    struct TTT_board board;
    init_board(&board, 3);

    // printf("DID WIN?: %d\n", check_win(&board, 1));

    mark_board(&board, 2, 1);
    mark_board(&board, 4, 1);
    mark_board(&board, 5, 2);
    mark_board(&board, 6, 1);
    mark_board(&board, 0, 2);

    printf("DID WIN?: %d\n", check_win(&board, 1));

    print_board(&board, 1);

    return 0;
}