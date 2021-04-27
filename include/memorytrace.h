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


#ifndef MEMORYTRACE_H_INCLUDED
#define MEMORYTRACE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"

class MemoryBlocks;
class Trace;

typedef struct {} TData;

class MemoryTrace
{
  public:
    class iterator
    {
      public:
        iterator();
        iterator( const Trace *whichTrace );

        virtual ~iterator();

        virtual void operator++() = 0;
        virtual void operator--() = 0;
        virtual MemoryTrace::iterator& operator=( const MemoryTrace::iterator& copy );

        virtual bool operator==( const iterator &it ) const;
        virtual bool operator!=( const iterator &it ) const;
        virtual bool isNull() const;

        virtual iterator *clone() const = 0;

        virtual TRecordType    getType() const = 0;
        virtual TRecordTime    getTime() const = 0;
        virtual TThreadOrder   getThread() const = 0;
        virtual TCPUOrder      getCPU() const = 0;
        virtual TObjectOrder   getOrder() const = 0;
        virtual TEventType     getEventType() const = 0;
        virtual TSemanticValue getEventValue() const = 0;
        virtual TEventValue    getEventValueAsIs() const = 0;
        virtual TState         getState() const = 0;
        virtual TRecordTime    getStateEndTime() const = 0;
        virtual TCommID        getCommIndex() const = 0;

        virtual void         setTime( const TRecordTime time ) = 0;
        virtual void         setType( const TRecordType whichType ) = 0;
        virtual void         setStateEndTime( const TRecordTime whichEndTime ) = 0;

        virtual TData *getRecord() const
        {
          return record;
        }

      protected :
        TData *record;
        const Trace *myTrace;
    };

    MemoryTrace()
    {}

    virtual ~MemoryTrace()
    {}

    virtual void insert( MemoryBlocks *blocks ) = 0;
    virtual TTime finish( TTime headerTime, Trace *whichTrace ) = 0;

    virtual MemoryTrace::iterator* empty() const = 0;
    virtual MemoryTrace::iterator* begin() const = 0;
    virtual MemoryTrace::iterator* end() const = 0;

    virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const = 0;
    virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const = 0;
    virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const = 0;


    virtual void getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter,
                                        TRecordTime whichTime ) const = 0;
    virtual void getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter,
                                     TRecordTime whichTime ) const = 0;

  protected:

  private:

};



#endif // MEMORYTRACE_H_INCLUDED
