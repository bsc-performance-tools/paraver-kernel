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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "traceshifter.h"
#include "kernelconnection.h"
#include "traceeditsequence.h"

using namespace std;

std::string TraceShifter::traceToolID = "shifter";
std::string TraceShifter::traceToolName = "trace_shifter";
std::string TraceShifter::traceToolExtension = "shift";


TraceShifter *TraceShifter::create( const KernelConnection *whichKernel,
                                    std::string traceIn,
                                    std::string traceOut,
                                    std::string shiftTimesFile,
                                    //TraceOptions *options,
                                    ProgressController *progress )
{
  return new TraceShifterProxy( whichKernel, traceIn, traceOut, shiftTimesFile, /*, options,*/ progress );
}


TraceShifterProxy::TraceShifterProxy( const KernelConnection *whichKernel,
                                      std::string traceIn,
                                      std::string traceOut,
                                      std::string shiftTimesFile,
                                      //TraceOptions *options,
                                      ProgressController *progress )
{
  myKernel = whichKernel;
  myTraceShifter = whichKernel->newTraceShifter( traceIn, traceOut, shiftTimesFile, progress );
}


TraceShifterProxy::~TraceShifterProxy()
{
  delete myTraceShifter;
}


void TraceShifterProxy::execute( std::string traceIn,
                                 std::string traceOut,
                                 ProgressController *progress )
{
  myTraceShifter->execute( traceIn, traceOut, progress );
}

// TODO: Reconsider this method; Is to know the size the only use of it?
const std::vector< TTime > TraceShifterProxy::getShiftTimes()
{
  return myTraceShifter->getShiftTimes();
}
