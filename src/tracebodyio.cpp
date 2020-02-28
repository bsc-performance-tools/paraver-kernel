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

#include "tracebodyio_v1.h"
#include "tracebodyio_v2.h"
#include "tracebodyio_csv.h"

using namespace std;

TraceBodyIO::TraceBodyIO()
{}

TraceBodyIO::~TraceBodyIO()
{}

TraceBodyIO *TraceBodyIO::createTraceBody( TraceStream *file, const Trace *myTrace  )
//: whichTrace( trace )
{
  TraceBodyIO *ret;
  string firstLine;

  std::size_t lastDot = file->getFilename().find_last_of( '.' );
  std::string fileType = file->getFilename().substr( lastDot + 1 );
  if ( fileType == "csv" )
  {
    ret = new TraceBodyIO_csv( myTrace );
  }

  else
  {
#ifndef WIN32
    file->getline( firstLine );
    if ( firstLine.compare( "new format" ) == 0 )
    {
      ret = new TraceBodyIO_v2( myTrace );
    }
    else
    {
      ret = new TraceBodyIO_v1( myTrace );
      file->seekbegin();
    }
#else
    ret = new TraceBodyIO_v1( myTrace );
#endif
  }
  return ret;
}

TraceBodyIO *TraceBodyIO::createTraceBody()
{
  return new TraceBodyIO_v2();
}

void TraceBodyIO::setProcessModel( const ProcessModel* whichProcessModel )
{
  processModel = whichProcessModel;
}

void TraceBodyIO::setResourceModel( const ResourceModel* whichResourceModel )
{
  resourceModel = whichResourceModel;
}
