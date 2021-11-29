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

// Paraver trace old format file
class TraceBodyIO_v1 : public TraceBodyIO
{
  public:
    TraceBodyIO_v1() {}

    static constexpr PRV_UINT8 CommentRecord = '#';
    static constexpr PRV_UINT8 StateRecord = '1';
    static constexpr PRV_UINT8 EventRecord = '2';
    static constexpr PRV_UINT8 CommRecord = '3';
    static constexpr PRV_UINT8 GlobalCommRecord = '4';

    bool ordered() const override;
    void read( TraceStream *file,
               MemoryBlocks& records,
               ProcessModel& whichProcessModel,
               const ResourceModel& whichResourceModel,
               std::unordered_set<TState>& states,
               std::unordered_set<TEventType>& events,
               MetadataManager& traceInfo,
               TRecordTime& endTime ) const override;
    void write( std::fstream& whichStream,
                const ProcessModel& whichProcessModel,
                const ResourceModel& whichResourceModel,
                MemoryTrace::iterator *record ) const override;

  protected:

  private:
    // Multievent lines
    typedef struct TMultiEventCommonInfo
    {
      // Used for multistreaming output (i.e. event driven cutter)
      std::fstream *myStream;

      TThreadOrder thread;
      TCPUOrder cpu;
      TRecordTime time;
    }
    TMultiEventCommonInfo;

    static TMultiEventCommonInfo multiEventCommonInfo;
    static std::string multiEventLine;

    static std::string line;
    static std::ostringstream ostr;

    void readTraceInfo( const std::string& line, MetadataManager& traceInfo ) const;

    void readState( const std::string& line,
                    const ProcessModel& whichProcessModel,
                    const ResourceModel& whichResourceModel,
                    MemoryBlocks& records,
                    std::unordered_set<TState>& states ) const;
    void readEvent( const std::string& line,
                    const ProcessModel& whichProcessModel,
                    const ResourceModel& whichResourceModel,
                    MemoryBlocks& records,
                    std::unordered_set<TEventType>& events ) const;
    void readComm( const std::string& line,
                   const ProcessModel& whichProcessModel,
                   const ResourceModel& whichResourceModel,
                   MemoryBlocks& records ) const;
    void readGlobalComm( const std::string& line, MemoryBlocks& records ) const;
    bool readCommon( const ProcessModel& whichProcessModel,
                     const ResourceModel& whichResourceModel,
                     std::string::const_iterator& it,
                     const std::string::const_iterator& end,
                     TCPUOrder& CPU,
                     TApplOrder& appl,
                     TTaskOrder& task,
                     TThreadOrder& thread,
                     TRecordTime& time ) const;

    void bufferWrite( std::fstream& whichStream, bool writeReady, bool lineClear = true  ) const;

    bool writeState( const ProcessModel& whichProcessModel,
                     const ResourceModel& whichResourceModel,
                     const MemoryTrace::iterator *record ) const;
    bool writePendingMultiEvent( const ProcessModel& whichProcessModel ) const;
    void appendEvent( const MemoryTrace::iterator *record ) const;
    bool writeEvent( const ProcessModel& whichProcessModel,
                     const MemoryTrace::iterator *record,
                     bool needCommons = true ) const;
    bool writeComm( const ProcessModel& whichProcessModel,
                    const ResourceModel& whichResourceModel,
                    const MemoryTrace::iterator *record ) const;
    bool writeGlobalComm( const ProcessModel& whichProcessModel,
                          const MemoryTrace::iterator *record ) const;
    void writeCommon( std::ostringstream& line,
                      const ProcessModel& whichProcessModel,
                      const ResourceModel& whichResourceModel,
                      const MemoryTrace::iterator *record ) const;

    bool sameMultiEvent( const MemoryTrace::iterator *record ) const;
};



