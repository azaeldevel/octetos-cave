

#ifndef OCTETOS_CAVE_SCHEMA_HH
#define OCTETOS_CAVE_SCHEMA_HH

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
		Builder(C& connection)
		{
			load(connection);
		}

		const std::vector<Schema>& get_schema() const
		{
			return schemas;
		}
	private:
		void load(C& connection);

	private:
		std::vector<Schema> schemas;
	};
}

#endif
 