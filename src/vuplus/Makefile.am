#
# Makefile for the XBMC Video Disk Recorder PVR AddOn
#
# See the README for copyright information and
# how to reach the author.
#

LIBS   =-ldl -lcurl -lpthread
LIBDIR = ../../addons/pvr.vuplus
LIB    = ../../addons/pvr.vuplus/XBMC_vuplus.pvr

lib_LTLIBRARIES = libvuplus.la

libvuplus_la_SOURCES = \
	client.cpp \
	xmlParser.cpp \
	VuData.cpp

libvuplus_la_LDFLAGS = \
	-avoid-version


CPPFLAGS = \
	$(ARCH_DEFINES)

INCLUDES = -I$(srcdir)/.. -I$(srcdir)/../.. -I$(srcdir)/../../include/xbmc -D__STDC_CONSTANT_MACROS -D_LINUX

EXTRA_DIST = \
	project

XBMC_vuplus.pvr: libvuplus.la
	cp -f .libs/libvuplus.so XBMC_vuplus.pvr
	$(STRIP) -x XBMC_vuplus.pvr

$(LIB): XBMC_vuplus.pvr
	cp -f XBMC_vuplus.pvr $(LIB)

all: $(LIB)

clean-local:
	-rm -f $(LIB) XBMC_vuplus.pvr

install:

