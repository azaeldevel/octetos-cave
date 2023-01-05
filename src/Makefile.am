
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

bin_PROGRAMS = devel

devel_LDADD = $(OCTETOS_CAVE_BASE_LIBS)
devel_CXXFLAGS = $(OCTETOS_CAVE_BASE_FLAGS) -I$(top_builddir) -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
devel_SOURCES = main.cc
devel_LDFLAGS = 
