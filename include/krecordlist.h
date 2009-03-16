#ifndef KRECORDLIST_H_INCLUDED
#define KRECORDLIST_H_INCLUDED

#include <set>
#include "recordlist.h"
#include "memorytrace.h"

using std::set;

class KWindow;

class KRecordList: public RecordList
{
  public:
    KRecordList();
    virtual ~KRecordList();

    virtual void clear();
    virtual void erase( iterator first, iterator last );
    virtual iterator begin();
    virtual iterator end();
    virtual bool newRecords() const;

    // Specific for KRecordList
    virtual void insert( KWindow *window, MemoryTrace::iterator *it );
    virtual RecordList *clone();

  protected:

  private:
    set<RLRecord, ltrecord> list;
    bool newRec;
};


#endif // KRECORDLIST_H_INCLUDED
