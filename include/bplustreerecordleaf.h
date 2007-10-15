#ifndef _RECORDLEAF_H
#define _RECORDLEAF_H

/******************************************************************************
 * RecordLeaf
 *
 * < *TRecord, order > --> key = [time-type-order]
 *
 * The key of a record is composed of its time, its type and global order.
 * Order is needed because events should be ordered in the same time.
 * The last key is the key of the record that came last.
 ******************************************************************************/
#include <string>
#include <iostream>

#include "bplustreetypes.h"

namespace bplustree
{
  class RecordLeaf
  {
    public:
      typedef UINT32 TRecordOrder;

    private:
      static TRecordOrder global_order; // Numbers different records.
      TRecord *rec;                     // Needed to get  time-type.
      TRecordOrder ord;                 // Needed to save order.

    public:
      RecordLeaf( TRecord *r );
      RecordLeaf( TRecord *r, TRecordOrder order );
      RecordLeaf();
      ~RecordLeaf();

      // Set & Get Methods
      inline void setRecord( TRecord *rec )
      {
        this->rec  = rec;
        ord = global_order++;
      }

      inline TRecord  *getRecord( void ) const
      {
        return rec;
      }

      inline void setOrder( TRecordOrder o )
      {
        ord = o;
      }

      inline TRecordOrder getOrder( void ) const
      {
        return ord;
      }

      inline void setRecordDirect( TRecord *r )
      {
        rec = r;
      }

      // Assign and Compare Methods
      RecordLeaf& operator=( const RecordLeaf &rl ); // Doesn't make copy!
      bool operator<( const RecordLeaf &rl );
      bool operator>( const RecordLeaf &rl );
      bool operator==( const RecordLeaf &rl );
      bool operator!=( const RecordLeaf &rl );
      bool operator>=( const RecordLeaf &rl );
      bool operator<=( const RecordLeaf &rl );

      friend std::ostream& operator<<( std::ostream& os, const RecordLeaf& rl );
  };

  //std::ostream& operator<<( std::ostream& os, const RecordLeaf& rl );
}

#endif
