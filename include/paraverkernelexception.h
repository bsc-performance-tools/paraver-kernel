#ifndef PARAVERKERNELEXCEPTION_H_INCLUDED
#define PARAVERKERNELEXCEPTION_H_INCLUDED

#include <exception>
#include <iostream>
#include <string>

typedef unsigned int TExceptionLine;

using namespace std;

class ParaverKernelException : exception
{

  public:

    enum TErrorCode
    {
      undefined = 0,
      LAST
  };

    static ostream& defaultPrintStream;

    ParaverKernelException( const char *whichFile,
                            TExceptionLine whichLine,
                            TErrorCode whichCode = undefined,
                            const char *whichAuxMessage = NULL ):
        file( whichFile ),
        line( whichLine ),
        code( whichCode ),
        auxMessage( whichAuxMessage ) {}

    ~ParaverKernelException() throw();

    const char *what() const throw();

    void printMessage( ostream& printStream = defaultPrintStream ) const
      { printStream << what(); }

  protected:

    static string kernelMessage;

    static string moduleMessage;

    const char *file;

    TExceptionLine line;

    TErrorCode code;

    string auxMessage;

  private:
    static const char *errorMessage[];

};

ostream& ParaverKernelException::defaultPrintStream( cerr );

string ParaverKernelException::kernelMessage( "Paraver kernel exception: " );

string ParaverKernelException::moduleMessage( "" );

const char *ParaverKernelException::errorMessage[] =
  {
    "Undefined error.",
    NULL
  };



#endif // PARAVERKERNELEXCEPTION_H_INCLUDED
