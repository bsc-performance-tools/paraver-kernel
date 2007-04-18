#ifndef TRACEHEADEREXCEPTION_H_INCLUDED
#define TRACEHEADEREXCEPTION_H_INCLUDED

#include "paraverkernelexception.h"

class TraceHeaderException: public ParaverKernelException
{

  public:
    typedef enum
    {
      undefined = 0,
      invalidApplNumber,
      invalidTaskNumber,
      invalidThreadNumber,
      invalidNodeNumber,
      LAST
  } TErrorCode;

    TraceHeaderException( TErrorCode whichCode = undefined,

                          const char *whichAuxMessage = "",
                          const char *whichFile = NULL,
                          TExceptionLine whichLine = 0 )
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
      { return errorMessage[ code ]; }

    virtual string& specificModuleMessage() const

      { return moduleMessage; }

};



#endif // TRACEHEADEREXCEPTION_H_INCLUDED
