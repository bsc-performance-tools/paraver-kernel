#include "index.h"

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

TRecord *Index::findRecord( TRecordTime time )
{
  TTraceIndex::iterator it = baseIndex.find( time );

  return it->second;
}

