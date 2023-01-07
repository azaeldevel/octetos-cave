


#include "v0.hh"

#include <string>
#include <iostream>
#include <src/client.hh>

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

void v0_develop()
{
	std::cout << "Testing cave component..\n";

	oct::cave::DataMaria dtm("localhost","muposys","123456","muposys-0-alpha");
	std::cout << "Testing : " << dtm.get_label() << "..\n";
	CU_ASSERT(strcmp(dtm.get_name(),"maria") == 0);
	
	oct::cave::Connection<oct::cave::DataMaria> conn(dtm);
	CU_ASSERT(conn.is_connected());
	
	oct::cave::Result<oct::cave::DataMaria> rest;
	CU_ASSERT(not rest.is_stored());
	rest = conn.execute("show databases;");
	CU_ASSERT(rest.is_stored());
	//std::cout << "rest : " << (void*)rest << "\n";

	std::cout << "Cantidad : " << rest.number_rows() << "\n";

	std::vector<DB_name> vec_dbs;
	rest.store(vec_dbs);
	for(const DB_name& n : vec_dbs)
	{
		std::cout << "Database : " << n.name << "\n";
	}

	
	
}
