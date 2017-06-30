## makefile.am
## Copyright 2016 Mac Radigan
## All Rights Reserved

# `pkg-config fuse3 --libs`
# `pkg-config fuse3 --cflags`

AM_LDFLAGS  = -pthread                                                       \
              `pkg-config fuse3 --libs`                                      \
              -L/usr/lib -ldl -lm                                             

AM_CPPFLAGS = -I$(top_srcdir)/include                                        \
              -D_FILE_OFFSET_BITS=64                                         \
              `pkg-config fuse3 --cflags`                                    \
              -std=c++11
AM_CFLAGS   = -I$(top_srcdir)/include                                        \
              -D_FILE_OFFSET_BITS=64                                         \
              `pkg-config fuse3 --cflags`                                     
AM_FCFLAGS  = -Jmod

durian_ldadd             = libdurian.la                                      \
                           -ldl -lm                                           

bin_PROGRAMS             = durian

durian_SOURCES           = src/apps/durian.c
durian_LDADD             = $(durian_ldadd)

lib_LTLIBRARIES          = libdurian.la
libdurian_la_SOURCES     = src/packages/common/status.cc
libdurian_la_LIBADD      = -lrt

libdurian_la_LDFLAGS     = -export-symbols-regex '^durian_[^_]' -fPIC        \
                           -Wl,--no-undefined                                \
                           `pkg-config fuse3 --libs`                         \
                           -shared -pthread                                   

pkginclude_HEADERS       = include/packages/common/status.h                  \
                           include/packages/common/thread.h

## *EOF*
