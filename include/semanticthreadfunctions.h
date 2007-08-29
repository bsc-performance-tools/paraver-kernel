#ifndef SEMANTICTHREADFUNCTIONS_H_INCLUDED
#define SEMANTICTHREADFUNCTIONS_H_INCLUDED

#include "semanticthread.h"

class StateAsIs: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    StateAsIs()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


#endif // SEMANTICTHREADFUNCTIONS_H_INCLUDED
