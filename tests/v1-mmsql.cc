


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
		name = s[1];
	}
	Version(cave::Row<char,cave::mmsql::Data> s)
	{
		id = std::atol(s[0]);
		name = s[1];
	}
	Version& operator =(const char** s)
	{
		id = std::atol(s[0]);
		name = s[1];

		return *this;
	}

	std::string insert_values()const
	{
	    std::string vals;
	    vals += "'" + name + "'";
	    vals += "," + std::to_string(major);
	    vals += "," + std::to_string(minor);

        return vals;
	}

	std::string update_values()const
	{
	    std::string vals;
	    vals += "name = '" + name + "'";
	    vals += ",major = " + std::to_string(major);
	    vals += ",minor = " + std::to_string(minor);

        return vals;
	}

	std::string update_values(const std::initializer_list<size_t>& list)const
	{
	    std::vector<std::string> vals(3);
	    vals[0] = "name = '" + name + "'";
	    vals[1] = "major = " + std::to_string(major);
	    vals[2] = "minor = " + std::to_string(minor);

        std::string str;
        str = vals[std::data(list)[0]];
        for(size_t i = 1; i < list.size(); i++)
        {
            str += "," + vals[std::data(list)[i]];
        }

        return str;
	}

	static std::string select_fields()
	{
        return "id,name,major,minor";
	}
	static std::string select_fields(const std::initializer_list<size_t>& list)
	{
	    std::vector<std::string> vals(3);
	    vals[0] = "name";
	    vals[1] = "major ";
	    vals[2] = "minor";

        std::string str;
        str = vals[std::data(list)[0]];
        for(size_t i = 1; i < list.size(); i++)
        {
            str += "," + vals[std::data(list)[i]];
        }

        return str;
	}

	static std::string insert_fields()
	{
        return "name,major,minor";
	}

	static std::string table()
	{
        return "Version";
	}


	static std::string identifier_name()
	{
        return "id";
	}
	std::string identifier_value() const
	{
        return std::to_string(id);
	}
};

void v1_develop()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> longint(1,92233720368547);
    std::uniform_int_distribution<std::mt19937::result_type> smallint(1,1000);
    std::string database_name = "cave-";
    database_name += std::to_string(smallint(rng));
    database_name += "-dev";

	bool conectflroot = false;
	cave::mmsql::Connection connroot;
	try
	{
		conectflroot = connroot.connect(dt_root, true);
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
	CU_ASSERT(conectflroot);

	cave::Database stmt(cave::Database::basic_header,database_name.c_str(),"develop","123456");
    //cave::print(std::cout,stmt);
    //stmt.print(std::cout);

	try
    {
        stmt.execute(connroot,true);
    }
    catch (const cave::ExceptionDriver& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (const cave::core::exception& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (...)
    {
        CU_ASSERT(false);
    }

    try
    {
        cave::Script base("/home/azael/develop/octetos/cave/tests/package/base.sql");
        base.execute(connroot,true);
        cave::Script data("/home/azael/develop/octetos/cave/tests/package/default-data-1.sql");
        data.execute(connroot,true);
    }
    catch (const cave::ExceptionDriver& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (const cave::core::exception& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (...)
    {
        CU_ASSERT(false);
    }

    std::string dropsql = "DROP DATABASE `";
    dropsql += database_name;
    dropsql += "`;";
    connroot.execute(dropsql,true);

    database_name = "cave-";
    database_name += std::to_string(smallint(rng));
    database_name += "-dev";

    /*cave::mmsql::Data dtm("localhost","develop","123456", database_name.c_str(), OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
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

    std::vector<std::filesystem::path> files;
    files.push_back("/home/azael/develop/octetos/cave/tests/package/base.sql");
    files.push_back("/home/azael/develop/octetos/cave/tests/package/defaul-data-1.sql");
    try
    {
        cave::mmsql::execute(conn,files,true);
    }
    catch (const cave::ExceptionDriver& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        CU_ASSERT(false);
    }
    catch (...)
    {
        CU_ASSERT(false);
    }*/
}

void v1_selects()
{
    cave::mmsql::Data dtm("localhost","develop","123456", "cave-dev", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
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
void v1_writes()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> longint(1,92233720368547);
    std::uniform_int_distribution<std::mt19937::result_type> smallint(1,128);

    cave::mmsql::Data dtm("localhost","develop","123456", "cave-dev", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
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

    Version ver1;
    ver1.name = "name" + std::to_string(longint(rng));
    ver1.major = smallint(rng);
    ver1.minor = smallint(rng);
    cave::mmsql::Result rs2 = conn.insert(ver1);

    std::vector<Version> rs3;
    try
    {
		 conn.select(rs3,"RAND()",3);
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
        std::cout << "id:" << rs3[i].id << "\n";
    }*/

    for(size_t i = 0; i < rs3.size(); i++)
    {
        rs3[i].major = smallint(rng);
        try
        {
             conn.update(rs3[i],{1});
        }
        catch (const cave::ExceptionDriver&)
        {
            CU_ASSERT(false);
        }
        catch (...)
        {
            CU_ASSERT(false);
        }
    }

}
