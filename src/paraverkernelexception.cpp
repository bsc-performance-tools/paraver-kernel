#include <sstream>
#include "paraverkernelexception.h"

const char *ParaverKernelException::what() const throw()
{
  ostringstream tempStream( "" );

  tempStream << kernelMessage << moduleMessage << errorMessage[ code ] << endl;
  tempStream << auxMessage << endl;
  tempStream << file << " " << line << endl;

  return tempStream.str().c_str();
}
