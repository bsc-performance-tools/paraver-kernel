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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef TRACEBODYIO_H_INCLUDED
#define TRACEBODYIO_H_INCLUDED

#ifdef WIN32
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include <fstream>
#include "memoryblocks.h"
#include "ktrace.h"
#include "tracestream.h"

#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO();

    virtual void setProcessModel( const ProcessModel* whichProcessModel );
    virtual void setResourceModel( const ResourceModel* whichResourceModel );

    virtual bool ordered() const = 0;
    virtual void read( TraceStream *file, MemoryBlocks& records,
                       hash_set<TEventType>& events, MetadataManager& traceInfo ) const = 0;
    virtual void write( std::fstream& whichStream,
                        const KTrace& whichTrace,
                        MemoryTrace::iterator *record,
                        PRV_INT32 numIter = 0 ) const = 0;
    virtual void writeCommInfo( std::fstream& whichStream,
                                const KTrace& whichTrace,
                                PRV_INT32 numIter = 1 ) const = 0;

//    virtual void close( std::fstream& whichStream, const KTrace& whichTrace ) = 0;

    static TraceBodyIO *createTraceBody( TraceStream *file);
    static TraceBodyIO *createTraceBody();
  protected:
    const ProcessModel* processModel;
    const ResourceModel* resourceModel;

  private:

};


#endif // TRACEBODYIO_H_INCLUDED
