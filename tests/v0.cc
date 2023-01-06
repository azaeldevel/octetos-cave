


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

	oct::cave::DataMaria dtm("localhost","muposys","123456");
	std::cout << "Testing : " << dtm.get_label() << "..\n";
	CU_ASSERT(strcmp(dtm.get_name(),"maria") == 0);

	oct::cave::Connection<oct::cave::DataMaria> conn(dtm);
	CU_ASSERT(conn.is_connected());
	
	
}
