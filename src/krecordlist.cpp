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


#include "krecordlist.h"
#include "paraverkernelexception.h"
#include "kwindow.h"

KRecordList::KRecordList()
{
  newRec = false;
}

KRecordList::~KRecordList()
{}

void KRecordList::clear()
{
  list.clear();
  newRec = false;
}

void KRecordList::erase( iterator first, iterator last )
{
  list.erase( first, last );
  newRec = false;
}

RecordList::iterator KRecordList::begin()
{
  return list.begin();
}

RecordList::iterator KRecordList::end()
{
  return list.end();
}

bool KRecordList::newRecords() const
{
  return newRec;
}


void KRecordList::insert( KWindow *window, MemoryTrace::iterator *it )
{
  //static RLRecord tmp;
  RLRecord tmp;
  TCommID id = it->getCommIndex();
  tmp.setType( it->getType() );
  tmp.setTime( it->getTime() );
  if ( window->getLevel() >= WORKLOAD && window->getLevel() <= THREAD )
    tmp.setOrder( it->getOrder() );
  else if ( window->getLevel() >= SYSTEM && window->getLevel() <= CPU )
    tmp.setOrder( it->getOrder() );

  if ( tmp.getType() & EVENT )
  {
    tmp.setEventType( it->getEventType() );
    tmp.setEventValue( it->getEventValue() );
  }
  else if ( tmp.getType() & COMM )
  {
    KTrace *trace = ( KTrace * ) window->getTrace();
    TWindowLevel level = window->getLevel();
    tmp.setCommSize( trace->getCommSize( id ) );
    tmp.setCommTag( trace->getCommTag( id ) );
    tmp.setCommId( id );
    if ( it->getType() & SEND )
    {
      if ( level >= WORKLOAD && level <= THREAD )
        tmp.setCommPartnerObject( trace->getReceiverThread( id ) );
      else if ( level >= SYSTEM && level <= CPU )
        tmp.setCommPartnerObject( trace->getReceiverCPU( id ) );

      if ( it->getType() & LOG )
      {
        TRecordTime tmpTime = trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id )
                              ? trace->getLogicalReceive( id ) : trace->getPhysicalReceive( id );
        tmp.setCommPartnerTime( tmpTime );
      }
      else if ( it->getType() & PHY )
        tmp.setCommPartnerTime( trace->getPhysicalReceive( id ) );
    }
    else if ( it->getType() & RECV )
    {
      if ( level >= WORKLOAD && level <= THREAD )
        tmp.setCommPartnerObject( trace->getSenderThread( id ) );
      else if ( level >= SYSTEM && level <= CPU )
        tmp.setCommPartnerObject( trace->getSenderCPU( id ) );

      if ( it->getType() & LOG )
      {
        if ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) )
          return;
        tmp.setCommPartnerTime( trace->getLogicalSend( id ) );
      }
      else if ( it->getType() & PHY )
      {
        tmp.setCommPartnerTime( trace->getPhysicalSend( id ) );
        if ( trace->getLogicalReceive( id ) <= trace->getPhysicalReceive( id ) )
        {
          if ( window->getFilter()->getPhysical() && window->getFilter()->getLogical() )
          {
            // Inserts the physical comm
            list.insert( tmp );
          }

          if( window->getFilter()->getLogical() )
          {
            // Prepares the logical comm for insert later
            tmp.setType( tmp.getType() - PHY + LOG );
            tmp.setCommPartnerTime( trace->getLogicalSend( id ) );
          }
        }
      }
    }
  }

  list.insert( tmp );
  newRec = true;
}


RecordList *KRecordList::clone()
{
  KRecordList *clonedRecordList = new KRecordList();

  clonedRecordList->newRec = newRec;

  /*
  for ( set<RLRecord,ltrecord>::iterator it = list.begin(); it != list.end(); it++ )
  {
    RLRecord * currentRLRecord = new RLRecord( *it );
    clonedRecordList->list.insert( currentRLRecord );
  }
  */
  clonedRecordList->list = list;

  return clonedRecordList;
}
