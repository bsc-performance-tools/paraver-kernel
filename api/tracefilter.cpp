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


#include "tracefilter.h"
#include "kernelconnection.h"


std::string TraceFilter::traceToolID = "filter";
std::string TraceFilter::traceToolName = "Filter";
std::string TraceFilter::traceToolExtension = "filter";

TraceFilter *TraceFilter::create( const KernelConnection *whichKernel,
                                  char *traceIn,
                                  char *traceOut,
                                  TraceOptions *options,
                                  ProgressController *progress,
                                  const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable )
{
  return new TraceFilterProxy( whichKernel, traceIn, traceOut, options, whichTranslationTable, progress );
}


std::string TraceFilter::getID()
{
  return TraceFilter::traceToolID;
}


std::string TraceFilter::getName()
{
  return TraceFilter::traceToolName;
}


std::string TraceFilter::getExtension()
{
  return TraceFilter::traceToolExtension;
}


TraceFilterProxy::TraceFilterProxy( const KernelConnection *whichKernel,
                                    char *traceIn,
                                    char *traceOut,
                                    TraceOptions *options,
                                    const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                                    ProgressController *progress )
{
  myTraceFilter = whichKernel->newTraceFilter( traceIn, traceOut, options, whichTranslationTable, progress );
}

TraceFilterProxy::~TraceFilterProxy()
{
  delete myTraceFilter;
}
