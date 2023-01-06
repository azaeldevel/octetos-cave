


#include "v0.hh"

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
	std::cout << "rest : " << (void*)rest << "\n";
}
