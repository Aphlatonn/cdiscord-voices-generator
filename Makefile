# compiler and flags
CC = gcc
CFLAGS = -Wall -g

# executable name
TARGET = bot.out

# src files
SRC = main.c

# libs
LIB = -lcurl -ldiscord

# object files
OBJ = $(SRC:.c=.o)

# default target
all: $(TARGET)

# build the executable
$(TARGET): $(OBJ)
	$(CC) ${CFLAGS} $(OBJ) -o $(TARGET) $(LIB)

# clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
