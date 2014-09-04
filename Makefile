.SUFFIXES:
.SUFFIXES: .c .o

INCLUDE_FLAGS=-I./src/libuv/include ./lib/libuv.a
CFLAGS=-pedantic -Wall -Wextra -g
LDFLAGS=-shared -fPIC
LDLIBS=-loxyale
vpath %.c src
vpath %.h src
vpath %.o obj
vpath %.so lib
SHARED_TARGET=liboxyale.so
STATIC_TARGET=liboxyale.a
TEST_TARGET=run-tests
BUILD_DIR=build
LIB_DIR=lib
BIN_DIR=bin
OBJ_DIR=obj
TEST_FLAGS=-L$(LIB_DIR)
TEST_DIR=src/tests
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(addprefix $(OBJ_DIR)/, env.o ox_regcode.o ox_crypt.o ox_str.o ox_netmsg.o ox_net.o)
LIBS=$(wildcard $(LIB_DIR)/*.a)
TESTS=$(addprefix $(TEST_DIR)/, tests.c)
DEBUG_FLAGS=-O0 -D _DEBUG
RELEASE_FLAGS=-O2 -D NDEBUG # -combine -fwhole_program

all: $(SHARED_TARGET) $(STATIC_TARGET) $(TESTS)

tests: $(SHARED_TARGET) $(STATIC_TARGET)
	$(CC) $(CFLAGS) $(TESTS) $(TEST_FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) ./$(LIB_DIR)/$(STATIC_TARGET) -o $(TEST_TARGET)

liboxyale.so: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(LDFLAGS) $(OBJS) -o $(BUILD_DIR)/$(SHARED_TARGET)
	cp $(BUILD_DIR)/$(SHARED_TARGET) $(LIB_DIR)/$(SHARED_TARGET)

liboxyale.a: $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(OBJS) -o $(BUILD_DIR)/$(STATIC_TARGET)
	ar cq $(BUILD_DIR)/$@ $(OBJS)
	cp $(BUILD_DIR)/$(STATIC_TARGET) $(LIB_DIR)/$(STATIC_TARGET)

$(OBJS): $(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c -o $@ $<

$(TESTS): $(TEST_DIR)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(TEST_DIR) $@

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_DIR)/$(SHARED_TARGET) $(LIB_DIR)/$(STATIC_TARGET)
	rm -rf $(BUILD_DIR)
	rm -f $(TEST_TARGET)
	rm -rf $(TEST_TARGET).dSYM
