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

#include "tracecutter.h"
//#include "ktraceoptions.h"
#include "traceoptions.h"
//#include "ktracecutter.h"
#include "kernelconnection.h"

#ifdef OLD_PCFPARSER
#include "utils/pcfparser/old/ParaverTraceConfig.h"
#else
#include "utils/pcfparser/UIParaverTraceConfig.h"
#endif

using namespace libparaver;

#include <map>
#include <string.h>
#include "eventlabels.h"

//#ifdef WIN32
#include <sys/types.h>
#include <sys/stat.h>
//#endif

using namespace std;

string TraceCutter::traceToolID = "cutter";
string TraceCutter::traceToolName = "Cutter";
string TraceCutter::traceToolExtension = "chop";

TraceCutter *TraceCutter::create( const KernelConnection *whichKernel,
                                  string traceIn,
                                  string traceOut,
                                  TraceOptions *options,
                                  ProgressController *progress )
{
  return new TraceCutterProxy( whichKernel, traceIn, traceOut, options, progress );
}


string TraceCutter::getID()
{
  return TraceCutter::traceToolID;
}


string TraceCutter::getName()
{
  return TraceCutter::traceToolName;
}


string TraceCutter::getExtension()
{
  return TraceCutter::traceToolExtension;
}

TraceCutterProxy::TraceCutterProxy( const KernelConnection *whichKernel,
                                    string traceIn,
                                    string traceOut,
                                    TraceOptions *options,
                                    ProgressController *progress )
{
  string pcf_name;
  FILE *pcfFile;
  vector< TEventType > HWCTypes;
#ifdef WIN32
  struct _stat tmpStatBuffer;
#else
  struct stat tmpStatBuffer;
#endif

  pcf_name = LocalKernel::composeName( traceIn, string( "pcf" ) );
  int statReturn;
#ifdef WIN32
  statReturn = _stat( pcf_name.c_str(), &tmpStatBuffer );
#else
  statReturn = stat( pcf_name.c_str(), &tmpStatBuffer );
#endif

  if( statReturn == 0 && tmpStatBuffer.st_size > 0 )
  {
#ifdef OLD_PCFPARSER
    ParaverTraceConfig *config = new ParaverTraceConfig( pcf_name );
    config->parse();
#else
    UIParaverTraceConfig *config = new UIParaverTraceConfig();
    config->parse( pcf_name );
#endif

    EventLabels myEventLabels = EventLabels( *config, set<TEventType>() );
    vector< TEventType > allTypes;
    myEventLabels.getTypes( allTypes );
    for( vector< TEventType >::iterator it = allTypes.begin(); it != allTypes.end(); ++it )
    {
      if( *it >= 42000000 && *it < 43000000 )
        HWCTypes.push_back( *it );
    }

    delete config;
  }

  myTraceCutter = whichKernel->newTraceCutter( options, HWCTypes );
}


TraceCutterProxy::~TraceCutterProxy()
{
  delete myTraceCutter;
}


void TraceCutterProxy::execute( string trace_in, string trace_out, ProgressController *progress )
{
  myTraceCutter->execute( trace_in, trace_out, progress );
}


void TraceCutterProxy::set_by_time( bool whichByTime )
{
  myTraceCutter->set_by_time( whichByTime );
}


void TraceCutterProxy::set_min_cutting_time( unsigned long long minCutTime )
{
  myTraceCutter->set_min_cutting_time( minCutTime );
}

void TraceCutterProxy::set_max_cutting_time( unsigned long long maxCutTime )
{
  myTraceCutter->set_min_cutting_time( maxCutTime );
}

void TraceCutterProxy::set_minimum_time_percentage( unsigned long long minimumPercentage )
{
  myTraceCutter->set_minimum_time_percentage( minimumPercentage );
}

void TraceCutterProxy::set_maximum_time_percentage( unsigned long long maximumPercentage )
{
  myTraceCutter->set_maximum_time_percentage( maximumPercentage );
}

void TraceCutterProxy::set_tasks_list( char *tasksList )
{
  myTraceCutter->set_tasks_list( tasksList );
}

void TraceCutterProxy::set_original_time( bool originalTime )
{
  myTraceCutter->set_original_time ( originalTime );
}

void TraceCutterProxy::set_max_trace_size( int traceSize )
{
  myTraceCutter->set_max_trace_size( traceSize );
}

void TraceCutterProxy::set_break_states( bool breakStates )
{
  myTraceCutter->set_break_states( breakStates );
}

void TraceCutterProxy::set_remFirstStates( bool remStates )
{
  myTraceCutter->set_remFirstStates ( remStates );
}

void TraceCutterProxy::set_remLastStates( bool remStates )
{
  myTraceCutter->set_remLastStates( remStates );
}

void TraceCutterProxy::set_keep_events( bool keepEvents )
{
  myTraceCutter->set_keep_events( keepEvents );
}

void TraceCutterProxy::setCutterApplicationCaller( string caller )
{
  myTraceCutter->setCutterApplicationCaller( caller );
}


