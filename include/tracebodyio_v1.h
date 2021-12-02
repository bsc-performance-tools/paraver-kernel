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
template< class    TraceStreamT,
          class    RecordContainerT,
          class    ProcessModelT,
          class    ResourceModelT,
          typename StateT,
          typename EventTypeT,
          class    MetadataManagerT,
          typename RecordTimeT,
          class    RecordT>
class TraceBodyIO_v1 : public TraceBodyIO<TraceStreamT,
                                          RecordContainerT,
                                          ProcessModelT,
                                          ResourceModelT,
                                          StateT,
                                          EventTypeT,
                                          MetadataManagerT,
                                          RecordTimeT,
                                          RecordT>
{
  public:
    TraceBodyIO_v1() {}

    static constexpr PRV_UINT8 CommentRecord = '#';
    static constexpr PRV_UINT8 StateRecord = '1';
    static constexpr PRV_UINT8 EventRecord = '2';
    static constexpr PRV_UINT8 CommRecord = '3';
    static constexpr PRV_UINT8 GlobalCommRecord = '4';

    bool ordered() const override;
    void read( TraceStreamT *file,
               RecordContainerT& records,
               const ProcessModelT& whichProcessModel,
               const ResourceModelT& whichResourceModel,
               std::unordered_set<StateT>& states,
               std::unordered_set<EventTypeT>& events,
               MetadataManagerT& traceInfo,
               RecordTimeT& endTime ) const override;
    void write( std::fstream& whichStream,
                const ProcessModelT& whichProcessModel,
                const ResourceModelT& whichResourceModel,
                RecordT *record ) const override;

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

    void readTraceInfo( const std::string& line, MetadataManagerT& traceInfo ) const;

    void readState( const std::string& line,
                    const ProcessModelT& whichProcessModel,
                    const ResourceModelT& whichResourceModel,
                    RecordContainerT& records,
                    std::unordered_set<StateT>& states ) const;
    void readEvent( const std::string& line,
                    const ProcessModelT& whichProcessModel,
                    const ResourceModelT& whichResourceModel,
                    RecordContainerT& records,
                    std::unordered_set<EventTypeT>& events ) const;
    void readComm( const std::string& line,
                   const ProcessModelT& whichProcessModel,
                   const ResourceModelT& whichResourceModel,
                   RecordContainerT& records ) const;
    void readGlobalComm( const std::string& line, RecordContainerT& records ) const;
    bool readCommon( const ProcessModelT& whichProcessModel,
                     const ResourceModelT& whichResourceModel,
                     std::string::const_iterator& it,
                     const std::string::const_iterator& end,
                     TCPUOrder& CPU,
                     TApplOrder& appl,
                     TTaskOrder& task,
                     TThreadOrder& thread,
                     RecordTimeT& time ) const;

    void bufferWrite( std::fstream& whichStream, bool writeReady, bool lineClear = true  ) const;

    bool writeState( const ProcessModelT& whichProcessModel,
                     const ResourceModelT& whichResourceModel,
                     const RecordT *record ) const;
    bool writePendingMultiEvent( const ProcessModelT& whichProcessModel ) const;
    void appendEvent( const RecordT *record ) const;
    bool writeEvent( const ProcessModelT& whichProcessModel,
                     const RecordT *record,
                     bool needCommons = true ) const;
    bool writeComm( const ProcessModelT& whichProcessModel,
                    const ResourceModelT& whichResourceModel,
                    const RecordT *record ) const;
    bool writeGlobalComm( const ProcessModelT& whichProcessModel,
                          const RecordT *record ) const;
    void writeCommon( std::ostringstream& line,
                      const ProcessModelT& whichProcessModel,
                      const ResourceModelT& whichResourceModel,
                      const RecordT *record ) const;

    bool sameMultiEvent( const RecordT *record ) const;
};

#include "../src/tracebodyio_v1.cpp"

