#include "server.h"

struct Server init_server(const char *ip, const char *port) {
    struct Server server;
    
    server.socket_descriptor = socket(AF_INET, SOCK_STREAM, PROTOCOL);
    if(server.socket_descriptor < 0) {
        fprintf(stderr, "Failed to create a server socket\n");
        exit(1);
    }

    unsigned int portui = atoi(port);
    if(portui < 0 || portui > USHRT_MAX) {
        fprintf(stderr, "Invalid port number\n");
        exit(1);
    }

    server.address.sin_family = AF_INET;
    server.address.sin_port = htons(portui);
    if(inet_aton(ip, &server.address.sin_addr) <= 0) {
        fprintf(stderr, "Invalid remote IP address\n");
        exit(1);
    }

    if(bind(server.socket_descriptor, (struct sockaddr*)&server.address, sizeof(server.address)) == -1) {
        fprintf(stderr, "Failed to bind the socket to the ip address\n");
        exit(1);
    }

    return server;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Invalid format! Use %s <ip> <port>", argv[0]);
        exit(1);
    }

    struct Server server = init_server(argv[1], argv[2]);

    srand(time(NULL));

    // check for errors
    if(listen(server.socket_descriptor, 5) < 0) {
        fprintf(stderr, "Error in listen()\n");
        exit(1);
    }

    // listen for connections, accept only one connection
    while(1) {
        printf(YELLOW "Waiting for a client to connect...\n" RESET);
        // hold the client socket 
        int client_socket = accept(server.socket_descriptor, NULL, NULL);

        if(client_socket == -1) {
            perror("Failed to accept a client socket\n");
            return -1;
        }

        printf(GREEN "Client connected!\n" RESET);

        // receive the first messege (matrix size) from the client
        char size;
        if(recv(client_socket, &size, sizeof(char), 0) <= 0) {
            fprintf(stderr, "Something went wrong\n");
            // loop back to listening for clients instead of exiting
            exit(1);
        }

        // create the gameboard
        struct TTT_board board;
        if(init_board(&board, size) < 0) {
            fprintf(stderr, "Error encountered while creating the gameboard\n");
            close(server.socket_descriptor);
            exit(1);
        }

        struct Bot bot;
        if(init_bot(&bot, board.edge_len) < 0) {
            fprintf(stderr, "Failed to initialize the bot\n");
            exit(1);
        }

        unsigned char client_play;


        while(1) {
            int recv_status = recv(client_socket, &client_play, sizeof(char), 0);
            if(client_play == CLIENT_WON || client_play == SERVER_WON) {
                break;
            }

            /*if(client_play == OUT_OF_MOVES) {
                break;
            }*/

            mark_board(&board, client_play, CLIENT_MARKER);
            clear_terminal();

            printf("RECEIVED: %d\n", client_play);

            if(recv_status == 0) {
                printf("Client disconnected\n");
                break;
            }
            else if (recv_status < 0) {
                perror("Unknown error occured\n");
                break;
            }

            unsigned char curr_play = play(&bot, &board);
            if(curr_play == OUT_OF_MOVES) {
                send(client_socket, &curr_play, sizeof(char), 0);
                break;
            }

            mark_board(&board, curr_play, SERVER_MARKER);

            print_board(&board, SERVER_MARKER);
            send(client_socket, &curr_play, sizeof(char), 0);
        }

        delete_board(&board);
        delete_bot(&bot);
    }

    close(server.socket_descriptor);
    return 0;
}

