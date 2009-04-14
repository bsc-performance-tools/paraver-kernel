#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

#include "paraverkerneltypes.h"
#include "bplustreetypes.h"

#include <map>

using std::map;
using namespace bplustree;

typedef map< TRecordTime, TRecord * > TTraceIndex;

static UINT32 INDEX_STEP = 10000;

class Index
{
  public:
    Index( UINT32 step = INDEX_STEP );
    ~Index();

    void indexRecord( TRecord *rec );
    TRecord *findRecord( TRecordTime time ) const;

  private:
    UINT32 indexStep;
    TTraceIndex baseIndex;
    UINT32 counter;
};

#endif // INDEX_H_INCLUDED
