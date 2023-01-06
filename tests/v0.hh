
#ifndef OCTETOS_CAVE_TESTS_HH
#define OCTETOS_CAVE_TESTS_HH

#include <CUnit/Basic.h>

#if __linux__
    
#elif defined MSYS2
    
#else
	#error "Plataforma desconocida."
#endif

int v0_init(void);
int v0_clean(void);
void v0_develop();

#endif
