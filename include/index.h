#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

#include "paraverkerneltypes.h"

#include <map>

using std::map;

static UINT32 INDEX_STEP = 10000;

template <typename RecordType>
class Index
{
  public:
    Index( UINT32 step = INDEX_STEP );
    ~Index();

    void indexRecord( TRecordTime time, RecordType rec );
    bool findRecord( TRecordTime time, RecordType& record ) const;

  private:
    typedef map< TRecordTime, RecordType > TTraceIndex;

    UINT32 indexStep;
    TTraceIndex baseIndex;
    UINT32 counter;
};

#include "src/index.cpp"

#endif // INDEX_H_INCLUDED
