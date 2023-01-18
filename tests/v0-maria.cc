


#include "v0.hh"

#include <string>
#include <iostream>
#include <chrono>

#include <stdio.h>

//>>>octetos::core
#include <math.h>

#include <src/maria.hh>

namespace v0 = oct::cave::v0;


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


struct MesureExecution
{
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::high_resolution_clock::duration duration;
	double media;	
};

namespace oct::core
{
	
	template<typename Data> Data media(size_t length,const Data* n)
	{
		//std::cout << "media<T>(...)\n";
		Data v = Data(0);
		for(size_t i = 0; i < length; i++)
		{
			v += n[i];
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		Data m = v / Data(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}
	template<class Container,class Data> double media(size_t length,const Container* data,const Data Container::* member)
	{
		std::cout << "media<T>(...)\n";
		double v = double(0);
		for(size_t i = 0; i < length; i++)
		{
			v += data[i].*member;
		}
		std::cout << "media<T>(...)  sigma : " << v << "\n";
		double m = v / double(length);
		std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}	
	//https://www.scs.stanford.edu/~dm/blog/param-pack.html,https://iamsorush.com/posts/cpp-variadic-template/
	/*template<typename T,typename ...Args> T media(Args... args)
	{
		//std::cout << "media<T>(...)\n";
		T sigma =  (args+...);		
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		T m = sigma / sizeof...(args);
		//std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}*/

	template<typename Data> Data desv(size_t length,const Data* numbers)
	{
		//std::cout << "media<T>(...)\n";
		Data m = media<Data>(length,numbers);
		Data v = Data(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(numbers[i] - m,2.0);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / Data(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}
	template<typename Container,typename Data> Data desv(size_t length,const Container* data,const Data Container::* member)
	{
		//std::cout << "media<T>(...)\n";
		Data m = media<Container>(length,data,member);
		Data v = Data(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(data[i].*member - m,2.0);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / Data(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}
	/*template<typename T,typename ...Args> T desv(Args... args)
	{
		//std::cout << "desv<T>(...)\n";	
		T m = media<T>(args...);
		//std::cout << "media : " << m << "\n";
		
		T sigma = (pow(args- m,2.0)+...);	
		//std::cout << "sigma : " << sigma << "\n";
		T d = sigma / sizeof...(args);
		d = sqrt(d);
		
		return d;
	}*/
	/*template<typename Container,typename Data = Container> Data desv(size_t length,const Container* numbers)
	{
		//std::cout << "media<T>(...)\n";
		Container m = media<Container>(length,numbers);
		Container v = Container(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(numbers[i] - m,2.0);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / Container(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}*/
	/*template<> double desv(size_t length,const MesureExecution* numbers)
	{
		//std::cout << "media<T>(...)\n";
		double m = media<MesureExecution,double>(length,numbers);
		double v = Container(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(numbers[i].media - m,2.0);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / double(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}*/
}

void v0_develop()
{
	std::cout << "\n";
	
}

template<typename T = std::chrono::milliseconds>
void mesure_query(short base_length,MesureExecution& mesure,v0::Connection<v0::DataMaria>& conn,const char* str)
{
	mesure.start = std::chrono::high_resolution_clock::now();
	v0::Result<v0::DataMaria> rest1[base_length];
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
	v0::DataMaria dtm("localhost","muposys","123456","INFORMATION_SCHEMA");
	v0::Connection<v0::DataMaria> conn(dtm,true);
	CU_ASSERT(conn.is_connected());

	constexpr size_t base_length = 1000;
	constexpr size_t base_test = 3;
	
	MesureExecution mesures[base_test];
	for(size_t i = 0; i < base_test; i++)
	{
		mesure_query(base_length,mesures[i],conn,"show databases;");	
	}
	
	double media = oct::core::media<MesureExecution,double>(base_test,(MesureExecution*)mesures,&MesureExecution::media);
	double fact = oct::core::desv<MesureExecution,double>(base_test,(MesureExecution*)mesures,&MesureExecution::media);
	std::cout << "Factor de ejecucion : (" <<  media << ","<< fact << ")ms\n";
}

void v0_conection()
{
	//std::cout << "Testing cave component..\n";

	v0::DataMaria dtm("localhost","muposys","123456","muposys-0-alpha");
		
	v0::Connection<v0::DataMaria> conn(dtm,true);
	CU_ASSERT(conn.is_connected());
	CU_ASSERT(conn.ping());
	
	v0::Result<v0::DataMaria> rest;
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
		v0::Result<v0::DataMaria> rest2  = conn.select("number","CatalogItem");
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