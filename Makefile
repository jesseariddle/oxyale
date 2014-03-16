.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS=-pedantic -Wall -Wextra -g
LDFLAGS=-shared -fPIC
LDLIBS=-lopal
TESTFLAGS=-Llib
vpath %.c src
vpath %.h src
vpath %.o obj
vpath %.so lib
TARGET=libopal.so
TESTTARGET=test
LIBDIR=lib
#BINDIR=bin
OBJDIR=obj
TESTDIR=src/test
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/, regcode.o)
TESTS=$(addprefix $(TESTDIR)/, regcodetest.c)
DEBUGFLAGS=-O0 -D _DEBUG
RELEASEFLAGS=-O2 -D NDEBUG -combine -fwhole_program

all: libopal

tests: libopal
	$(CC) $(CFLAGS) $(TESTS) $(LDLIBS) $(TESTFLAGS) -o $(TESTTARGET)

libopal: $(LIBDIR) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIBDIR)/$(TARGET)

$(OBJS): $(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TESTS): $(TESTDIR)
	$(CC) -c $(TESTDIR) $@

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/$(OBJS)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f $(TESTTARGET)
	rm -rf $(TESTTARGET).dSYM
