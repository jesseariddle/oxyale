UV_PATH=$(shell pwd)/deps/libuv
UV_LIB=$(UV_PATH)/out/Debug/libuv.a
CFLAGS=-g -O2 -Wall -I$(UV_PATH)/include
uname_S=$(shell uname -s)
LIBS=
LDFLAGS=
SRC=src
BUILD=build
INCL_DIR=$(SRC)
INCLUDES=$(wildcard $(INCL_DIR)/*.h)
SOURCES=$(SRC)/tests/alcazar-test.c $(wildcard $(SRC)/*.c)
OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c))
EXECUTABLE=$(BUILD)/alcazar-test

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

all: $(SOURCES) $(EXECUTABLE) # libuv.a liboxyale.a

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) $< -o $@

clean: 
	$(RM) $(BUILD)/*.o

.PHONY: all clean
