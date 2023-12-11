

#ifndef OCTETOS_CAVE_V0_SCHEMA_HH
#define OCTETOS_CAVE_V0_SCHEMA_HH

#include "base.hh"

namespace oct::cave::v0
{
	struct Table
	{
		std::string name;

		Table() = default;
		Table(const char** s)
		{
			name = s[0];
		}
	};
	struct Schema
	{
		std::string name;
		std::vector<Table> tables;


		Schema() = default;
		Schema(const char** s)
		{
			name = s[0];
		}
	};

	template<typename C>
	class Builder
	{
	public:
		Builder(C& connection,const std::string& name = "")
		{
			load(connection);
		}

		const std::vector<Schema>& get_schemas() const
		{
			return schemas;
		}
	private:
		void load(C& connection)
		{
			if (not connection.is_connected()) throw ExceptionDriver("Se deve de realizar la conection antesde usar eon objeto");

			typename C::result_type result;
			try
			{
				result = connection.execute("SELECT SCHEMA_NAME from INFORMATION_SCHEMA.SCHEMATA;");
			}
			catch (const ExceptionDriver&)
			{
			}
			catch (const std::exception&)
			{
			}
			catch (...)
			{

			}
			if (not result.is_stored()) throw ExceptionDriver("Fallo la solicitud de informacion.");
			result.store(schemas);

			typename C::result_type result_tables;
			for (Schema& s : schemas)
			{
				try
				{
					//std::cout << "Reading schema : " << s.name << "\n";
					result_tables = connection.execute("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA ='" + s.name + "';");
				}
				catch (const ExceptionDriver&)
				{
				}
				catch (const std::exception&)
				{
				}
				catch (...)
				{

				}
				result_tables.store(s.tables);
				result_tables.close();
			}

			/*for (const Schema& s : schemas)
			{
				std::cout << "Reading schema : " << s.name << "\n";
				for (const Table& t : s.tables)
				{
					std::cout << "\t" << t.name << "\n";
				}
			}*/
		}

	private:
		std::vector<Schema> schemas;
	};
}

#endif
