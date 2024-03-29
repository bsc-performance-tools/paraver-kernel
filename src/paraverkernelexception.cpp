/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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


#include <sstream>
#include "paraverkernelexception.h"

using namespace std;

ostream& ParaverKernelException::defaultPrintStream( cerr );

string ParaverKernelException::kernelMessage( "Paraver kernel exception: " );

const char *ParaverKernelException::errorMessage[] =
  {
    "Undefined error: ",
    "Empty trace not allowed: ",
    "Cannot open trace file: ",
    "Null pointer in operand: ",
    "Memory error: ",
    "Gzipped files not supported on Windows: ",
    "Requested Tool ID not defined: ",
    "Requested Tool Name not defined: ",
    "Index out of range: ",
    "Download failed: ",
    nullptr
  };

ParaverKernelException::ParaverKernelException( TErrorCode whichCode,
                                                const char *whichAuxMessage,
                                                const char *whichFile,
                                                TExceptionLine whichLine ) noexcept:
    code( whichCode ),
    auxMessage( whichAuxMessage ),
    file( whichFile ),
    line( whichLine )
{
  ostringstream tmpStream( "" );

  tmpStream << specificErrorMessage() << auxMessage << endl;

  if ( file != nullptr )
    tmpStream << file << " " << line << endl;

  message = string( tmpStream.str() );
}

const char *ParaverKernelException::what() const noexcept
{
  return message.c_str();
}

