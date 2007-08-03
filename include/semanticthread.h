#ifndef SEMANTICTHREAD_H_INCLUDED
#define SEMANTICTHREAD_H_INCLUDED

#include "semanticfunction.h"

class SemanticThread : public SemanticFunction
{
  public:
    SemanticThread()
    {}
    ~SemanticThread()
    {}

    static bool getInitFromBegin()
    {
      return initFromBegin;
    }
    static bool validRecord( const MemoryTrace::iterator *record )
    {
      TRecordType type = record->getType();
      TRecordType mask = validateMask;

      if ( type & EMPTYREC )
        return true;
      if ( type & END )
        type -= END;
      if ( mask & RSEND )
      {
        if ( type & RSEND )
          return true;
        else
          mask -= RSEND;
      }
      else if ( mask & RRECV )
      {
        if ( type & RRECV )
          return true;
        else
          mask -= RRECV;
      }
      return ( mask & type == mask );
    }

  protected:
    // Valid records for this function
    static const TRecordType  validateMask;
    // Must initialize from the beginning of the trace
    static const bool         initFromBegin;

  private:
};


#endif // SEMANTICTHREAD_H_INCLUDED
