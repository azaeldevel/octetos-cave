


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


struct DB_name
{
	std::string name;

	DB_name() = default;
	DB_name(const char** s)
	{
		name = s[0];
	}
};


void v0_develop()
{
	std::cout << "\n";
	
}

template<typename T = std::chrono::milliseconds>
void mesure_query(size_t base_length,oct::core::MesureExecution& mesure, driver::Connection& conn,const char* str)
{
	mesure.start = std::chrono::high_resolution_clock::now();
	driver::Result rest1[base_length];//TODO: usar base_length
	for(size_t i = 0; i < base_length; i++)
	{
		rest1[i] = conn.execute(str);
	}
	mesure.end = std::chrono::high_resolution_clock::now();
	mesure.duration = duration_cast<T>(mesure.end - mesure.start);
  	//std::cout << "Ejecucion : took " << time_1/std::chrono::milliseconds(1) << "ns to run.\n";
	mesure.media = double(mesure.duration.count()) / double(base_length);
	
}
void v0_mesures()
{
	std::cout << "\n";
	driver::Data dtm("localhost","develop","123456","INFORMATION_SCHEMA", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	driver::Connection conn;
	bool conectfl = false;
	try
	{
		conectfl = conn.connect(dtm, true);
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
	CU_ASSERT(conn.is_connected());
	CU_ASSERT(conn.ping());

	const size_t base_length = 1000;
	const size_t base_test = 10;
	
	oct::core::MesureExecution mesures[base_test];
	/*for (size_t i = 0; i < base_test; i++)
	{
		mesure_query(base_length,mesures[i],conn,"show databases;");	
	}*/
	
	/*double media = oct::core::media(base_test, (oct::core::MesureExecution*)mesures, &oct::core::MesureExecution::media);
	double desv = oct::core::desv(base_test,(oct::core::MesureExecution*)mesures,&oct::core::MesureExecution::media);
	double time = desv/1000.0;
	std::cout << "Factor de ejecucion : (" <<  media << ","<< desv << ")ms - "<< time << "s\n";*/
	//if(media > 0.0) CU_ASSERT(150000 - media < 50000);
	//CU_ASSERT(time < 5.0);
}

void v0_conection()
{
	//std::cout << "Testing cave component..\n";

	driver::Data dtm("localhost","develop","123456", "INFORMATION_SCHEMA", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	bool conectfl = false;
	driver::Connection conn;
	try
	{
		conectfl = conn.connect(dtm, true);
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

	CU_ASSERT(conn.is_connected());
	CU_ASSERT(conn.ping());
	
	driver::Result rest;
	CU_ASSERT(not rest.is_stored());
	try
	{
		rest = conn.execute("show databases;");
	}
	catch(const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch(...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest.is_stored());

	//std::cout << "Cantidad : " << rest.number_rows() << "\n";

	std::vector<DB_name> vec_dbs;
	rest.store(vec_dbs);
	/*for (const DB_name& n : vec_dbs)
	{
		std::cout << "Database : " << n.name << "\n";
	}*/
	CU_ASSERT(rest.number_rows() == vec_dbs.size());

	driver::Result rest2;
	CU_ASSERT(not rest2.is_stored());
	try
	{
		rest2 = conn.execute("show databases;");
	}
	catch (const cave::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch (...)
	{
		CU_ASSERT(false);
	}
	CU_ASSERT(rest2.is_stored());
	std::list<DB_name> lst_dbs;
	rest2.store(lst_dbs);
	/*for (const DB_name& n : lst_dbs)
	{
		std::cout << "Database : " << n.name << "\n";
	}*/
	CU_ASSERT(rest2.number_rows() == lst_dbs.size());
}