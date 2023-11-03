CC=gcc
CPPFLAGS = -MMD
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -Isrc/includes/

SRC:=$(shell find src -type f -name "*.c")

OBJ:=$(patsubst %.c,%.o,$(SRC))
DEPS:=$(patsubst %.c,%.d,$(SRC))

TARGET=myfind

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

check: all
	@ dash ./tests/tests.sh

.PHONY: clean

-include $(DEPS)

clean:
	$(RM) $(OBJ) $(DEPS) $(TARGET)
