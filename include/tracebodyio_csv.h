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

#pragma once

#include "tracebodyio.h"
#include "tracestream.h"
#include "ktrace.h"

// Paraver trace old format file
class TraceBodyIO_csv : public TraceBodyIO<TraceStream,
                                           MemoryBlocks,
                                           ProcessModel<>,
                                           ResourceModel<>,
                                           TState,
                                           TEventType,
                                           MetadataManager,
                                           TRecordTime,
                                           MemoryTrace::iterator>
{
  public:
    TraceBodyIO_csv( ) {}
    TraceBodyIO_csv( Trace* trace, ProcessModel<>& whichProcessModel );

    static const PRV_UINT8 CommentRecord = '#';
    static const PRV_UINT8 StateRecord = '1';
    static const PRV_UINT8 EventRecord = '2';
    static const PRV_UINT8 CommRecord = '3';
    static const PRV_UINT8 GlobalCommRecord = '4';

    bool ordered() const override;
    void read( TraceStream *file,
               MemoryBlocks& records,
               const ProcessModel<>& whichProcessModel,
               const ResourceModel<>& whichResourceModel,
               std::unordered_set<TState>& states,
               std::unordered_set<TEventType>& events,
               MetadataManager& traceInfo,
               TRecordTime& endTime ) const override;
    void write( std::fstream& whichStream,
                const ProcessModel<>& whichProcessModel,
                const ResourceModel<>& whichResourceModel,
                MemoryTrace::iterator *record ) const override;

  protected:

  private:
    static std::istringstream fieldStream;
    static std::istringstream strLine;
    static std::string tmpstring;
    static std::string line;
    static std::ostringstream ostr;

     ProcessModel<> *myProcessModel;

    Trace* myTrace;

    void readTraceInfo( const std::string& line, MetadataManager& traceInfo ) const;

    void readEvents( const ResourceModel<>& whichResourceModel,
                     const std::string& line,
                     MemoryBlocks& records,
                     std::unordered_set<TState>& states,
                     TRecordTime& endTime ) const;

    bool readCommon( const ResourceModel<>& whichResourceModel,
                     std::istringstream& line,
                     TCPUOrder& CPU,
                     TApplOrder& appl,
                     TTaskOrder& task,
                     TThreadOrder& thread,
                     TRecordTime& begintime,
                     TRecordTime& time,
                     TEventValue& eventtype,
                     double& decimals ) const;

    void bufferWrite( std::fstream& whichStream, bool writeReady, bool lineClear = true  ) const;

};



