#include "memorytrace.h"

MemoryTrace::iterator::iterator()
{}

MemoryTrace::iterator::~iterator()
{}

bool MemoryTrace::iterator::operator==( const MemoryTrace::iterator &it ) const
{
  return ( this->record == it.record );
}

bool MemoryTrace::iterator::operator!=( const MemoryTrace::iterator &it ) const
{
  return ( this->record != it.record );
}

bool MemoryTrace::iterator::isNull() const
{
  return ( record == NULL );
}
