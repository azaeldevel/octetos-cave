
#include <CUnit/Basic.h>

#ifdef OCTEOTOS_CAVE_ENABLE_TDD
	#include "v0.hh"
#endif


int main(int argc, char *argv[])
{
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

#ifdef OCTEOTOS_CAVE_ENABLE_TDD_V0

	CU_pSuite pSuite_v0 = CU_add_suite("Octetos Cave Library", v0_init, v0_clean);
	if (NULL == pSuite_v0)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if (NULL == CU_add_test(pSuite_v0, "Developing", v0_develop))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite_v0, "Basic conection", v0_conection))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite_v0, "Medidas de ejecucion ", v0_mesures))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_pSuite pSuite_cotetos_core_v0 = CU_add_suite("Octetos Core Library", v0_init, v0_clean);
	if (NULL == pSuite_cotetos_core_v0)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if (NULL == CU_add_test(pSuite_v0, "Medidas de ejecucion ", v0_mesures))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

#endif

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
