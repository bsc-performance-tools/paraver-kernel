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

    virtual bool validRecord( MemoryTrace::iterator *record )
    {
      TRecordType type = record->getType();
      TRecordType mask = getValidateMask();

      if ( type == EMPTYREC )
        return true;
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
      return ( ( mask & type ) == mask );
    }

  protected:
    // Valid records for this function
    virtual const TRecordType getValidateMask() = 0;

  private:
};


#endif // SEMANTICTHREAD_H_INCLUDED
