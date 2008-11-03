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

KRecordList::iterator KRecordList::begin() const
{
  return list.begin();
}

KRecordList::iterator KRecordList::end() const
{
  return list.end();
}

bool KRecordList::newRecords() const
{
  return newRec;
}

void KRecordList::insert( KWindow *window, MemoryTrace::iterator *it )
{
  RLRecord tmp;
  TCommID id = it->getCommIndex();
  tmp.setType( it->getType() );
  tmp.setTime( it->getTime() );
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
    if ( it->getType() & SEND )
    {
      if ( level >= WORKLOAD && level <= THREAD )
        tmp.setCommPartnerObject( trace->getReceiverThread( id ) );
      else if( level >=SYSTEM && level <= CPU )
        tmp.setCommPartnerObject( trace->getReceiverCPU( id ) );

      if ( it->getType() & LOG )
      {
        TRecordTime tmpTime = trace->getLogicalReceive( id ) > trace->getPhysicalReceive( id )
                              ? trace->getLogicalReceive( id ) : trace->getPhysicalReceive( id );
        tmp.setCommPartnerTime( tmpTime );
      }
      else if( it->getType() & PHY )
        tmp.setCommPartnerTime( trace->getPhysicalReceive( id ) );
    }
    else if( it->getType() & RECV )
    {
      if ( level >= WORKLOAD && level <= THREAD )
        tmp.setCommPartnerObject( trace->getSenderThread( id ) );
      else if( level >=SYSTEM && level <= CPU )
        tmp.setCommPartnerObject( trace->getSenderCPU( id ) );

      if ( it->getType() & LOG )
        tmp.setCommPartnerTime( trace->getLogicalSend( id ) );
      else if( it->getType() & PHY )
        tmp.setCommPartnerTime( trace->getLogicalReceive( id ) );
    }
    tmp.setCommSize( trace->getCommSize( id ) );
    tmp.setCommTag( trace->getCommTag( id ) );
  }
  else
    throw ParaverKernelException();

  list.insert( tmp );

  newRec = true;
}

