/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/


#ifndef PARAVERKERNELEXCEPTION_H_INCLUDED
#define PARAVERKERNELEXCEPTION_H_INCLUDED

#include <exception>
#include <iostream>
#include <string>

typedef int TExceptionLine;

class ParaverKernelException : public std::exception
{

  public:

    typedef enum
    {
      undefined = 0,
      emptytrace,
      cannotOpenTrace,
      nullOperand,
      memoryError,
      gzipNotSupported,
      undefinedToolID,
      undefinedToolName,
      indexOutOfRange,
      LAST
    } TErrorCode;

    static std::ostream& defaultPrintStream;

    ParaverKernelException( TErrorCode whichCode = undefined,
                            const char *whichAuxMessage = "",
                            const char *whichFile = NULL,
                            TExceptionLine whichLine = 0 ) throw():
        code( whichCode ),
        auxMessage( whichAuxMessage ),
        file( whichFile ),
        line( whichLine )
    {};

    ~ParaverKernelException() throw() {};

    const char *what() throw();

    void printMessage( std::ostream& printStream = defaultPrintStream )
    {
      printStream << what();
    }

  protected:

    static std::string kernelMessage;

    static std::string moduleMessage;

    TErrorCode code;

    std::string auxMessage;

    const char *file;

    TExceptionLine line;

  private:
    static const char *errorMessage[];
    std::string message;

    virtual const char *specificErrorMessage() const
    {
      return errorMessage[ code ];
    }

    virtual std::string& specificModuleMessage() const

    {
      return moduleMessage;
    }

};

#endif // PARAVERKERNELEXCEPTION_H_INCLUDED
