## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = @X_CFLAGS@
INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
RESWRAP  = $(top_builddir)/utils/reswrap$(EXEEXT)
SUFFIXES = .gif .bmp
LDADD = $(top_builddir)/lib/libFOX-1.7.la -lm


bin_PROGRAMS = shutterbug

man_MANS = shutterbug.1

ICONS  = \
$(top_srcdir)/shutterbug/line_0.gif \
$(top_srcdir)/shutterbug/line_1.gif \
$(top_srcdir)/shutterbug/line_2.gif \
$(top_srcdir)/shutterbug/line_3.gif \
$(top_srcdir)/shutterbug/line_4.gif \
$(top_srcdir)/shutterbug/line_5.gif \
$(top_srcdir)/shutterbug/line_6.gif \
$(top_srcdir)/shutterbug/line_7.gif \
$(top_srcdir)/shutterbug/line_8.gif \
$(top_srcdir)/shutterbug/shutterbug.gif \
$(top_srcdir)/shutterbug/tinyshutterbug.gif


shutterbug_SOURCES = \
icons.h \
icons.cpp \
ShutterBug.h \
ShutterBug.cpp \
Snapper.h \
Snapper.cpp


BUILT_SOURCES = icons.h icons.cpp


icons.h: $(ICONS)
	$(RESWRAP) -h -o $@ $^


icons.cpp: $(ICONS)
	$(RESWRAP) -i icons.h -o $@ $^


CLEANFILES = icons.h icons.cpp

EXTRA_DIST = \
$(man_MANS) \
LICENSE \
$(ICONS)
