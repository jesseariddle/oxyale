.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS=-pedantic -Wall -Wextra -g
LDFLAGS=-shared -fPIC
LDLIBS=-loxyale
TESTFLAGS=-Llib
vpath %.c src
vpath %.h src
vpath %.o obj
vpath %.so lib
TARGET=liboxyale.so
TESTTARGET=run-tests
LIBDIR=lib
BINDIR=bin
OBJDIR=obj
TESTDIR=src/tests
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/, env.o ox_regcode.o ox_crypt.o ox_str.o ox_msg.o)
TESTS=$(addprefix $(TESTDIR)/, tests.c)
DEBUGFLAGS=-O0 -D _DEBUG
RELEASEFLAGS=-O2 -D NDEBUG # -combine -fwhole_program

all: liboxyale

tests: liboxyale
	$(CC) $(CFLAGS) $(TESTS) $(LDLIBS) $(TESTFLAGS) $(DEBUGFLAGS) -o $(TESTTARGET)

liboxyale: $(OBJS)
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
