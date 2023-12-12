


#include <string>
#include <iostream>
#include <stdio.h>
#include <codecvt>
#include <locale>
#include <locale>
#include <string>
#include <random>


#if defined LINUX_ARCH
	#include <mysql/mysql.h>
#elif defined LINUX_GENTOO
	#include <mariadb/mysql.h>
#elif defined LINUX_DEBIAN
	#include <mariadb/mysql.h>
#elif MSYS2
    #include <mariadb/mysql.h>
#elif (defined(_WIN32) || defined(_WIN64))
    #include <mysql/mysql.h>
#else
	#error "Plataforma desconocida."
#endif

#include "v1.hh"

#include <cave/1/mmsql.hh>

namespace core = oct::core::v3;
namespace cave = oct::cave::v1;



struct DB_name
{
	std::string name;

	DB_name() = default;
	DB_name(const char** s)
	{
		name = s[0];
	}
};


struct Table
{
	std::string name;

	Table() = default;
	Table(cave::Row<char,cave::mmsql::Data> s)
	{
		name = s[0];
	}

	static std::string fields()
	{
        return "SCHEMA_NAME";
	}

	static std::string table()
	{
        return "INFORMATION_SCHEMA.SCHEMATA";
	}
};

struct Version
{
    long id;
	std::string name;
	char major, minor,patch;

	Version() = default;
	Version(const char** s)
	{
		id = std::atol(s[0]);
		name = s[0];
	}
	Version(cave::Row<char,cave::mmsql::Data> s)
	{
		id = std::atol(s[0]);
		name = s[0];
	}
	Version& operator =(const char** s)
	{
		id = std::atol(s[0]);
		name = s[0];

		return *this;
	}

	static std::string select_fields()
	{
        return "id,name,major,minor";
	}
	static std::string insert_fields()
	{
        return "name,major,minor";
	}

	static std::string table()
	{
        return "Version";
	}

	/*template<class C> cave::mmsql::Result insert(C& connector)
	{
	    std::string sql;
        sql += "'" + name + "'," + std::to_string(major)  + "," + std::to_string(minor);
        //std::cout << "SQL 2 : " << sql << "\n";
        return connector.insert(fields_insert(),sql,table());
	}*/
};

void v1_develop()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> longint(1,92233720368547);
    std::uniform_int_distribution<std::mt19937::result_type> smallint(1,128);

    cave::mmsql::Data dtm("localhost","develop","123456", "muposys-dev", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	bool conectfl = false;
	cave::mmsql::Connection conn;
	try
	{
		conectfl = conn.connect(dtm, true);
	}
	catch (const cave::ExceptionDriver& e)
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


	std::string sqlversionInser = "INSERT INTO Version(name,major,minor) VALUES(";
	sqlversionInser += "'name" + std::to_string(longint(rng)) + "'," + std::to_string(smallint(rng))  + "," + std::to_string(smallint(rng)) + ")";
	//std::cout << sqlversionInser << "\n";
    cave::mmsql::Result rs1;
	try
	{
        rs1 = conn.execute(sqlversionInser);
	}
	catch(...)
	{
        std::cout << "Error desconocido en escritura de base de datos\n";
	}



}

void v1_selects()
{
    cave::mmsql::Data dtm("localhost","develop","123456", "muposys-dev", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	bool conectfl = false;
	cave::mmsql::Connection conn;
	try
	{
		conectfl = conn.connect(dtm, true);
	}
	catch (const cave::ExceptionDriver& e)
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

    cave::mmsql::Result rs1;
    try
    {
		 rs1 = conn.select("Version","*");
	}
	catch (const cave::ExceptionDriver&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	/*for(size_t i = 0; i < rs1.size(); i++)
    {
        std::cout << "id:" << rs1.next()[0] << "\n";
    }*/


    std::vector<Version> rs2;
    try
    {
		 conn.select(rs2);
	}
	catch (const cave::ExceptionDriver&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	/*for(const Version& v : rs2)
    {
        std::cout << "id:" << v.id << "\n";
    }*/

    cave::mmsql::Result rs3;
    try
    {
		 rs3 = conn.select(cave::names::Table<std::string>("Version"));
	}
	catch (const cave::ExceptionDriver&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	/*for(size_t i = 0; i < rs3.size(); i++)
    {
        std::cout << "id:" << rs3.next()[0] << "\n";
    }*/
}
