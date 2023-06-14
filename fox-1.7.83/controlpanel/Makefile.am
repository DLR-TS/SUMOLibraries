## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = @X_CFLAGS@
INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include
RESWRAP  = $(top_builddir)/utils/reswrap$(EXEEXT)
SUFFIXES = .gif .bmp
LDADD = $(top_builddir)/lib/libFOX-1.7.la -lm


bin_PROGRAMS = ControlPanel

man_MANS = ControlPanel.1

ICONS  = \
$(top_srcdir)/controlpanel/controlpanel_gif.gif \
$(top_srcdir)/controlpanel/colors_gif.gif \
$(top_srcdir)/controlpanel/filebinding_gif.gif \
$(top_srcdir)/controlpanel/settings_gif.gif

ControlPanel_SOURCES = \
icons.h \
icons.cpp \
ControlPanel.h \
ControlPanel.cpp

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
