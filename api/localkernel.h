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

#include <map>

#include "kernelconnection.h"

class KWindow;
class PreviousFiles;

class LocalKernel: public KernelConnection
{
  public:
    static void init();

    LocalKernel( bool (*messageFunction)(std::string) );
    virtual ~LocalKernel();

    virtual bool checkTraceSize( const std::string& filename, TTraceSize maxSize ) const;
    virtual TTraceSize getTraceSize( const std::string& filename ) const;
    virtual Trace *newTrace( const std::string& whichFile, bool noLoad, ProgressController *progress = NULL ) const;
    virtual std::string getPCFFileLocation( const std::string& traceFile ) const;
    virtual std::string getROWFileLocation( const std::string& traceFile ) const;
    virtual Window *newSingleWindow() const;
    virtual Window *newSingleWindow( Trace *whichTrace ) const;
    virtual Window *newDerivedWindow() const;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newHistogram() const;
//    virtual RecordList *newRecordList() const;
    virtual ProgressController *newProgressController() const;
    virtual Filter *newFilter( Filter *concreteFilter ) const;

    virtual std::string getToolID( const std::string &toolName ) const;
    virtual std::string getToolName( const std::string &toolID ) const;
    virtual TraceOptions *newTraceOptions() const;
    virtual TraceCutter *newTraceCutter( //TraceCutter *concreteTraceCutter,
                                         char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options,
                                         const std::vector< TEventType > &whichTypesWithValuesZero,
                                         ProgressController *progress = NULL) const;
    virtual TraceFilter *newTraceFilter( char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options,
                                         const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                                         ProgressController *progress = NULL ) const;
    virtual TraceSoftwareCounters *newTraceSoftwareCounters( char *trace_in,
                                                             char *trace_out,
                                                             TraceOptions *options,
                                                             ProgressController *progress = NULL ) const;

    virtual void getAllStatistics( std::vector<std::string>& onVector ) const;
    virtual void getAllFilterFunctions( std::vector<std::string>& onVector ) const;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          std::vector<std::string>& onVector ) const;

    virtual bool userMessage( const std::string& message ) const;

    virtual void copyPCF( char *name, char *traceToLoad );
    virtual void copyROW( char *name, char *traceToLoad );

    // WILL BE SUBSTITUTED
    virtual void getNewTraceName( char *name,
                                  char *new_trace_name,
                                  std::string action,
                                  bool saveNewNameInfo = true );

    // Returns modified fullPathTracName, with appended or modified filter suffix.
    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::string& traceFilterID,
                                         const bool commitName = false ) const;

    // Returns modified fullPathTracName, with appended or modified filter suffixes.
    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::vector< std::string >& traceFilterID,
                                         const bool commitName = false ) const;

    // Returns modified fullPathTracName, with appended or modified filter suffixes.
    // Takes path as root for the trace
    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::string& outputPath,
                                         const std::vector< std::string >& traceFilterID,
                                         const bool commitName = false ) const;

    virtual char *composeName( char *name, char *newExtension );
    virtual void commitNewTraceName( const std::string& newTraceName ) const;

    inline virtual std::string getPathSeparator() const { return pathSeparator; }
    inline virtual void setPathSeparator( const std::string& whichPath ) { pathSeparator = whichPath; }

  protected:

  private:
    std::string pathSeparator;

    bool (*myMessageFunction)(std::string);

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

    // Paraver user file that stores path and names of last cutted/filtered traces
    // It's opened/created in LocalKernel constructor.
    // And written in getNewTraceName when commitName == true
    PreviousFiles *prevTraceNames;

    void copyFile( char *in, char *out );
};


#endif // LOCALKERNEL_H_INCLUDED
