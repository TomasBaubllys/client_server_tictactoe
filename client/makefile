CC = gcc

CFLAGS = -Wall -I ../game

TARGET = client

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c ../game/tictactoe.c

clean:
	$(RM) $(TARGET)