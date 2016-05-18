rm=rm -f
CC=cc
AR=ar cq
RANLIB=ranlib
LIBNAME=libjpec.a
CLINAME=jpec
# LIB_BUILD_MODE: release or debug
LIB_BUILD_MODE=release

INCLUDES= -Isrc -Iinclude

DEFINES= $(INCLUDES)
_release_cflags= -Os -DNDEBUG
_debug_cflags= -g -O0
CFLAGS= $(DEFINES) -std=c99 -Wall -Werror
CFLAGS+= $(_$(LIB_BUILD_MODE)_cflags)

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

%.c.o:
	$(CC) $(CFLAGS) -c $*.c

all: $(CLINAME) $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(rm) $@
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

$(CLINAME): $(LIBNAME) main.c
	$(CC) -o $@ main.c -std=c99 `pkg-config --cflags --libs opencv` -Iinclude -L. -l$@

clean:
	$(rm) $(OBJS) $(CLINAME) $(LIBNAME)
