CC = gcc

CFLAGS = -Wall -I ../game

TARGET = server

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c ../game/tictactoe.c ../game/bot.c

clean:
	$(RM) $(TARGET)