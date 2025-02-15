#include "client.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Invalid format! Use %s <port>", argv[0]);
        exit(1);
    }

    unsigned int port = atoi(argv[1]);
    if(port < 0 || port > USHRT_MAX) {
        fprintf(stderr, "Invalid port number\n");
        exit(1);
    }

    int client_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket_descriptor < 0) {
        fprintf(stderr, "Failed to create a socket\n");
        exit(1);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(client_socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address));

    if(connection_status == -1) {
        perror("Failed to connect to the server\n");
        close(client_socket_descriptor);
        return -1;
    }

    // Connection success from this point
    printf("Successfully connected to the Tic Tac Toe server!\n");

    // Create a default tic tac toe board for now
    struct TTT_board board;
    if(init_board(&board, read_size()) < 0) {
        fprintf(stderr, "Failed to create a game board\n");
        close(client_socket_descriptor);
        exit(1);
    }

    // send the size as the first messege to the server
    send(client_socket_descriptor, &(board.edge_len), sizeof(char), 0);

    unsigned char received = 0;
    char x = 0, y = 0;
    unsigned char encoded = 0;

    while(read_cords(board.edge_len, &x, &y) == 0) {
        // coordinates are flipped arround for the visual representation
        encoded = encode_cords(y, x, board.edge_len);
        mark_board(&board, encoded, CLIENT_MARKER);

        if(check_win(&board, CLIENT_MARKER) == 1) {
            // inform the server
            encoded = CLIENT_WON;
            send(client_socket_descriptor, &encoded, sizeof(char), 0);
            clear_terminal();
            print_board(&board, CLIENT_MARKER);
            printf("Winner winner chicken dinner\n");
            break;
        }

        // send the char from the console
        send(client_socket_descriptor, &encoded, sizeof(char), 0);
        
        // wait for the response and print it
        int recv_status = recv(client_socket_descriptor, &received, sizeof(char), 0);

        if(recv_status == 0) {
            fprintf(stderr, "Server died...\n");
            break;
        }
        else if (recv_status < 0) {
            print_board(&board, CLIENT_MARKER);
            perror("Unknown error occured\n");
            break;
        }

        if(received == OUT_OF_MOVES) {
            clear_terminal();
            print_board(&board, CLIENT_MARKER);
            printf("Its a drawwww\n");
            break;
        }

        mark_board(&board, received, SERVER_MARKER);

        if(check_win(&board, SERVER_MARKER) == 1) {
            clear_terminal();
            print_board(&board, CLIENT_MARKER);
            received = SERVER_WON;
            send(client_socket_descriptor, &received, sizeof(char), 0);
            printf("You loose looser\n");
            break;
        }

        clear_terminal();
        print_board(&board, CLIENT_MARKER);

        #ifdef DEBUG
            printf("RECEIVED: %d\n", received);
        #endif


        if(is_stall(&board) == 1) {
            encoded = OUT_OF_MOVES;
            send(client_socket_descriptor, &encoded, sizeof(char), 0);
            clear_terminal();
            print_board(&board, CLIENT_MARKER);
            printf("Its a drawwww\n");
            break;
        }
    }

    delete_board(&board);
    close(client_socket_descriptor);

    return 0;
}

int read_cords(char size, char *x, char *y) {
    int x_local, y_local;
    printf("Please enter x and y coordinates separeted by a comma\n");
    while(1) {
        if(scanf(" %d , %d", &x_local, &y_local) == 2) {
            if(x_local < size && y_local < size && x_local >= 0 && y_local >= 0) {
                break;
            }

            printf("x and y must be between 0 and %d\n", size);
        }
        else {
            while(getchar() != '\n');
            printf("Invalid format must be x,y\n");
        }
    }

    *x = (char)x_local;
    *y = (char)y_local;

    return 0;
}

char read_size() {
    int size;
    printf("Enter the tic tac toe board size must be between 2 and %d\n", MAX_SIZE + 1);
    while (1) {
        if(scanf(" %d", &size) == 1) {
            if(size <= MAX_SIZE && size > 2) {
                return (char)size;
            }
            else {
                printf("Size must be between more than 2 and less than %d\n", MAX_SIZE + 1);
            }
        }
        else {
            while(getchar() != '\n');
            printf("Invalid symbols format must be: <size>\n");
        }
    }

    return DEFAULT_MATRIX_SIZE;
}

