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
#include "intervalderived.h"

KRecordList *IntervalDerived::init( TRecordTime initialTime, TCreateList create,
                                   KRecordList *displayList )
{
  TRecordTime myInitTime;
  info.values.clear();
  info.callingInterval = this;

  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

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

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      begin = childIntervals[ i ]->getBegin()->clone();
    }

    if ( end == NULL ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end != NULL )
        delete end;
      end = childIntervals[ i ]->getEnd()->clone();
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalDerived::calcNext( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( begin != NULL )
    delete begin;
  begin = end->clone();

  if ( end != NULL )
  {
    delete end;
    end = NULL;
  }

  for ( TObjectOrder i = 0; i < childIntervals.size(); ++i )
  {
    if ( childIntervals[ i ]->getEnd()->getTime() <= begin->getTime() )
//    if ( *childIntervals[ i ]->getEnd() == *begin )
      childIntervals[ i ]->calcNext( displayList );

    if ( ( end == NULL
           && childIntervals[ i ]->getEnd()->getTime() < childIntervals[ i ]->getWindow()->getTrace()->getEndTime() )
         ||
         ( end != NULL
           && childIntervals[ i ]->getEnd()->getTime() < end->getTime()
           && childIntervals[ i ]->getEnd()->getTime() < childIntervals[ i ]->getWindow()->getTrace()->getEndTime() )
        )
    {
      if ( end != NULL )
        delete end;
      end = childIntervals[ i ]->getEnd()->clone();
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  if( end == NULL )
    end = ( (KTrace *)window->getTrace() )->end();

  currentValue = function->execute( &info );

  return displayList;
}


KRecordList *IntervalDerived::calcPrev( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( end != NULL )
    delete end;
  end = begin->clone();

  if ( begin != NULL )
  {
    delete begin;
    begin = NULL;
  }

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    if ( childIntervals[ i ]->getBegin()->getTime() >= end->getTime() )
      childIntervals[ i ]->calcPrev( displayList );

    if ( begin == NULL ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin != NULL )
        delete begin;
      begin = childIntervals[ i ]->getBegin()->clone();
    }

    info.values.push_back( childIntervals[ i ]->getValue() *
                           window->getFactor( i ) );
  }
  currentValue = function->execute( &info );

  return displayList;
}


TWindowLevel IntervalDerived::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalDerived::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalDerived::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalDerived::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalDerived::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalDerived::setChilds()
{
  KWindow *window1;
  KWindow *window2;
  TApplOrder tmpAppl;
  TTaskOrder tmpTask;
  TThreadOrder tmpThread;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  childIntervals.clear();

  if ( window->getParent( 0 )->getLevel() >= window->getParent( 1 )->getLevel() )
  {
    window1 = (KWindow *) window->getParent( 0 );
    window2 = (KWindow *) window->getParent( 1 );
  }
  else
  {
    window1 = (KWindow *) window->getParent( 1 );
    window2 = (KWindow *) window->getParent( 0 );
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

  childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order, true ) );

  if ( window2->getLevel() == WORKLOAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
  }
  else if ( window2->getLevel() == APPLICATION )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpAppl, true ) );
  }
  else if ( window2->getLevel() == TASK )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalTask( tmpAppl, tmpTask ), true ) );
  }
  else if ( window2->getLevel() == THREAD )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalThread( tmpAppl, tmpTask, tmpThread ), true ) );
  }
  else if ( window2->getLevel() == SYSTEM )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
  }
  else if ( window2->getLevel() == NODE )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpNode, true ) );
  }
  else if ( window2->getLevel() == CPU )
  {
    childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                              window2->getTrace()->getGlobalCPU( tmpNode, tmpCPU ), true ) );
  }
}
