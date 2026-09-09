CC = gcc
CFLAGS = -Wall -std=c89 -pedantic
TARGET = test_game

.PHONY: all run clean

all: $(TARGET)

$(TARGET): test_game.c game.c game.h
	$(CC) $(CFLAGS) -o $(TARGET) test_game.c game.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe