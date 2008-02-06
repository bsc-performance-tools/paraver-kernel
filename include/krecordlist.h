#ifndef KRECORDLIST_H_INCLUDED
#define KRECORDLIST_H_INCLUDED

#include "recordlist.h"

class KRecordList: public RecordList
{
  public:
    KRecordList();
    virtual ~KRecordList();
  protected:

  private:
    // r1 is less than r2?
    struct ltrecord
    {
      bool operator()( RLRecord *r1, RLRecord *r2 ) const
      {
        if ( r1->time < r2->time )
          return true;
        else if ( r1->time == r2->time )
        {
          if ( r1->order < r2->order )
            return true;
          else if ( r1->order == r2->order )
          {
            if ( ( r1->type & COMM ) && ( r2->type & EVENT ) )
              return true;
          }
        }
        return false;
      }
    };

    set<RLRecord, ltrecord> list;

};


#endif // KRECORDLIST_H_INCLUDED
