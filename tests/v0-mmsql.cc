


#include "v0.hh"
#include <string>
#include <iostream>
#include <stdio.h>
#if  (defined(_WIN32) || defined(_WIN64)) && COMPILER_VS
	#include <cave/src/mmsql.hh>
	#include <cave/src/oct-core.hh>
#elif defined __linux__
	#include <src/maria.hh>
	#include <src/oct-core.hh>
#else
#error "Plataforma desconocida."
#endif


namespace v0 = oct::cave::v0;
namespace v0_sql = oct::cave::v0::mmsql;


struct DB_name
{
	std::string name;

	char** operator =(char** s)
	{
		name = s[0];

		return s;
	}
};

struct CatalogItem_testv0
{
	std::string number;

	char** operator =(char** s)
	{
		number = s[0];

		return s;
	}
};




void v0_develop()
{
	std::cout << "\n";
	
}

template<typename T = std::chrono::milliseconds>
void mesure_query(size_t base_length,oct::core::MesureExecution& mesure, v0::mmsql::Connection& conn,const char* str)
{
	mesure.start = std::chrono::high_resolution_clock::now();
	v0::mmsql::Result rest1[base_length];//TODO: usar base_length
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
	v0::mmsql::Data dtm("localhost","muposys","123456","INFORMATION_SCHEMA", OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
	v0::Connection<v0::mmsql::Data> conn(dtm,true);
	CU_ASSERT(conn.is_connected());

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

	v0::mmsql::Data dtm("localhost","muposys","123456", OCTEOTOS_CAVE_TESTS_MMSQL_DB, OCTEOTOS_CAVE_TESTS_MMSQL_PORT);
		
	v0::mmsql::Connection conn(dtm,true);
	CU_ASSERT(conn.is_connected());
	CU_ASSERT(conn.ping());
	
	v0::mmsql::Result rest;
	CU_ASSERT(not rest.is_stored());
	try
	{
		rest = conn.execute("show databases;");
	}
	catch(const v0::ExceptionQuery&)
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
	/*for(const DB_name& n : vec_dbs)
	{
		std::cout << "Database : " << n.name << "\n";
	}*/
		
	try
	{
		v0::mmsql::Result rest2  = conn.select("number","CatalogItem");
		std::vector<CatalogItem_testv0> vec_items;
		rest2.store(vec_items);
		/*for(const CatalogItem_testv0& n : vec_items)
		{
			std::cout << "Item : " << n.number << "\n";
		}*/
	}
	catch(const v0::ExceptionQuery&)
	{
		CU_ASSERT(false);
	}
	catch(...)
	{
		CU_ASSERT(false);
	}
}