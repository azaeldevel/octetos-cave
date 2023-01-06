
AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(GLIBMM_CFLAGS). \
	$(CUNIT_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DOCTETOS_CAVE_LOCALEDIR=\"${OCTETOS_CAVE_LOCALEDIR}\"
	

AM_CFLAGS =\
	 -Wall\
	 -g

LIBS = $(OCTETOS_CAVE_BASE_LIBS) $(OCTETOS_CAVE_MARIA_LIBS) 
FLAGS = $(OCTETOS_CAVE_BASE_FLAGS) -I$(top_builddir) -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include



lib_LTLIBRARIES = liboctetos-cave.la

liboctetos_cave_la_CXXFLAGS = $(FLAGS)
liboctetos_cave_la_SOURCES = client.cc
liboctetos_cave_la_LIBADD =  $(LIBS)


bin_PROGRAMS = devel

devel_LDADD = $(LIBS)
devel_CXXFLAGS = $(FLAGS)
devel_SOURCES = main.cc
devel_LDFLAGS = 