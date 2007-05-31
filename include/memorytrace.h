#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED

#include "paraverkerneltypes.h"

class MemoryTrace
{
  public:
    class  iterator
    {
      public:
        iterator();
        virtual ~iterator() = 0;

        virtual void operator++() = 0;
        virtual void operator--() = 0;

        virtual TRecordType getType() = 0;
        virtual TRecordTime getTime() = 0;
      protected:

      private:

    };


    MemoryTrace();
    ~MemoryTrace();
  protected:
   // typedef struct TRecord;

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
