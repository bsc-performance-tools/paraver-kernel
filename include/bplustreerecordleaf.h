#ifndef _RECORDLEAF_H
#define _RECORDLEAF_H

#include <string>
#include <iostream>


/****************************************************************************
 * RecordLeaf
 *
 * <*TRecord, order> --> <time-type-order>
 *
 * The key of a record is composed of its time, its type and global order.
 * Order is needed because events should be ordered in the same time, where
 * the last key is the key of the event that came last.
 ****************************************************************************/
class RecordLeaf
{
  private:
    static TRecordOrder global_order = 0;
    TRecordOrder order;
    TRecord *rec;

  public:
    typedef UINT32 TRecordOrder;

    RecordLeaf( TRecord *r );
    RecordLeaf( TRecord *r, TRecordOrder order );
    RecordLeaf();
    ~RecordLeaf();

    void setRecord( TRecord *rec );
    inline TRecordOrder getOrder( void )  { return ord; };
    inline void getOrder( TRecordOrder o ) { ord = o; };
    inline TRecord  *getRecord( void )  { return rec; };
    inline void setRecordDirect( TRecord *r ) { rec = r; };

    RecordLeaf& operator=(const RecordLeaf &rl); // Doesn't make copy!
    bool operator<(const RecordLeaf &rl);
    bool operator>(const RecordLeaf &rl);
    bool operator==(const RecordLeaf &rl);
    bool operator!=(const RecordLeaf &rl);
    bool operator>=(const RecordLeaf &rl);
    bool operator<=(const RecordLeaf &rl);

    friend ostream& operator<<(ostream& os, const RecordLeaf& rl);
};

#endif
