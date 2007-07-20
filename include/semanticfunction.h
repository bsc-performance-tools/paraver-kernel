#ifndef SEMANTICFUNCTION_H_INCLUDED
#define SEMANTICFUNCTION_H_INCLUDED

#include <vector>
#include "memorytrace.h"
#include "semanticexception.h"

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

    virtual TParamIndex getMaxParam() const = 0;
    virtual const TParamValue& getParam( TParamIndex whichParam ) const
    {
      if( whichParam > getMaxParam() - 1 )
        throw SemanticException( SemanticException::maxParamExceeded );
      return parameters[whichParam];
    }

    virtual void setParam( TParamIndex whichParam, const TParamValue& newValue )
    {
      if( whichParam > getMaxParam() - 1 )
        throw SemanticException( SemanticException::maxParamExceeded );
      parameters[whichParam] = newValue;
    }
    //void setWindow( & whichWindow ) { window = whichWindow; }
    //& getWindow() { return window; }
    //
  protected:
    TParamValue parameters[];
    //& window -> associated window
    //parameters
  private:

};


#endif // SEMANTICFUNCTION_H_INCLUDED
