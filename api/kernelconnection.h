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
#include <map>

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
class TraceEditSequence;

typedef std::pair< unsigned long long, unsigned long long > TTypeValuePair;

class KernelConnection
{
  public:
    virtual ~KernelConnection() {}

    virtual bool checkTraceSize( const std::string& filename, TTraceSize maxSize ) const = 0;
    virtual TTraceSize getTraceSize( const std::string& filename ) const = 0;
    virtual Trace *newTrace( const std::string& whichFile, bool noLoad, ProgressController *progress ) const = 0;
    virtual std::string getPCFFileLocation( const std::string& traceFile ) const = 0;
    virtual std::string getROWFileLocation( const std::string& traceFile ) const = 0;
    virtual Window *newSingleWindow() const = 0;
    virtual Window *newSingleWindow( Trace *whichTrace ) const = 0;
    virtual Window *newDerivedWindow() const = 0;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const = 0;
    virtual Histogram *newHistogram() const = 0;
    virtual ProgressController *newProgressController() const = 0;
    virtual Filter *newFilter( Filter *concreteFilter ) const = 0;
    virtual TraceEditSequence *newTraceEditSequence() = 0;

    virtual std::string getToolID( const std::string &toolName ) const = 0;
    virtual std::string getToolName( const std::string &toolID ) const = 0;
    virtual TraceOptions *newTraceOptions() const = 0;
    virtual TraceCutter *newTraceCutter( TraceOptions *options,
                                         const std::vector< TEventType > &whichTypesWithValuesZero ) const = 0;
    virtual TraceFilter *newTraceFilter( char *trace_in,
                                         char *trace_out,
                                         TraceOptions *options,
                                         const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                                         ProgressController *progress = NULL ) const = 0;
    virtual TraceSoftwareCounters *newTraceSoftwareCounters( char *trace_in,
                                                             char *trace_out,
                                                             TraceOptions *options,
                                                             ProgressController *progress = NULL ) const = 0;

    virtual void getAllStatistics( std::vector<std::string>& onVector ) const = 0;
    virtual void getAllFilterFunctions( std::vector<std::string>& onVector ) const = 0;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          std::vector<std::string>& onVector ) const = 0;

    virtual bool userMessage( const std::string& message ) const = 0;

    virtual void copyPCF( const std::string& name, const std::string& traceToLoad ) = 0;
    virtual void copyROW( const std::string& name, const std::string&traceToLoad ) = 0;
    virtual void getNewTraceName( char *name,
                                  char *new_trace_name,
                                  std::string action,
                                  bool saveNewNameInfo = true ) = 0;
    // Returns modified fullPathTracName, with appended or modified filter suffix.
    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::string& traceFilterID,
                                         const bool commitName = false ) const = 0;

    // Returns modified fullPathTracName, with appended or modified filter suffixes.
    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::vector< std::string >& traceFilterID,
                                         const bool commitName = false ) const = 0;

    virtual std::string getNewTraceName( const std::string& fullPathTraceName,
                                         const std::string& outputPath,
                                         const std::vector< std::string >& traceFilterID,
                                         const bool commitName = false ) const = 0;

    //virtual char *composeName( char *name, char *newExtension ) = 0;
    virtual void commitNewTraceName( const std::string& newTraceName ) const = 0;


    virtual std::string getPathSeparator() const = 0;
    virtual void setPathSeparator( const std::string& whichPath ) = 0;

  protected:

  private:

};


#endif // KERNELCONNECTION_H_INCLUDED
