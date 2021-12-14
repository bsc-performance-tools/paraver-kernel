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

// Paraver trace NEW format file
class TraceBodyIO_v2 : public TraceBodyIO<TraceStream,
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
    static const PRV_UINT8 StateBeginRecord = '1';
    static const PRV_UINT8 StateEndRecord = '2';
    static const PRV_UINT8 EventRecord = '3';
    static const PRV_UINT8 CommRecord = 'd';
    static const PRV_UINT8 LogicalSendRecord = '4';
    static const PRV_UINT8 LogicalRecvRecord = '5';
    static const PRV_UINT8 PhysicalSendRecord = '6';
    static const PRV_UINT8 PhysicalRecvRecord = '7';
    static const PRV_UINT8 GlobalCommRecord = '8';

    TraceBodyIO_v2() {}

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
    void writeEvents( std::fstream& whichStream,
                      const ProcessModel<>& whichProcessModel,
                      const ResourceModel<>& whichResourceModel,
                      std::vector<MemoryTrace::iterator *>& recordList ) const;

    void writeCommInfo( std::fstream& whichStream,
                        const KTrace& whichTrace,
                        PRV_INT32 numIter = 1 ) const;

  protected:

  private:
    void readState( const string& line, 
                    const ProcessModel<>& whichProcessModel,
                    const ResourceModel<>& whichResourceModel,
                    MemoryBlocks& records,
                    std::unordered_set<TState>& states ) const;
    void readEvent( const string& line,
                    const ProcessModel<>& whichProcessModel,
                    const ResourceModel<>& whichResourceModel,
                    MemoryBlocks& records,
                    std::unordered_set<TEventType>& events ) const;
    void readComm( const std::string& line,
                   const ProcessModel<>& whichProcessModel,
                   const ResourceModel<>& whichResourceModel,
                   MemoryBlocks& records ) const;
    void readGlobalComm( const std::string& line, MemoryBlocks& records ) const;
    bool readCommon( std::istringstream& line,
                     const ProcessModel<>& whichProcessModel,
                     const ResourceModel<>& whichResourceModel,
                     TCPUOrder& CPU,
                     TThreadOrder& thread,
                     TRecordTime& time ) const;

    bool writeState( std::string& line,
                     const ProcessModel<>& whichProcessModel,
                     const ResourceModel<>& whichResourceModel,
                     MemoryTrace::iterator *record ) const;
    bool writeEvent( std::string& line,
                     const ProcessModel<>& whichProcessModel,
                     const ResourceModel<>& whichResourceModel,
                     MemoryTrace::iterator *record,
                     bool needCommons = true ) const;
    bool writeCommRecord( std::string& line,
                          MemoryTrace::iterator *record ) const;
    bool writeGlobalComm( std::string& line,
                          const ProcessModel<>& whichProcessModel,
                          MemoryTrace::iterator *record ) const;
    void writeCommon( std::ostringstream& line,
                      const ProcessModel<>& whichProcessModel,
                      const ResourceModel<>& whichResourceModel,
                      MemoryTrace::iterator *record ) const;

};



