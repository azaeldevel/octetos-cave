
#include <string>
#include <iostream>
#include <stdio.h>
#if  (defined(_WIN32) || defined(_WIN64)) && COMPILER_VS
	#include <cave/src/mmsql.hh>
	#include <cave/src/oct-core.hh>
#elif defined __linux__
	#include <src/mmsql.hh>
	#include <src/oct-core.hh>
#else
#error "Plataforma desconocida."
#endif


#include "v0.hh"


namespace core = oct::core::v3;
#if defined OCTEOTOS_CAVE_TESTS_DRIVER_MMSQL
namespace cave = oct::cave::v0;
namespace driver = oct::cave::v0::mmsql;
#elif defined  OCTEOTOS_CAVE_TESTS_DRIVER_MARIA
namespace cave = oct::cave::v0;
namespace driver = oct::cave::v0::maria;
#elif defined  OCTEOTOS_CAVE_TESTS_DRIVER_MYSQL
namespace cave = oct::cave::v0;
namespace driver = oct::cave::v0::mysql;
#else
	#error "Driver Desconocido."
#endif


struct Schema
{
	std::string name;

	Schema() = default;
	Schema(const char** s)
	{
		name = s[0];
	}
};


void v0_schema()
{
	driver::Data dtm("localhost", "develop", "123456", "INFORMATION_SCHEMA", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	bool conectfl = false;
	driver::Connection connection_schema;
	try
	{
		conectfl = connection_schema.connect(dtm, true);
	}
	catch (const oct::core::v3::Exception& e)
	{
		CU_ASSERT(false);
		std::cout << "Exception (cave testing) : " << e.describe() << "\n";
		return;
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(conectfl);
	


	driver::Result rest_schema;
	CU_ASSERT(not rest_schema.is_stored());
	try
	{
		rest_schema = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema.is_stored());

	std::vector<Schema> vector_schema;
	rest_schema.store(vector_schema);
	/*for (const Schema& n : vector_schema)
	{
		std::cout << "Database : " << n.name << "\n";
	}*/
	CU_ASSERT(rest_schema.number_rows() == vector_schema.size());
}
