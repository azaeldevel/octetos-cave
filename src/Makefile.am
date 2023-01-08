
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

LIBS = $(OCTETOS_CAVE_BASE_LIBS) 
FLAGS = $(OCTETOS_CAVE_BASE_FLAGS) -I$(top_builddir) -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

if OCTEOTOS_CAVE_ENABLE_DEV
FLAGS += -DOCTEOTOS_CAVE_ENABLE_DEV
endif

lib_LTLIBRARIES = 

if DATABASE_ENGINE_MARIA
LIBS += $(OCTETOS_CAVE_MARIA_LIBS)
lib_LTLIBRARIES += liboctetos-cave-maria.la
liboctetos_cave_maria_la_CXXFLAGS = $(FLAGS)
liboctetos_cave_maria_la_SOURCES = base.cc maria.cc
liboctetos_cave_maria_la_LIBADD =  $(LIBS)  
endif


bin_PROGRAMS = devel

devel_LDADD = $(LIBS)
devel_CXXFLAGS = $(FLAGS)
devel_SOURCES = main.cc
devel_LDFLAGS = 