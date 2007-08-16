#ifndef SEMANTICFUNCTION_H_INCLUDED
#define SEMANTICFUNCTION_H_INCLUDED

#include <vector>
#include "memorytrace.h"
#include "semanticexception.h"
#include "semanticinfo.h"

using namespace std;

typedef UINT8 TParamIndex;
typedef vector<double> TParamValue;

class SemanticFunction
{
  public:
    SemanticFunction()
    {}
    virtual ~SemanticFunction()
    {}

    void setDefaultParam()
    {
      for ( TParamIndex i = 0; i < getMaxParam(); i++ )
        parameters[i] = getDefaultParam( i );
    }

    virtual TParamIndex getMaxParam() const = 0;
    virtual const TParamValue& getParam( TParamIndex whichParam ) const
    {
      if ( whichParam > getMaxParam() - 1 )
        throw SemanticException( SemanticException::maxParamExceeded );
      return parameters[whichParam];
    }

    virtual void setParam( TParamIndex whichParam, const TParamValue& newValue )
    {
      if ( whichParam > getMaxParam() - 1 )
        throw SemanticException( SemanticException::maxParamExceeded );
      parameters[whichParam] = newValue;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) = 0;
    //void setWindow( & whichWindow ) { window = whichWindow; }
    //& getWindow() { return window; }
    //
  protected:
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) = 0;
    TParamValue parameters[];
    //& window -> associated window
  private:

};


#endif // SEMANTICFUNCTION_H_INCLUDED
