## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = @X_CFLAGS@
INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
RESWRAP  = $(top_builddir)/utils/reswrap$(EXEEXT)
SUFFIXES = .gif .bmp
LDADD = $(top_builddir)/lib/libFOX-1.7.la -lm


bin_PROGRAMS = calculator

man_MANS = calculator.1

ICONS  = \
$(top_srcdir)/calculator/colors.gif \
$(top_srcdir)/calculator/information.gif \
$(top_srcdir)/calculator/bigcalc.gif \
$(top_srcdir)/calculator/constmem.bmp \
$(top_srcdir)/calculator/question.gif \
$(top_srcdir)/calculator/tinycalc.gif


calculator_SOURCES = \
icons.h \
icons.cpp \
Calculator.h \
Calculator.cpp \
HelpWindow.h \
HelpWindow.cpp \
Preferences.h \
Preferences.cpp \
help.cpp \
main.cpp


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
