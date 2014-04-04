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
TESTTARGET=run-tests
LIBDIR=lib
BINDIR=bin
OBJDIR=obj
TESTDIR=src/tests
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/, env.o opal_regcode.o opal_crypt.o opal_string.o opal_message.o)
TESTS=$(addprefix $(TESTDIR)/, tests.c)
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
