
#include <string>
#include <iostream>
#include <stdio.h>
#include <cave/0/mmsql.hh>
#include <cave/0/oct-core.hh>
#include <cave/0/schema.hh>


#include "v0.hh"


namespace core = oct::core::v3;
namespace cave_current = oct::cave::v0;

/*#if defined OCTEOTOS_CAVE_TESTS_DRIVER_MMSQL
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
#endif*/


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
	cave_current::mmsql::Data dtm("localhost", "develop", "123456", "INFORMATION_SCHEMA", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	bool conectfl = false;
	cave_current::mmsql::Connection connection_schema;
	try
	{
		conectfl = connection_schema.connect(dtm, true);
	}
	catch (const cave_current::ExceptionDriver& e)
	{
		CU_ASSERT(false);
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return;
	}
	catch (const std::exception& e)
	{
		CU_ASSERT(false);
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return;
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(conectfl);

	cave_current::Result<char, cave_current::mmsql::Data> rest_schema;
	CU_ASSERT(not rest_schema.is_stored());
	try
	{
		rest_schema = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave_current::ExceptionDriver&)
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
	CU_ASSERT(rest_schema.size() == vector_schema.size());

	//>>>
	cave_current::Result<char, cave_current::mmsql::Data> rest_schema1;
	CU_ASSERT(not rest_schema1.is_stored());
	try
	{
		rest_schema1 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave_current::ExceptionDriver& e)
	{
		CU_ASSERT(false);
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return;
	}
	catch (const std::exception& e)
	{
		CU_ASSERT(false);
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return;
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
	CU_ASSERT(rest_schema1.size() == vector_schema1.size());


	//>>>
	cave_current::Result<char, cave_current::mmsql::Data> rest_schema2;
	CU_ASSERT(not rest_schema2.is_stored());
	try
	{
		rest_schema2 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA;");
	}
	catch (const cave_current::ExceptionDriver&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema2.is_stored());

	std::vector<cave_current::Row<char,cave_current::mmsql::Data>> vector_schema2;
	rest_schema2.store(vector_schema2);
	//std::cout << "vector_schema2 : \n";
	/*for (const cave::Row<const char*>& n : vector_schema2)
	{
		std::cout << "Database : " << n[0] << "\n";
	}*/
	for (const cave_current::Row<char, cave_current::mmsql::Data>& n : vector_schema2)
	{
		CU_ASSERT(n[0] != NULL);
	}
	for (const cave_current::Row<char, cave_current::mmsql::Data>& n : vector_schema2)
	{
		CU_ASSERT(n[100] == NULL);
	}
	CU_ASSERT(rest_schema2.size() == vector_schema2.size());

	//>>>
	cave_current::Result<char, cave_current::mmsql::Data> rest_schema3;
	CU_ASSERT(not rest_schema3.is_stored());
	try
	{
		rest_schema3 = connection_schema.execute("SELECT SCHEMA_NAME from SCHEMATA WHERE  SCHEMA_NAME = 'INFORMATION_SCHEMA';");
	}
	catch (const cave_current::ExceptionDriver&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest_schema3.is_stored());

	cave_current::Row<char, cave_current::mmsql::Data> row1;
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

	cave_current::Builder buider1(connection_schema);
	CU_ASSERT(buider1.get_schemas().size() > 0);
}
