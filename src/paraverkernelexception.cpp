#include <sstream>
#include "paraverkernelexception.h"

ostream& ParaverKernelException::defaultPrintStream( cerr );

string ParaverKernelException::kernelMessage( "Paraver kernel exception: " );

string ParaverKernelException::moduleMessage( "" );

const char *ParaverKernelException::errorMessage[] =
  {
    "Undefined error: ",
    "Empty trace not allowed :",
    "Null pointer in operand: ",
    NULL
  };

const char *ParaverKernelException::what() const throw()
{
  ostringstream tempStream( "" );

  tempStream << kernelMessage << specificModuleMessage() << endl;
  tempStream << specificErrorMessage() << auxMessage << endl;

  if ( file != NULL )
    tempStream << file << " " << line << endl;

  return tempStream.str().c_str();
}

