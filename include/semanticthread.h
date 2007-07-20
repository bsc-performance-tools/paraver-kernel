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

    static bool getInitFromBegin() const
    {
      return initFromBegin;
    }
    static bool validRecord( const MemoryTrace::iterator& record );
    TSemanticValue execute( const MemoryTrace::iterator& record ) = 0;
  protected:
    static const TRecordType  validateMask;   // Valid records for this function
    static const bool         initFromBegin;  /* Must initialize from the
                                                    beginning of the trace */

  private:
};


#endif // SEMANTICTHREAD_H_INCLUDED
