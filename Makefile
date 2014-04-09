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
SHAREDTARGET=liboxyale.so
STATICTARGET=liboxyale.a
TESTTARGET=run-tests
LIBDIR=lib
BINDIR=bin
OBJDIR=obj
TESTDIR=src/tests
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/, env.o ox_regcode.o ox_crypt.o ox_str.o ox_msg.o ox_client.o)
TESTS=$(addprefix $(TESTDIR)/, tests.c)
DEBUGFLAGS=-O0 -D _DEBUG
RELEASEFLAGS=-O2 -D NDEBUG # -combine -fwhole_program

all: $(SHAREDTARGET)

tests: $(SHAREDTARGET)
	$(CC) $(CFLAGS) $(TESTS) $(LDLIBS) $(TESTFLAGS) $(DEBUGFLAGS) -o $(TESTTARGET)

liboxyale.so: $(OBJS)
	mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIBDIR)/$(SHAREDTARGET)

liboxyale.a: $(OBJS)
	mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) -c $(OBJS) -o $(LIBDIR)/$(STATICTARGET)
	ar cq $(LIBDIR)/$@ $(OBJS)

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
