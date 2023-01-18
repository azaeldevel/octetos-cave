


#include "v0.hh"

#include <string>
#include <iostream>
#include <src/base.hh>

#include <src/oct-core.hh>


int v0_init(void)
{
	return 0;
}
int v0_clean(void)
{
	return 0;
}


void v0_core_core()
{
	std::cout << "\n";
	
	double media = oct::core::media<int,double>(1,2,3,4,5,6,7,8,9,0);
	//std::cout << "media2 : " <<  media2 << "\n";
	double desv = oct::core::desv<int,double>(1,2,3,4,5,6,7,8,9,0);
	//std::cout << "desv2 : " <<  desv2 << "\n";
	
	double media1 = oct::core::media<double>(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,0.0);
	//std::cout << "media1 : " <<  media1 << "\n";
	double desv1 = oct::core::desv<double>(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,0.0);
	//std::cout << "desv1 : " <<  desv1 << "\n";

	CU_ASSERT(media - media1 < 0.01);
	CU_ASSERT(desv - desv1 < 0.01);
}
