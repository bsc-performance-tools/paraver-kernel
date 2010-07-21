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

#ifndef KERNELCONNECTION_H_INCLUDED
#define KERNELCONNECTION_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"

class Window;
class Histogram;
class Trace;
class RecordList;
class ProgressController;
class Filter;

class TraceOptions;
class TraceCutter;
class TraceFilter;
class TraceSoftwareCounters;

using namespace std;

class KernelConnection
{
  public:
    virtual ~KernelConnection() {}

    virtual Trace *newTrace( const string& whichFile, bool noLoad, ProgressController *progress ) const = 0;
    virtual string getPCFFileLocation( const string& traceFile ) const = 0;
    virtual string getROWFileLocation( const string& traceFile ) const = 0;
    virtual Window *newSingleWindow() const = 0;
    virtual Window *newSingleWindow( Trace *whichTrace ) const = 0;
    virtual Window *newDerivedWindow() const = 0;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const = 0;
    virtual Histogram *newHistogram() const = 0;
    virtual ProgressController *newProgressController() const = 0;
    virtual Filter *newFilter( Filter *concreteFilter ) const = 0;

    virtual TraceOptions *newTraceOptions() const = 0;
    virtual TraceCutter *newTraceCutter( char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options  ) const = 0;
    virtual TraceFilter *newTraceFilter( char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options ) const = 0;
    virtual TraceSoftwareCounters *newTraceSoftwareCounters( char *trace_in,
                                                             char *trace_out,
                                                             TraceOptions *options) const = 0;

    virtual void getAllStatistics( vector<string>& onVector ) const = 0;
    virtual void getAllFilterFunctions( vector<string>& onVector ) const = 0;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          vector<string>& onVector ) const = 0;

    virtual bool userMessage( const string& message ) const = 0;

    #define INC_CHOP_COUNTER 0
    #define INC_SC_COUNTER 1
    #define INC_FILTER_COUNTER 2
    virtual void copyPCF( char *name, char *traceToLoad ) = 0;
    virtual void copyROW( char *name, char *traceToLoad ) = 0;
    virtual void getNewTraceName( char *name, char *new_trace_name, int action ) = 0;
    virtual char *composeName( char *name, char *newExtension ) = 0;


  protected:

  private:

};


#endif // KERNELCONNECTION_H_INCLUDED
