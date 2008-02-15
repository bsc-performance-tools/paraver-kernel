#ifndef KRECORDLIST_H_INCLUDED
#define KRECORDLIST_H_INCLUDED

#include <set>
#include "recordlist.h"
#include "memorytrace.h"

using std::set;

class KRecordList: public RecordList
{
  public:
    KRecordList();
    virtual ~KRecordList();

    virtual void clear();
    virtual void erase( iterator first, iterator last );
    virtual iterator begin() const;
    virtual iterator end() const;
    virtual bool newRecords() const;

    // Specific for KRecordList
    virtual void insert( MemoryTrace::iterator *it );
  protected:

  private:
    // r1 is less than r2?
    struct ltrecord
    {
      bool operator()( const RLRecord& r1, const RLRecord& r2 )
      {
        if ( r1.getTime() < r2.getTime() )
          return true;
        else if ( r1.getTime() == r2.getTime() )
        {
          if ( r1.getOrder() < r2.getOrder() )
            return true;
          else if ( r1.getOrder() == r2.getOrder() )
          {
            if ( ( r1.getType() & COMM ) && ( r2.getType() & EVENT ) )
              return true;
          }
        }
        return false;
      }
    };

    set<RLRecord, ltrecord> list;
    bool newRec;
};


#endif // KRECORDLIST_H_INCLUDED
