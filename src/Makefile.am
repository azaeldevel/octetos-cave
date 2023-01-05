
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

bin_PROGRAMS = cave

cave_SOURCES = main.cc
cave_LDFLAGS = 
cave_LDADD = $(OCTETOS_CAVE_BASE_LIBS)
cave_CXXFLAGS = $(OCTETOS_CAVE_BASE_FLAGS)
