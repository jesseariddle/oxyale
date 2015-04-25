TARGET := test

TGT_LDFLAGS :=  -Ldeps/libuv/build/Release -L${TARGET_DIR}
TGT_LDLIBS  := -luv -loxyale
TGT_PREREQS := liboxyale.a

SOURCES := oxyale-test.c

SRC_INCDIRS := \
    deps/libuv/include \
    include \
    src
