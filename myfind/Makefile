CC=gcc
CPPFLAGS = -MMD
CFLAGS=-std=c99 -pedantic -Isrc/includes/ -g# -fsanitize=address
#LDFLAGS += -fsanitize=address

SRC:=$(shell find src -type f -name "*.c")

OBJ:=$(patsubst %.c,%.o,$(SRC))
DEPS:=$(patsubst %.c,%.d,$(SRC))

TARGET=myfind

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

check: clean all
	@ dash ./tests/tests.sh

leaks: CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -Isrc/includes/ -g
leaks: LDFLAGS =
leaks: clean all
	@ leaks --atExit -- ./$(TARGET) tests/testing -type f -print -o -type d -print -o -type l -print -o -type p -print -o -type b -print -o -type s -print -o -type k -print

.PHONY: clean

-include $(DEPS)

clean:
	@ $(RM) $(OBJ) $(DEPS) $(TARGET)
