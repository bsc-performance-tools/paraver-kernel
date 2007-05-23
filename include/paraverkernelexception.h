#ifndef PARAVERKERNELEXCEPTION_H_INCLUDED
#define PARAVERKERNELEXCEPTION_H_INCLUDED

#include <exception>
#include <iostream>
#include <string>

typedef int TExceptionLine;

using namespace std;

class ParaverKernelException : public exception
{

  public:

    typedef enum
    {
      undefined = 0,
      emptytrace,
      nullOperand,
      LAST
  } TErrorCode;

    static ostream& defaultPrintStream;

    ParaverKernelException( TErrorCode whichCode = undefined,
                            const char *whichAuxMessage = "",
                            const char *whichFile = NULL,
                            TExceptionLine whichLine = 0 ):
        code( whichCode ),
        auxMessage( whichAuxMessage ),
        file( whichFile ),
        line( whichLine ) {};

    ~ParaverKernelException() throw() {};

    const char *what() const throw();

    void printMessage( ostream& printStream = defaultPrintStream ) const
      { printStream << what(); }

  protected:

    static string kernelMessage;

    static string moduleMessage;

    TErrorCode code;

    string auxMessage;

    const char *file;

    TExceptionLine line;

  private:
    static const char *errorMessage[];

    virtual const char *specificErrorMessage() const
      { return errorMessage[ code ]; }

    virtual string& specificModuleMessage() const

      { return moduleMessage; }

};

#endif // PARAVERKERNELEXCEPTION_H_INCLUDED
