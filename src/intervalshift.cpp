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

#include "intervalshift.h"
#include "kwindow.h"


IntervalShift *IntervalShift::clone() const
{
  IntervalShift *tmpClone = new IntervalShift();
  
  tmpClone->order = order;
  tmpClone->level = level;
  tmpClone->window = window;
  tmpClone->notWindowInits = notWindowInits;

  return tmpClone;
}

KRecordList *IntervalShift::init( TRecordTime initialTime, TCreateList create,
                                  KRecordList *displayList )
{
  TRecordTime myInitTime = 0.0;

  clearSemanticBuffer();

/*  if ( begin != nullptr )
  {
    delete begin;
    begin = nullptr;
  }
  if ( end != nullptr )
  {
    delete end;
    end = nullptr;
  }
*/
  childIntervals[ 0 ]->init( myInitTime, create, displayList );
  addSemanticBuffer();

  if( semanticShift > 0 )
  {
    for( PRV_INT16 i = 1; i <= semanticShift; ++i )
      calcNext( displayList );
  }

  return displayList;
}

KRecordList *IntervalShift::calcNext( KRecordList *displayList, bool initCalc )
{
  childIntervals[ 0 ]->calcNext( displayList, initCalc );
  addSemanticBuffer();

  if( semanticBuffer.size() > semanticShift + 1 )
    popSemanticBuffer();

  return displayList;
}


KRecordList *IntervalShift::calcPrev( KRecordList *displayList, bool initCalc )
{
  childIntervals[ 0 ]->calcPrev( displayList, initCalc );
  addSemanticBuffer();

  if( semanticBuffer.size() > semanticShift + 1 )
    popSemanticBuffer();

  return displayList;
}

TWindowLevel IntervalShift::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalShift::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalShift::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalShift::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalShift::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalShift::setChildren()
{
}

void IntervalShift::setChildInterval( Interval *whichInterval )
{
  childIntervals.clear();
  childIntervals.push_back( whichInterval );
}

void IntervalShift::setSemanticShift( PRV_INT16 whichShift )
{
  semanticShift = whichShift;
}

TRecordTime IntervalShift::getBeginTime() const
{
  return semanticBuffer.front().begin->getTime();
}

TRecordTime IntervalShift::getEndTime() const
{
  return semanticBuffer.front().end->getTime();
}

TSemanticValue IntervalShift::getValue() const
{
#warning TODO: Negative semantic shift pending / 0?
  return semanticBuffer.back().semanticValue; 
}

MemoryTrace::iterator *IntervalShift::getBegin() const
{
  return semanticBuffer.front().begin;
}

MemoryTrace::iterator *IntervalShift::getEnd() const
{
  return semanticBuffer.front().end;
}

void IntervalShift::popSemanticBuffer()
{
  IntervalShift::ShiftSemanticInfo tmpElem;

  if( semanticShift > 0 )
    tmpElem = semanticBuffer.front();
  else
    tmpElem = semanticBuffer.back();

  delete tmpElem.begin;
  delete tmpElem.end;

  if( semanticShift > 0 )
    semanticBuffer.pop_front();
  else
    semanticBuffer.pop_back();
}

void IntervalShift::clearSemanticBuffer()
{
  while ( !semanticBuffer.empty() )
    popSemanticBuffer();
}

void IntervalShift::addSemanticBuffer()
{
  ShiftSemanticInfo tmpInfo;

  tmpInfo.begin = childIntervals[ 0 ]->getBegin()->clone();
  tmpInfo.end = childIntervals[ 0 ]->getEnd()->clone();
  tmpInfo.semanticValue = childIntervals[ 0 ]->getValue();
  
  if( semanticShift > 0 )
    semanticBuffer.push_back( tmpInfo );
  else
    semanticBuffer.push_front( tmpInfo );
}
