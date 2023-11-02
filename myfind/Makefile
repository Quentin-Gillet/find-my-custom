CC=gcc
CPPFLAGS = -MMD
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC:=$(shell find src -type f -name "*.c")

HEADER:=$(patsubst %.h,-I%.h,$(shell find src -type f -name "*.h"))
OBJ:=$(patsubst %.c,%.o,$(SRC))
DEPS:=$(patsubst %.c,%.d,$(SRC))

TARGET=myfind

all: CFLAGS += $(HEADER)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

check: all
	@ dash ./tests/tests.sh

.PHONY: clean

-include $(DEPS)

clean:
	$(RM) $(OBJ) $(OBJ_TESTS) $(DEPS) $(TARGET)
