TARGET := liboxyale.a

TGT_LDFLAGS := -L../deps/libuv/build/Release
TGT_LDLIBS  := -luv

SOURCES := \
    oxl_common.c \
    oxl_crypt.c \
    oxl_net.c \
    oxl_propstore.c \
    oxl_regcode.c \
    oxl_user.c \
    oxl_client.c

SRC_INCDIRS := \
    . \
    ../deps/libuv/include \
    ../include

# SUBMAKEFILES = 
