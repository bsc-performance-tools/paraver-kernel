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


#include "intervalderived.h"
#include "kwindow.h"


IntervalDerived::IntervalDerived()
{
  function = nullptr;
}

IntervalDerived::IntervalDerived( KDerivedWindow *whichWindow,
                                  TWindowLevel whichLevel,
                                  TObjectOrder whichOrder ):
    IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
{
  function = nullptr;
  shift1 = IntervalShift( whichWindow, whichLevel, whichOrder );
  shift2 = IntervalShift( whichWindow, whichLevel, whichOrder );
}

IntervalDerived::~IntervalDerived()
{
  if ( begin != nullptr )
    delete begin;
  if ( end != nullptr )
    delete end;
}


KRecordList *IntervalDerived::init( TRecordTime initialTime, TCreateList create,
                                    KRecordList *displayList )
{
  TRecordTime myInitTime;
  info.values.clear();
  info.callingInterval = this;

  createList = create;
  currentValue = 0.0;

  if ( displayList == nullptr )
  {
    displayList = &myDisplayList;
    displayList->clear();
  }

  function = ( SemanticDerived * ) window->getSemanticFunction( level );

  setChildren();

  if ( begin != nullptr )
  {
    delete begin;
    begin = nullptr;
  }
  if ( end != nullptr )
  {
    delete end;
    end = nullptr;
  }

  if ( function->getInitFromBegin() )
    myInitTime = 0.0;
  else
    myInitTime = initialTime;

  info.callingInterval = this;

  for ( TObjectOrder i = 0; i < childIntervals.size(); ++i )
  {
    childIntervals[ i ]->init( myInitTime, createList, displayList );

    if ( begin == nullptr ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin == nullptr )
        begin = childIntervals[ i ]->getBegin()->clone();
      else
        *begin = *childIntervals[ i ]->getBegin();
    }

    if ( end == nullptr ||
         childIntervals[ i ]->getEnd()->getTime() < end->getTime() )
    {
      if ( end == nullptr )
        end = childIntervals[ i ]->getEnd()->clone();
      else
        *end = *childIntervals[ i ]->getEnd();
    }

    // Apply factor
    double tmpAbsFactor = fabs( window->getFactor( i ) );
    if ( tmpAbsFactor < 1.0 )
    {
      if ( window->getFactor( i ) < 0.0 )
        info.values.push_back( -childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
      else
        info.values.push_back( childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
    }
    else
      info.values.push_back( childIntervals[ i ]->getValue() * window->getFactor( i ) );
  }

  info.dataBeginTime = childIntervals[ 0 ]->getBegin()->getTime();
  currentValue = function->execute( &info );

  while ( end->getTime() < initialTime )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalDerived::calcNext( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

  if ( displayList == nullptr )
    displayList = &myDisplayList;

  *begin = *end;

  if ( end != nullptr )
  {
    delete end;
    end = nullptr;
  }

  for ( TObjectOrder i = 0; i < childIntervals.size(); ++i )
  {
    if ( childIntervals[ i ]->getEnd()->getTime() <= begin->getTime() )
      childIntervals[ i ]->calcNext( displayList );

    if ( ( end == nullptr
           && childIntervals[ i ]->getEnd()->getTime() < childIntervals[ i ]->getWindow()->getTrace()->getEndTime() )
         ||
         ( end != nullptr
           && childIntervals[ i ]->getEnd()->getTime() < end->getTime()
           && childIntervals[ i ]->getEnd()->getTime() < childIntervals[ i ]->getWindow()->getTrace()->getEndTime() )
        )
    {
      if ( end == nullptr )
        end = childIntervals[ i ]->getEnd()->clone();
      else
        *end = *childIntervals[ i ]->getEnd();
    }

    // Apply factor
    double tmpAbsFactor = fabs( window->getFactor( i ) );
    if ( tmpAbsFactor < 1.0 )
    {
      if ( window->getFactor( i ) < 0.0 )
        info.values.push_back( -childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
      else
        info.values.push_back( childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
    }
    else
      info.values.push_back( childIntervals[ i ]->getValue() * window->getFactor( i ) );
  }
  if( end == nullptr )
    end = childIntervals[ 0 ]->getTraceEnd();

  info.dataBeginTime = childIntervals[ 0 ]->getBegin()->getTime();
  currentValue = function->execute( &info );

  return displayList;
}


KRecordList *IntervalDerived::calcPrev( KRecordList *displayList, bool initCalc )
{
  info.values.clear();

  if ( displayList == nullptr )
    displayList = &myDisplayList;

  *end = *begin;

  if ( begin != nullptr )
  {
    delete begin;
    begin = nullptr;
  }

  for ( TObjectOrder i = 0; i < childIntervals.size(); i++ )
  {
    if ( childIntervals[ i ]->getBegin()->getTime() >= end->getTime() )
      childIntervals[ i ]->calcPrev( displayList );

    if ( begin == nullptr ||
         childIntervals[ i ]->getBegin()->getTime() > begin->getTime() )
    {
      if ( begin == nullptr )
        begin = childIntervals[ i ]->getBegin()->clone();
      else
        *begin = *childIntervals[ i ]->getBegin();
    }

    // Apply factor
    double tmpAbsFactor = fabs( window->getFactor( i ) );
    if ( tmpAbsFactor < 1.0 )
    {
      if ( window->getFactor( i ) < 0.0 )
        info.values.push_back( -childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
      else
        info.values.push_back( childIntervals[ i ]->getValue() / ( 1.0 / tmpAbsFactor ) );
    }
    else
      info.values.push_back( childIntervals[ i ]->getValue() * window->getFactor( i ) );
  }

  info.dataBeginTime = childIntervals[ 0 ]->getBegin()->getTime();
  currentValue = function->execute( &info );

  return displayList;
}


TTraceLevel IntervalDerived::getWindowLevel() const
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


TWindowLevel IntervalDerived::getComposeLevel( TTraceLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalDerived::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalDerived::setChildren()
{
  KTimeline *window1;
  KTimeline *window2;
  TApplOrder tmpAppl;
  TTaskOrder tmpTask;
  TThreadOrder tmpThread;
  TNodeOrder tmpNode;
  TCPUOrder tmpCPU;

  childIntervals.clear();

  if ( window->getParent( 0 )->getLevel() >= window->getParent( 1 )->getLevel() )
  {
    shift1.setSemanticShift( window->getShift( 0 ) );
    shift2.setSemanticShift( window->getShift( 1 ) );
    window1 = (KTimeline *) window->getParent( 0 );
    window2 = (KTimeline *) window->getParent( 1 );
  }
  else
  {
    shift1.setSemanticShift( window->getShift( 1 ) );
    shift2.setSemanticShift( window->getShift( 0 ) );
    window1 = (KTimeline *) window->getParent( 1 );
    window2 = (KTimeline *) window->getParent( 0 );
  }

  if ( window1->getLevel() == TTraceLevel::APPLICATION )
  {
    tmpAppl = order;
  }
  else if ( window1->getLevel() == TTraceLevel::TASK )
  {
    window1->getTrace()->getTaskLocation( order, tmpAppl, tmpTask );
  }
  else if ( window1->getLevel() == TTraceLevel::THREAD )
  {
    window1->getTrace()->getThreadLocation( order, tmpAppl, tmpTask, tmpThread );
  }
  else if ( window1->getLevel() == TTraceLevel::NODE )
  {
    tmpNode = order;
  }
  else if ( window1->getLevel() == TTraceLevel::CPU )
  {
    window1->getTrace()->getCPULocation( order, tmpNode, tmpCPU );
  }

  if( window->getShift( 0 ) != 0 )
  {
    shift1.setChildInterval( window1->getLevelInterval( TOPCOMPOSE1, order, true ) );
    childIntervals.push_back( &shift1 );
  }
  else
    childIntervals.push_back( window1->getLevelInterval( TOPCOMPOSE1, order, true ) );

  if ( window2->getLevel() == TTraceLevel::WORKLOAD )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::APPLICATION )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1, tmpAppl, true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpAppl, true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::TASK )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1,
                                                          window2->getTrace()->getGlobalTask( tmpAppl, tmpTask ),
                                                          true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                                window2->getTrace()->getGlobalTask( tmpAppl, tmpTask ), true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::THREAD )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1,
                                                          window2->getTrace()->getGlobalThread( tmpAppl, tmpTask, tmpThread ),
                                                          true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                                                           window2->getTrace()->getGlobalThread( tmpAppl, tmpTask, tmpThread ),
                                                           true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::SYSTEM )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, 0, true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::NODE )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1, tmpNode, true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1, tmpNode, true ) );
  }
  else if ( window2->getLevel() == TTraceLevel::CPU )
  {
    if( window->getShift( 1 ) != 0 )
    {
      shift2.setChildInterval( window2->getLevelInterval( TOPCOMPOSE1,
                                                          window2->getTrace()->getGlobalCPU( tmpNode, tmpCPU ),
                                                          true ) );
      childIntervals.push_back( &shift2 );
    }
    else
      childIntervals.push_back( window2->getLevelInterval( TOPCOMPOSE1,
                                                           window2->getTrace()->getGlobalCPU( tmpNode, tmpCPU ),
                                                           true ) );
  }
}
