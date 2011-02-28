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

#include "kwindow.h"
#include "intervalcontrolderived.h"

KRecordList *IntervalControlDerived::init( TRecordTime initialTime, TCreateList create,
    KRecordList *displayList )
{
  TRecordTime myInitTime;
  SemanticHighInfo info;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticDerived * ) window->getSemanticFunction( level );

  setChilds();

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  info.callingInterval = this;

  childIntervals[ 0 ]->init( myInitTime, createList, displayList );
  childIntervals[ 1 ]->init( myInitTime, createList, displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 1 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 1 ]->getEnd() );
  }
  while ( childIntervals[ 0 ]->getEnd()->getTime() < begin->getTime() )
    childIntervals[ 0 ]->calcNext( displayList );

  if ( childIntervals[ 0 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  while ( childIntervals[ 0 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 0 ]->calcNext( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalControlDerived::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  info.callingInterval = this;

  childIntervals[ 1 ]->calcNext( displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 1 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 1 ]->getEnd() );
  }

  if( begin->getTime() == window->getTrace()->getEndTime() )
    return displayList;

  while ( childIntervals[ 0 ]->getEnd()->getTime() <= begin->getTime() )
    childIntervals[ 0 ]->calcNext( displayList );

  if ( childIntervals[ 0 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  while ( childIntervals[ 0 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 0 ]->calcNext( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  return displayList;
}


KRecordList *IntervalControlDerived::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo info;

  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }
  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  info.callingInterval = this;

  childIntervals[ 1 ]->calcPrev( displayList );

  if ( window->getLevel() >= SYSTEM )
  {
    begin = window->copyCPUIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyCPUIterator( childIntervals[ 1 ]->getEnd() );
  }
  else
  {
    begin = window->copyThreadIterator( childIntervals[ 1 ]->getBegin() );
    end = window->copyThreadIterator( childIntervals[ 1 ]->getEnd() );
  }

  while ( childIntervals[ 0 ]->getEnd()->getTime() <= begin->getTime() )
    childIntervals[ 0 ]->calcPrev( displayList );

  if ( childIntervals[ 0 ]->getEnd()->getTime() > begin->getTime() )
  {
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  while ( childIntervals[ 0 ]->getEnd()->getTime() < end->getTime() )
  {
    childIntervals[ 0 ]->calcPrev( displayList );
    info.values.clear();
    info.values.push_back( currentValue );
    info.values.push_back( childIntervals[ 0 ]->getValue() *
                           window->getFactor( 0 ) );
    currentValue = function->execute( &info );
  }

  return displayList;
}


TWindowLevel IntervalControlDerived::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalControlDerived::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalControlDerived::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalControlDerived::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalControlDerived::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalControlDerived::setChilds()
{
  KWindow *window1;
  KWindow *window2;
  TApplOrder tmpAppl;
  TTaskOrder tmpTask;
  TThreadOrder tmpThread;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  childIntervals.clear();

  if ( window->getParent( 0 )->getLevel() > window->getParent( 1 )->getLevel() )
  {
    window1 = ( KWindow * ) window->getParent( 0 );
    window2 = ( KWindow * ) window->getParent( 1 );
  }
  else
  {
    window1 = ( KWindow * ) window->getParent( 1 );
    window2 = ( KWindow * ) window->getParent( 0 );
  }

  if ( window1->getLevel() == APPLICATION )
  {
    tmpAppl = order;
  }
  else if ( window1->getLevel() == TASK )
  {
    window1->getTrace()->getTaskLocation( order, tmpAppl, tmpTask );
  }
  else if ( window1->getLevel() == THREAD )
  {
    window1->getTrace()->getThreadLocation( order, tmpAppl, tmpTask, tmpThread );
  }
  else if ( window1->getLevel() == NODE )
  {
    tmpNode = order;
  }
  else if ( window1->getLevel() == CPU )
  {
    window1->getTrace()->getCPULocation( order, tmpNode, tmpCPU );
  }

  if ( window1 == window->getParent( 0 ) )
    childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order ) );

  if ( window2->getLevel() == WORKLOAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0 ) );
  }
  else if ( window2->getLevel() == APPLICATION )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpAppl ) );
  }
  else if ( window2->getLevel() == TASK )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalTask( tmpAppl, tmpTask ) ) );
  }
  else if ( window2->getLevel() == THREAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalThread( tmpAppl, tmpTask, tmpThread ) ) );
  }
  else if ( window2->getLevel() == SYSTEM )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0 ) );
  }
  else if ( window2->getLevel() == NODE )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpNode ) );
  }
  else if ( window2->getLevel() == CPU )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalCPU( tmpNode, tmpCPU ) ) );
  }

  if ( window1 == window->getParent( 1 ) )
    childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order ) );

}
