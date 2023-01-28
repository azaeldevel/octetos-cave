
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
	/*std::cout << "rest_schema : \n";
	for (const Schema& n : vector_schema)
	{
		std::cout << "\tDatabase : " << n.name << "\n";
	}*/
	CU_ASSERT(rest_schema.number_rows() == vector_schema.size());

	//>>>
	driver::Result rest_schema1;
	CU_ASSERT(not rest_schema1.is_stored());
	try
	{
		rest_schema1 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema1.is_stored());

	std::vector<const char**> vector_schema1;
	rest_schema1.store(vector_schema1);
	/*std::cout << "vector_schema1 : \n";
	for (const char** n : vector_schema1)
	{
		std::cout << "\tDatabase : " << n[0] << "\n";
	}*/
	CU_ASSERT(rest_schema1.number_rows() == vector_schema1.size());


	//>>>
	driver::Result rest_schema2;
	CU_ASSERT(not rest_schema2.is_stored());
	try
	{
		rest_schema2 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema2.is_stored());

	std::vector<cave::Row<const char*>> vector_schema2;
	rest_schema2.store(vector_schema2);
	std::cout << "vector_schema2 : \n";
	/*for (const cave::Row<const char*>& n : vector_schema2)
	{
		std::cout << "Database : " << n[0] << "\n";
	}*/
	for (const cave::Row<const char*>& n : vector_schema2)
	{
		CU_ASSERT(n[0] != NULL);
	}
	for (const cave::Row<const char*>& n : vector_schema2)
	{
		CU_ASSERT(n[100] == NULL);
	}
	CU_ASSERT(rest_schema2.number_rows() == vector_schema2.size());

	//>>>
	driver::Result rest_schema3;
	CU_ASSERT(not rest_schema3.is_stored());
	try
	{
		rest_schema3 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA WHERE  SCHEMA_NAME = 'INFORMATION_SCHEMA';");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema3.is_stored());

	cave::Row<const char*> row1;
	row1 = rest_schema3.next();
	//std::cout << "row1 : " << row1[0] << "\n";
	CU_ASSERT(strcmp(row1[0],"information_schema") == 0);

	std::string rest_schema3_str;
	row1.store(rest_schema3_str,(size_t)0);
	//std::cout << "rest_schema3_str : " << rest_schema3_str << "\n";
	CU_ASSERT(rest_schema3_str.compare("information_schema") == 0);

	char rest_schema3_char;
	row1.store(rest_schema3_char, (size_t)0);
	//std::cout << "rest_schema3_char : " << rest_schema3_char << "\n";
	CU_ASSERT(rest_schema3_char = 'i');

	const char* rest_schema3_cstr;
	row1.store(rest_schema3_cstr, (size_t)0);
	//std::cout << "rest_schema3_cstr : " << rest_schema3_cstr << "\n";
	CU_ASSERT(strcmp(rest_schema3_cstr, "information_schema") == 0);

	//>>>
	driver::Result rest_schema4;
	CU_ASSERT(not rest_schema4.is_stored());
	try
	{
		rest_schema4 = connection_schema.execute("SELECT pwdtxt from `muposys-dev`.user;");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema4.is_stored());

	cave::Row<const char*> row2;
	row2 = rest_schema4.next();
	//std::cout << "row2 : " << row2[0] << "\n";
	//CU_ASSERT(strcmp(row1[0], "information_schema") == 0);
}
