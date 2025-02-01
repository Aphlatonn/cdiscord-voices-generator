# compiler and flags
CC = gcc
CFLAGS = -Wall -g

# executable name
TARGET = bot.out

# src files
SRC = main.c database.c commands.c utils.c parser.c

# libs
LIB = -lcurl -ldiscord -lsqlite3

# object files
OBJ = $(SRC:.c=.o)

# default target
all: $(TARGET)

# build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIB)

# compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET) *.log
