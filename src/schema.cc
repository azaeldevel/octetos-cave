

#include <iostream>
#include "schema.hh"
#include "mmsql.hh"

namespace oct::cave::v0
{
	template<> void Builder<mmsql::Connection>::load(mmsql::Connection& connection)
	{
		if (not connection.is_connected()) throw ExceptionQuery("Se deve de realizar la conection antesde usar eon objeto", __FILE__, __LINE__);

		mmsql::Result result;
		try
		{
			result = connection.execute("SELECT SCHEMA_NAME from INFORMATION_SCHEMA.SCHEMATA;");
		}
		catch (const ExceptionQuery&)
		{
		}
		catch (const oct::core::v3::Exception&)
		{
		}
		catch (...)
		{

		}
		if (not result.is_stored()) throw ExceptionQuery("Fallo la solicitud de informacion.", __FILE__, __LINE__);
		result.store(schemas);

		mmsql::Result result_tables;
		for (Schema& s : schemas)
		{
			try
			{
				std::cout << "Reading schema : " << s.name << "\n";
				//result_tables = connection_schema.execute("SELECT SCHEMA_NAME from INFORMATION_SCHEMA.SCHEMATA;");
			}
			catch (const ExceptionQuery&)
			{
			}
			catch (const oct::core::v3::Exception&)
			{
			}
			catch (...)
			{
			}
		}
	}
}
 