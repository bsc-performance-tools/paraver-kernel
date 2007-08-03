#ifndef SEMANTICTHREADFUNCTIONS_H_INCLUDED
#define SEMANTICTHREADFUNCTIONS_H_INCLUDED

#include "semanticthread.h"

class StateAsIs: public SemanticThread
{
  public:
    typedef enum {
      MAXPARAM = 0
  } TParam;

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticThreadInfo& info );

  protected:
  private:
    static const TRecordType  validateMask = STATE;
    static const bool         initFromBegin = false;
};


#endif // SEMANTICTHREADFUNCTIONS_H_INCLUDED
