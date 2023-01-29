
check_PROGRAMS = v0
TESTS= $(check_PROGRAMS)

LIBS = $(OCTETOS_CAVE_BASE_LIBS) $(CUNIT_LIBS) 
FLAGS = $(OCTETOS_CAVE_BASE_FLAGS) $(CUNIT_FLAGS)  -I$(top_builddir) -I/usr/include/glibmm-2.4 -I/usr/lib/glibmm-2.4/include -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

if DATABASE_ENGINE_MARIA
FLAGS += -DDATABASE_ENGINE_MARIA
LIBS += $(OCTETOS_CAVE_MARIA_LIBS) $(top_builddir)/src/liboctetos-cave-maria.la
endif
if OCTEOTOS_CAVE_ENABLE_TDD
FLAGS += -DOCTEOTOS_CAVE_ENABLE_TDD -DOCTEOTOS_CAVE_ENABLE_TDD_V0
endif
if DATABASE_ENGINE_MARIA
FLAGS += -DDATABASE_ENGINE_MARIA
endif
if OCTEOTOS_CAVE_ENABLE_DEV
FLAGS += -DOCTEOTOS_CAVE_ENABLE_DEV
endif
if DATABASE_ENGINE_MMSQL
FLAGS += -DOCTEOTOS_CAVE_TESTS_DRIVER_MMSQL -DOCTEOTOS_CAVE_TESTS_MMSQL_PORT=3306
LIBS += $(OCTETOS_CAVE_MMSQL_LIBS) $(top_builddir)/src/liboctetos-cave-mmsql.la
endif

v0_LDFLAGS =
v0_LDADD = $(LIBS) 
v0_CXXFLAGS = $(FLAGS) -I$(top_builddir)  -I$(top_srcdir)
v0_SOURCES = main.cc v0.cc v0-mmsql.cc v0-schema.cc
