#include "index.h"
#include <iostream>
Index::Index( UINT32 step )
{
  indexStep = step;
  counter = 0;
}

Index::~Index()
{}

void Index::indexRecord( TRecord *rec )
{
  counter++;
  if ( counter == indexStep )
  {
    baseIndex[ rec->time ] = rec;
    counter = 0;
  }
}

TRecord *Index::findRecord( TRecordTime time ) const
{
  TTraceIndex::const_iterator it = baseIndex.lower_bound( time );

  if ( it == baseIndex.end() )
    return NULL;

  return it->second;
}

