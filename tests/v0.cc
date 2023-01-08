


#include "v0.hh"

#include <string>
#include <iostream>
#include <src/base.hh>

int v0_init(void)
{
	return 0;
}
int v0_clean(void)
{
	return 0;
}

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
	//std::cout << "Testing cave component..\n";

	oct::cave::DataMaria dtm("localhost","muposys","123456","muposys-0-alpha");
		
	oct::cave::Connection<oct::cave::DataMaria> conn(dtm,true);
	CU_ASSERT(conn.is_connected());
	CU_ASSERT(conn.ping());
	
	oct::cave::Result<oct::cave::DataMaria> rest;
	CU_ASSERT(not rest.is_stored());
	rest = conn.execute("show databases;");
	CU_ASSERT(rest.is_stored());
	std::cout << "rest : " << (void*)rest << "\n";

	std::cout << "Cantidad : " << rest.number_rows() << "\n";

	std::vector<DB_name> vec_dbs;
	rest.store(vec_dbs);
	for(const DB_name& n : vec_dbs)
	{
		std::cout << "Database : " << n.name << "\n";
	}

	oct::cave::Result<oct::cave::DataMaria> rest2  = conn.select("number","CatalogItem");
	std::vector<CatalogItem_testv0> vec_items;
	rest2.store(vec_items);
	for(const CatalogItem_testv0& n : vec_items)
	{
		std::cout << "Item : " << n.number << "\n";
	}
}
