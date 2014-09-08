SRC=src
EXESRC=src/exe
OBJ=obj
LIB=lib
LIBS=
LDFLAGS=
INC=$(SRC)
BUILD=build
LIBUV_PATH=$(shell pwd)/deps/libuv
LIBUV=$(LIBUV_PATH)/build/Debug/libuv.a
CFLAGS=-g -O2 -Wall #-I$(UV_PATH)
INCS=$(wildcard $(INC)/*.h)
SRCS=$(wildcard $(SRC)/*.c)
EXESRCS=$(wildcard $(EXESRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SRCS))
NAME=oxyale
EXE=oxyale-test

MKDIR=mkdir
uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

all: clean $(EXE) # libuv.a liboxyale.a

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INC) -I$(SRC) $(LIBUV) $(EXESRC)/$(EXE).c $(OBJS) -o $(EXE)

$(BUILD)/%.o: $(SRC)/%.c $(INC)/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I$(INC) -I$(SRC) -c $< -o $@

setup:
	$(MKDIR) -p $(BUILD)

install:
##    $(INSTALL) -m 755 -o 0 -g 0 -d $(DESTDIR)/usr/local/bin/
##    $(INSTALL) -m 755 -o 0 -g 0 $(BINARY) $(DESTDIR)/usr/local/bin/$(BINARY)
##    $(INSTALL) -m 755 -o 0 -g 0 -d $(DESTDIR)/usr/local/$(NAME)/ui/
##    $(INSTALL) -m 644 -o 0 -g 0 ./ui/*.ui $(DESTDIR)/usr/local/$(NAME)/ui/
#   $(INSTALL) -m 755 -o 0 -g 0 -d $(DESTDIR)/usr/local/$(NAME)/model/
#   $(INSTALL) -m 644 -o 0 -g 0 ./model/*.model $(DESTDIR)/usr/local/$(NAME)/model/

clean: 
	$(RM) -f $(EXE) $(OBJS)

distclean: clean

help:
	@$(ECHO) "Targets:"
	@$(ECHO) "all     - build/compile what is necessary"
	@$(ECHO) "clean   - cleanup old .o and .bin"
	@$(ECHO) "install - not yet fully supported"

.PHONY: all clean
