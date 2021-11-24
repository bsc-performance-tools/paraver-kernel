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


#ifdef _MSC_VER
#include <hash_set>
#else
#include  <unordered_set>
#endif
#include <fstream>
#include "memoryblocks.h"
#include "ktrace.h"
#include "tracestream.h"

#ifdef _MSC_VER
using namespace stdext;
#endif

class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO();

    virtual void setProcessModel( ProcessModel* whichProcessModel );
    virtual void setResourceModel( const ResourceModel* whichResourceModel );

    virtual bool ordered() const = 0;
    virtual void read( TraceStream *file, MemoryBlocks& records,
                       std::unordered_set<TState>& states, std::unordered_set<TEventType>& events,
                       MetadataManager& traceInfo, TRecordTime& endTime ) const = 0;
    virtual void write( std::fstream& whichStream,
                        const KTrace& whichTrace,
                        MemoryTrace::iterator *record,
                        PRV_INT32 numIter = 0 ) const = 0;
    virtual void writeCommInfo( std::fstream& whichStream,
                                const KTrace& whichTrace,
                                PRV_INT32 numIter = 1 ) const = 0;

//    virtual void close( std::fstream& whichStream, const KTrace& whichTrace ) = 0;

    static TraceBodyIO *createTraceBody( TraceStream *file, Trace *trace );
    static TraceBodyIO *createTraceBody();
  protected:
    ProcessModel* processModel;
    const ResourceModel* resourceModel;

};



