#include "tictactoe.h"

int init_board(struct TTT_board* board, char size) {
    if(size > MAX_SIZE) {
        return -1;
    }

    board -> array = calloc(size * size, sizeof(char));
    if(!board -> array) {
        return -1;
    }

    board -> size = 0;
    board -> edge_len = size;
    board -> capacity = size * size;
    return 0;
};

int mark_board(struct TTT_board* board, char index, char marker) {
    if(!board || !(board -> array)) {
        return -1;
    }

    if(index >= (board -> capacity)) {
        return -1;
    }
    
    if(*(board -> array + index) == 0) {
        *(board -> array + index) = marker;
        ++(board -> size);
    }
    
    return 0;
}

int check_win(struct TTT_board* board, char marker) {
    if(!board || !(board -> array)) {
        return -1;
    }

    char* row_count = calloc(board -> edge_len, sizeof(char));
    char diag1_count = 0;
    char diag2_count = 0;

    char index = 0;

    for(int i = 0; i < board -> edge_len; ++i) {
        char col_check = 0;
        for(int j = 0; j < board -> edge_len; ++j) {
            if(*(board -> array + index) == marker) {
                ++col_check;
                ++*(row_count + j);
                
                if(i == j) {
                    ++diag1_count;
                }

                if(i == board -> edge_len - 1 - j) {
                    ++diag2_count;
                }
            }

            ++index;
        }

        if(col_check == board -> edge_len) {
            free(row_count);
            return 1;
        }
    }

    // check the rows
    for(int i = 0; i < board -> edge_len; ++i) {
        if(*(row_count + i) == board -> edge_len) {
            free(row_count);
            return 1;
        }
    }

    free(row_count);

    return diag1_count == board -> edge_len || diag2_count == board -> edge_len? 1 : 0;
}

int clear_board(struct TTT_board* board) {
    if(!board || (!board -> array)) {
        return -1;
    }

    memset(board -> array, 0, board -> capacity);
    return 0;
}

int print_board(struct TTT_board* board, int marker) {
    if(!board || (!board -> array)) {
        return -1;
    }

    char curr_point = 0;
    for(int i = 0; i < board -> edge_len; ++i) {
        for(int j = 0; j < board -> edge_len; ++j) {
            curr_point = *(board -> array + (j * board -> edge_len) + i);
            if(curr_point == marker) {
                printf("O ");
            }
            else if(curr_point == 0) {
                printf("_ ");
            }
            else {
                printf("X ");
            }
        }

        printf("\n");
    }

    return 0;
}

char encode_cords(char x, char y, char size) {
    return x * size + y;
}

void clear_terminal() {
    printf("\e[1;1H\e[2J");
}

void delete_board(struct TTT_board *board) {
    if(!board) {
        return;
    }

    if(!board -> array) {
        board -> size = 0;
        board -> capacity = 0;
        board -> edge_len = 0;
        return;
    }

    free(board -> array);
    board -> array = NULL;
    board -> size = 0;
    board -> capacity = 0;
    board -> edge_len = 0;
}

int is_stall(struct TTT_board* board) {
    return board -> size < board -> capacity? 0: 1;
}