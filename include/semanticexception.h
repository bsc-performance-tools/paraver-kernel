#ifndef SEMANTICEXCEPTION_H_INCLUDED
#define SEMANTICEXCEPTION_H_INCLUDED

#include "paraverkernelexception.h"

class SemanticException: public ParaverKernelException
{
  public:
    typedef enum
    {
      undefined = 0,
      maxParamExceeded,
      LAST
  } TErrorCode;

    SemanticException( TErrorCode whichCode = undefined,
                       const char *whichAuxMessage = "",
                       const char *whichFile = NULL,
                       TExceptionLine whichLine = 0 ) throw()
    {
      code = whichCode;
      auxMessage = whichAuxMessage;
      file = whichFile;
      line = whichLine;
    }

  protected:

    static string moduleMessage;

    TErrorCode code;

  private:
    static const char *errorMessage[];

    virtual const char *specificErrorMessage() const
    {
      return errorMessage[ code ];
    }

    virtual string& specificModuleMessage() const
    {
      return moduleMessage;
    }
};


#endif // SEMANTICEXCEPTION_H_INCLUDED
