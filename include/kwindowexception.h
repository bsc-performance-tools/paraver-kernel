#ifndef KWINDOWEXCEPTION_H_INCLUDED
#define KWINDOWEXCEPTION_H_INCLUDED

#include "paraverkernelexception.h"

class KWindowException: public ParaverKernelException
{
  public:
    typedef enum
    {
      undefined = 0,
      invalidLevel,
      LAST
    } TErrorCode;

    KWindowException( TErrorCode whichCode = undefined,
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


#endif // KWINDOWEXCEPTION_H_INCLUDED
