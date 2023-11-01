CC=gcc
CPPFLAGS = -MMD
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS=

SRC:=$(shell find src -type f -name "*.c")
HEADER:=$(shell find src -type f -name "*.h")
HEADER:=$(patsubst %.h,-I%.h,$(HEADER))
OBJ:=$(patsubst %.c,%.o,$(SRC))
DEPS:=$(patsubst %.c,%.d,$(SRC))
TARGET=myfind

SRC_TESTS:=$(shell find tests -type f -name "*.c")
OBJ_TESTS:=$(patsubst %.c,%.o,$(SRC_TESTS))

all: CFLAGS += $(HEADER)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

check: CFLAGS += -g $(shell pkg-config --cflags criterion)
check: LDFLAGS += $(shell pkg-config --libs criterion)
check: $(OBJ) $(OBJ_TESTS)
	$(CC) $(LDFLAGS) $^ -o check

.PHONY: clean

-include $(DEPS)

clean:
	$(RM) $(OBJ) $(OBJ_TESTS) $(DEPS) $(TARGET) check
