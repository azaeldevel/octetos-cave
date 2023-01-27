
#ifndef OCTETOS_CORE_CAVE_HH
#define OCTETOS_CORE_CAVE_HH

/*
 * Copyright (C) 2022 Azael R. <azael.devel@gmail.com>
 *
 * octetos-cave is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * octetos-cave is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <concepts>
#include <chrono>

namespace oct::core
{
	template < class T >
	concept Number = std::is_same_v<T, float> or std::is_same_v<T, double> or std::is_same_v<T, int> or std::is_same_v<T, unsigned int> or std::is_same_v<T, long> or std::is_same_v<T, unsigned long>;
	

	//>>>Media
	template<Number Data> Data media(size_t length,const Data* n)
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
	template<class Container,Number Data> double media(size_t length,const Container* data,const Data Container::* member)
	{
		//std::cout << "media<T>(...)\n";
		double v = double(0);
		for(size_t i = 0; i < length; i++)
		{
			v += data[i].*member;
		}
		//std::cout << "media<T>(...)  sigma : " << v << "\n";
		double m = v / double(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}	
	//https://www.scs.stanford.edu/~dm/blog/param-pack.html,https://iamsorush.com/posts/cpp-variadic-template/
	template<Number T,typename ...Args> T media(Args... args)
	{
		//std::cout << "media<T>(...)\n";
		T sigma =  (args+...);		
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		T m = sigma / T(sizeof...(args));
		//std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}
	template<Number T,Number R,typename ...Args> R media(Args... args)
	{
		//std::cout << "media<T>(...)\n";
		T sigma =  (args+...);		
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		R m = R(sigma) / R(sizeof...(args));
		//std::cout << "media<T>(...)  m : " << m << "\n";
    	return m;
	}


	//desviacion estandar
	template<Number Data> Data desv(size_t length,const Data* numbers)
	{
		//std::cout << "media<T>(...)\n";
		Data m = media<Data>(length,numbers);
		Data v = Data(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(numbers[i] - m,2);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / Data(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}
	template<typename Container,Number Data> Data desv(size_t length,const Container* data,const Data Container::* member)
	{
		//std::cout << "media<T>(...)\n";
		Data m = media<Container>(length,data,member);
		Data v = Data(0);
		for(size_t i = 0; i < length; i++)
		{
			v += pow(data[i].*member - m,2);
		}
		//std::cout << "media<T>(...)  sigma : " << sigma << "\n";
		v = v / Data(length);
		//std::cout << "media<T>(...)  m : " << m << "\n";
		v = sqrt(v);
    	return v;
	}
	template<Number T,typename ...Args> T desv(Args... args)
	{
		//std::cout << "desv<T>(...)\n";	
		T m = media<T>(args...);
		//std::cout << "media : " << m << "\n";
		
		T sigma = (pow(args- m,T(2))+...);	
		//std::cout << "sigma : " << sigma << "\n";
		T d = sigma / sizeof...(args);
		d = sqrt(d);
		
		return d;
	}
	template<Number T,Number R,typename ...Args> R desv(Args... args)
	{
		//std::cout << "desv<T>(...)\n";	
		R m = media<T,R>(args...);
		//std::cout << "media : " << m << "\n";
		
		R sigma = (pow(args- m,R(2))+...);	
		//std::cout << "sigma : " << sigma << "\n";
		R d = sigma / sizeof...(args);
		d = sqrt(d);
		
		return d;
	}
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

	struct MesureExecution
	{
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point end;
		std::chrono::high_resolution_clock::duration duration;
		double media;	
	};
	//https://en.cppreference.com/w/cpp/string/byte/atoi
	template<std::signed_integral T, typename C> T atoi(const C* str)
	{
		while (std::isspace(static_cast<unsigned char>(*str)))
		{
			++str;
		}

		bool negative = false;

		if (*str == '+')
		{
			++str;
		}
		else if (*str == '-')
		{
			++str;
			negative = true;
		}

		T result = 0;
		for (; std::isdigit(static_cast<unsigned char>(*str)); ++str)
		{
			T digit = *str - '0';
			result *= 10;
			result -= digit; // calculate in negatives to support INT_MIN, LONG_MIN,..
		}
		
		return negative ? result : -result;
	}
	template<std::unsigned_integral T,typename C> T atoi(const C* str)
	{
		while (std::isspace(static_cast<unsigned char>(*str)))
		{
			++str;
		}

		bool negative = false;

		if (*str == '+')
		{
			++str;
		}
		else if (*str == '-')
		{
			++str;
			negative = true;
			//throw error
		}

		T result = 0;
		for (; std::isdigit(static_cast<unsigned char>(*str)); ++str)
		{
			T digit = *str - '0';
			result *= 10;
			result -= digit; // calculate in negatives to support INT_MIN, LONG_MIN,..
		}

		return result;
	}
}


#endif