
check_PROGRAMS = v0
TESTS= $(check_PROGRAMS)

LIBS = $(OCTETOS_CAVE_BASE_LIBS) $(CUNIT_LIBS) $(OCTETOS_CAVE_MARIA_LIBS) $(top_builddir)/src/liboctetos-cave.la
FLAGS = $(OCTETOS_CAVE_BASE_FLAGS) $(CUNIT_FLAGS)  -I$(top_builddir) -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

if DATABASE_ENGINE_MARIA
FLAGS += -DDATABASE_ENGINE_MARIA
endif
if OCTEOTOS_CAVE_ENABLE_TDD
FLAGS += -DOCTEOTOS_CAVE_ENABLE_TDD -DOCTEOTOS_CAVE_ENABLE_TDD_V0
endif
if DATABASE_ENGINE_MARIA
FLAGS += -DDATABASE_ENGINE_MARIA
endif

v0_LDFLAGS =
v0_LDADD = $(LIBS) 
v0_CXXFLAGS = $(FLAGS) -I$(top_builddir)  -I$(top_srcdir)
v0_SOURCES = main.cc v0.cc
