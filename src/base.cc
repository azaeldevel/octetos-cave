
#include "base.hh"

namespace oct::cave
{

	
Exception::Exception()
{
}
Exception::Exception(const Exception& e) : oct::core::v3::Exception(e)
{
}
Exception::Exception(Exception&& e) : oct::core::v3::Exception(e)
{
}


Exception::Exception(unsigned int c) : oct::core::v3::Exception(c)
{
}
Exception::Exception(unsigned int c,const char* s) : oct::core::v3::Exception(c,s)
{
}
Exception::Exception(unsigned int c,const char* fn, unsigned int l) : oct::core::v3::Exception(c,fn,l)
{
}
Exception::Exception(unsigned int c,const char* s,const char* fn, unsigned int l) : oct::core::v3::Exception(c,s,fn,l)
{
}

Exception::Exception(const std::string& m) : oct::core::v3::Exception(m)
{
}
Exception::Exception(const std::string& m,const char* f, unsigned int l) : oct::core::v3::Exception(m,f,l)
{
}


Exception::Exception(unsigned int c,const std::string& m) : oct::core::v3::Exception(c,m)
{
}
Exception::Exception(unsigned int c,const std::string& m,const char* f, unsigned int l) : oct::core::v3::Exception(c,m,f,l)
{
}

Exception::~Exception()
{
}

const char* Exception::what() const throw ()
{
	/*if(code() == DB_ERROR)
	{
		
	}*/
	switch(code())
	{
	case NoErros:
		return "No hay error presente";
	case NotYet:
		return "Esta funcion esta en desarrollo aún.";
	case IlegalOperation:
		return "Esta operacion no se permite para este objecto";
	case NotCopiable:
		return "Esta objeto no puede ser copiado";
	case NotCopiableResult:
		return "El obejto Result no puede ser copiado.";
	case NotVoidObject:
		return "El objeto no esta vacio.";
	case DB_ERROR:
		return "Error duarante la operaci con base de datos";
	case DB_ERROR_Result:
		return "Error duarante la operaci con base de datos, la ejecucion del query falló.";
	case DB_ERROR_Result_Unknow:
		return "Error duarante la operación con base de datos, la ejecucion del query falló, sin embargo, no hay error registrado";
	}
	return "Error desconocido.";
}





ExceptionMaria::ExceptionMaria()
{
}
ExceptionMaria::ExceptionMaria(const ExceptionMaria& e) : Exception(e)
{
}
ExceptionMaria::ExceptionMaria(ExceptionMaria&& e) : Exception(e)
{
}

ExceptionMaria::ExceptionMaria(unsigned int c,const char* s,const char* fn, unsigned int l) : Exception(c,fn,l), state(s)
{
	
}

ExceptionMaria::~ExceptionMaria()
{
}

const char* ExceptionMaria::what() const throw ()
{
	return "";
}

}