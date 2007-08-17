#ifndef SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
#define SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED

#include "semanticcompose.h"

class ComposeAsIs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAsIs()
    {
      setDefaultParam();
    }

    ~ComposeAsIs()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
  protected:
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
    static const bool initFromBegin = false;

};


#endif // SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
