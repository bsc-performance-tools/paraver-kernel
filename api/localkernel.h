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

#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class KWindow;

class LocalKernel: public KernelConnection
{
  public:
    static void init();

    LocalKernel( bool (*messageFunction)(string) );
    virtual ~LocalKernel();

    virtual bool checkTraceSize( const string& filename, TTraceSize maxSize ) const;
    virtual TTraceSize getTraceSize( const string& filename ) const;
    virtual Trace *newTrace( const string& whichFile, bool noLoad, ProgressController *progress = NULL ) const;
    virtual string getPCFFileLocation( const string& traceFile ) const;
    virtual string getROWFileLocation( const string& traceFile ) const;
    virtual Window *newSingleWindow() const;
    virtual Window *newSingleWindow( Trace *whichTrace ) const;
    virtual Window *newDerivedWindow() const;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newHistogram() const;
//    virtual RecordList *newRecordList() const;
    virtual ProgressController *newProgressController() const;
    virtual Filter *newFilter( Filter *concreteFilter ) const;

    virtual TraceOptions *newTraceOptions() const;
    virtual TraceCutter *newTraceCutter( //TraceCutter *concreteTraceCutter,
                                         char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options,
                                         const vector< TEventType > &whichTypesWithValuesZero,
                                         ProgressController *progress = NULL) const;
    virtual TraceFilter *newTraceFilter( char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options,
                                         ProgressController *progress = NULL ) const;
    virtual TraceSoftwareCounters *newTraceSoftwareCounters( char *trace_in,
                                                             char *trace_out,
                                                             TraceOptions *options,
                                                             ProgressController *progress = NULL ) const;

    virtual void getAllStatistics( vector<string>& onVector ) const;
    virtual void getAllFilterFunctions( vector<string>& onVector ) const;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          vector<string>& onVector ) const;

    virtual bool userMessage( const string& message ) const;

    virtual void copyPCF( char *name, char *traceToLoad );
    virtual void copyROW( char *name, char *traceToLoad );
    virtual void getNewTraceName( char *name, char *new_trace_name, int action );
    virtual char *composeName( char *name, char *newExtension );

  protected:

  private:
    bool (*myMessageFunction)(string);

    // FILTERS
    struct traces_table {
      char *name;
      int num_chop;
      int num_filter;
      int num_sc;
    };

    #define MAX_TRACES_HISTORY_LENGTH 256
    struct traces_table trace_names_table[ MAX_TRACES_HISTORY_LENGTH ];
    int trace_names_table_last; // should be static?

    void copyFile( char *in, char *out );
};


#endif // LOCALKERNEL_H_INCLUDED
