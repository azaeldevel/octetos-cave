
#include "base.hh"

namespace oct::cave::v0
{




ExceptionResult::ExceptionResult()
{
}
ExceptionResult::ExceptionResult(const ExceptionResult& e) : core::v3::Exception(e)
{
}

ExceptionResult::ExceptionResult(const char* m) : core::v3::Exception(m)
{
}
ExceptionResult::ExceptionResult(const char* m,const char* fn, unsigned int l) : core::v3::Exception(m,fn,l)
{
}

ExceptionResult::~ExceptionResult()
{
}


ExceptionQuery::ExceptionQuery()
{
}
ExceptionQuery::ExceptionQuery(const ExceptionQuery& e) : core::v3::Exception(e)
{
}

ExceptionQuery::ExceptionQuery(const char* m) : core::v3::Exception(m)
{
}
ExceptionQuery::ExceptionQuery(const char* m,const char* fn, unsigned int l) : core::v3::Exception(m,fn,l)
{
}

ExceptionQuery::~ExceptionQuery()
{
}


}