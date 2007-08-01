#include "index.h"

Index::Index( UINT32 step )
{
  indexStep = step;
}

Index::~Index()
{}

void Index::indexRecord( TRecord *rec )
{
  baseIndex[ rec->time ] = rec;
}

TRecord *Index::findRecord( TRecordTime time )
{
  TTraceIndex::iterator it = baseIndex.find( time );

  return it->second;
}

