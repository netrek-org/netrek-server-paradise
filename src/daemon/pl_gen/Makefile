# Generated automatically from Makefile.in by configure.
# Makefile.in for Paradise server src/common 3.1
# Generated automatically from Makefile.in by configure.



bindir=${exec_prefix}/bin
datadir=${prefix}/share
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
libexecdir=${exec_prefix}/libexec
mandir=${prefix}/man
prefix=/usr/local
srcdir=.
top_srcdir=../../..

AR=ar
ARFLAGS=cruv
RANLIB=ranlib

CC=gcc
CPP=gcc -E

CFLAGS=-g -O2
CPPFLAGS= -I../../include
DEFS= -DHAVE_CONFIG_H
LDFLAGS=
LIBS= -lm

INSTALL=/opt/gnu/bin/install -c
INSTALL_SCRIPT=${INSTALL_PROGRAM}
INSTALL_DATA=${INSTALL} -m 644

####

PLGEN_OBJS=pl_gen0.o \
           pl_gen1.o \
	   pl_gen2.o \
	   pl_gen3.o \
	   pl_gen4.o \
	   pl_gen5.o \
	   pl_gen6.o \
	   pl_gen7.o \
	   pl_gen8.o

all: pl_gen.a

pl_gen.a: $(PLGEN_OBJS)
	$(AR) $(ARFLAGS) $@ $(PLGEN_OBJS)
	$(RANLIB) $@

clean:
	-rm -f $(PLGEN_OBJS) plgen.a

# nothing to do for install
install:

distclean: clean
	-rm -f Makefile
