#ifndef HISTOGRAMEXCEPTION_H_INCLUDED
#define HISTOGRAMEXCEPTION_H_INCLUDED

#include "paraverkernelexception.h"

class HistogramException: public ParaverKernelException
{
  public:
    typedef enum
    {
      undefined = 0,
      noControlWindow,
      LAST
    } TErrorCode;

    HistogramException( TErrorCode whichCode = undefined,
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
    {
      return errorMessage[ code ];
    }
    virtual string& specificModuleMessage() const
    {
      return moduleMessage;
    }

};

#endif // HISTOGRAMEXCEPTION_H_INCLUDED
