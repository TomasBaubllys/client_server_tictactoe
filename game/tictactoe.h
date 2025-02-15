#ifndef TICTACTOE_H_INCLUDED
#define TICTACTOE_H_INCLUDED

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define DEFAULT_MATRIX_SIZE 3
#define MAX_SIZE 15

#define CLIENT_MARKER 1
#define SERVER_MARKER 2

#define SERVER_WON UCHAR_MAX
#define CLIENT_WON UCHAR_MAX - 1
#define OUT_OF_MOVES UCHAR_MAX - 2

struct TTT_board {
    char *array;
    unsigned char size;
    unsigned char capacity;
    unsigned char edge_len;
};

// initializes the game board, allocates the memory
int init_board(struct TTT_board* board, char size);

// marks the coordinate one the board using y = index / size, x = index mod size
int mark_board(struct TTT_board* board, char index, char marker);

// checks if a player specified by a marker has won or not returns 1 if yes
int check_win(struct TTT_board* board, char marker);

// returns 1 if the board is full, 0 otherwise O(N) bad
int is_stall(struct TTT_board* board);

// resets the board 
int clear_board(struct TTT_board* board);

// prints the board (current players markers are marked as O)
int print_board(struct TTT_board* board, int marker);

// encodes two coordinates into a single byte
char encode_cords(char x, char y, char size);

// clears the terminal screen
void clear_terminal();

// deletes the boards resources
void delete_board(struct TTT_board *board);

#endif // TICTACTOE_H_INCLUDED