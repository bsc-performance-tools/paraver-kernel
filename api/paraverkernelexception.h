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


#pragma once


#include <exception>
#include <iostream>
#include <string>

typedef int TExceptionLine;

enum class TErrorCode
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
  downloadFailed,
  LAST
};

class ParaverKernelException : public std::exception
{

  public:

    static std::ostream& defaultPrintStream;

    ParaverKernelException( TErrorCode whichCode = TErrorCode::undefined,
                            const char *whichAuxMessage = "",
                            const char *whichFile = nullptr,
                            TExceptionLine whichLine = 0 ) noexcept:
        code( whichCode ),
        auxMessage( whichAuxMessage ),
        file( whichFile ),
        line( whichLine )
    {};

    ~ParaverKernelException() noexcept = default;

    const char *what() const noexcept;

    void printMessage( std::ostream& printStream = defaultPrintStream )
    {
      printStream << what();
    }

  protected:

    static std::string kernelMessage;

    TErrorCode code;

    std::string auxMessage;

    const char *file;

    TExceptionLine line;

    virtual const char *specificErrorMessage() const
    {
      return errorMessage[ static_cast<int>( code ) ];
    }

  private:
    static const char *errorMessage[];

};


