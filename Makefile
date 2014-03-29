.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS=-pedantic -Wall -Wextra -g
LDFLAGS=-shared -fPIC
LDLIBS=-lopenpalace
TESTFLAGS=-Llib
vpath %.c src
vpath %.h src
vpath %.o obj
vpath %.so lib
TARGET=libopenpalace.so
TESTTARGET=test
LIBDIR=lib
BINDIR=bin
OBJDIR=obj
TESTDIR=src/test
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/, platform.o regcode.o)
TESTS=$(addprefix $(TESTDIR)/, regcodetest.c)
DEBUGFLAGS=-O0 -D _DEBUG
RELEASEFLAGS=-O2 -D NDEBUG -combine -fwhole_program

all: libopenpalace

tests: libopenpalace
	$(CC) $(CFLAGS) $(TESTS) $(LDLIBS) $(TESTFLAGS) -o $(TESTTARGET)

libopenpalace: $(OBJS)
	mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIBDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: %.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TESTS): $(TESTDIR)
	$(CC) -c $(TESTDIR) $@

.PHONY: clean

clean:
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)
	rm -f $(TESTTARGET)
	rm -rf $(TESTTARGET).dSYM
