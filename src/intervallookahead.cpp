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


#include <algorithm>

#include "intervallookahead.h"
#include "kwindow.h"


KRecordList *IntervalLookAhead::init( TRecordTime initialTime, TCreateList create,
                                      KRecordList *displayList )
{
  buffer.clear();
  validActiveTime = false;

  // The child starts where the reference interval begins (not at initialTime),
  // otherwise the pieces between both times would not be counted as active time
  childIntervals[ 0 ]->init( reference->getBegin()->getTime(), create, displayList );
  pushChild();

  return displayList;
}


KRecordList *IntervalLookAhead::calcNext( KRecordList *displayList, bool initCalc )
{
  buffer.pop_front();

  // The front of the buffer must always be the current piece
  // so if it was not read ahead we need to add it
  if( buffer.empty() )
  {
    childIntervals[ 0 ]->calcNext( displayList, initCalc );
    pushChild();
  }

  return displayList;
}

// Relies on the child's calcPrev
KRecordList *IntervalLookAhead::calcPrev( KRecordList *displayList, bool initCalc )
{
  dropReadAhead( displayList );

  childIntervals[ 0 ]->calcPrev( displayList, initCalc );
  buffer.clear();

  // The front of the buffer must always be the current piece
  // so after going back we need to add the new one
  pushChild();

  return displayList;
}


TRecordTime IntervalLookAhead::getActiveTime( KRecordList *displayList )
{
  TRecordTime referenceBegin = reference->getBegin()->getTime();
  TRecordTime referenceEnd   = reference->getEnd()->getTime();

  // In order to avoid recompute: if the interval didn't change, return the last one
  if( validActiveTime && referenceBegin == activeTimeBegin && referenceEnd == activeTimeEnd )
    return activeTime;

  Interval *child = childIntervals[ 0 ];
  activeTime = 0;
  // Adds the time the function piece is != 0, counting only the part of the piece that
  // falls inside the reference interval (max/min clip it to its limits).
  auto addActiveTime = [&]( TRecordTime pieceBegin, TRecordTime pieceEnd, TSemanticValue pieceValue )
  {
    pieceBegin = std::max( pieceBegin, referenceBegin );
    pieceEnd   = std::min( pieceEnd, referenceEnd );
    if( pieceValue != 0 && pieceEnd > pieceBegin )
      activeTime += pieceEnd - pieceBegin;
  };

  // If the reference interval begins before the current piece (calcPrev)
  // the pieces behind it are not in the buffer, so we need to visit them.
  // Drop the read-ahead, go back adding the active time of the pieces behind
  // and return to the current piece
  if( buffer.front().begin->getTime() > referenceBegin )
  {
    dropReadAhead( displayList );

    // Go back adding the active time of the pieces behind
    size_t piecesBehind = 0;
    while( child->getBegin()->getTime() > referenceBegin )
    {
      child->calcPrev( displayList );
      ++piecesBehind;
      addActiveTime( child->getBegin()->getTime(), child->getEnd()->getTime(), child->getValue() );
    }

    // Return to the current piece
    for( ; piecesBehind > 0; --piecesBehind )
      child->calcNext( displayList );
  }

  // Read the child ahead, pushing the next pieces into the buffer,
  // until the reference interval is covered (or the trace ends)
  TRecordTime traceEnd = child->getWindow()->getTrace()->getEndTime();
  while( child->getEnd()->getTime() < std::min( referenceEnd, traceEnd ) )
  {
    child->calcNext( displayList );
    pushChild();
  }

  // Finally compute the active time of the buffer pieces
  for( const TPiece& piece : buffer )
    addActiveTime( piece.begin->getTime(), piece.end->getTime(), piece.value );

  validActiveTime = true;
  activeTimeBegin = referenceBegin;
  activeTimeEnd   = referenceEnd;

  return activeTime;
}


void IntervalLookAhead::setIntervals( Interval *whichChild, Interval *whichReference )
{
  childIntervals.clear();
  childIntervals.push_back( whichChild );
  reference = whichReference;
}


// The child is at the last piece read ahead (buffer back):
// takes it back to the current piece (buffer front)
void IntervalLookAhead::dropReadAhead( KRecordList *displayList )
{
  while( buffer.size() > 1 )
  {
    childIntervals[ 0 ]->calcPrev( displayList );
    buffer.pop_back();
  }
}


// Saves a copy of the child's current piece,
// because the child will move when reading ahead
void IntervalLookAhead::pushChild()
{
  TPiece piece;
  piece.begin.reset( childIntervals[ 0 ]->getBegin()->clone() );
  piece.end.reset( childIntervals[ 0 ]->getEnd()->clone() );
  piece.value = childIntervals[ 0 ]->getValue();
  buffer.push_back( piece );
}


TRecordTime IntervalLookAhead::getBeginTime() const
{
  return buffer.front().begin->getTime();
}

TRecordTime IntervalLookAhead::getEndTime() const
{
  return buffer.front().end->getTime();
}

TSemanticValue IntervalLookAhead::getValue() const
{
  return buffer.front().value;
}

MemoryTrace::iterator *IntervalLookAhead::getBegin() const
{
  return buffer.front().begin.get();
}

MemoryTrace::iterator *IntervalLookAhead::getEnd() const
{
  return buffer.front().end.get();
}


TTraceLevel IntervalLookAhead::getWindowLevel() const
{
  return window->getLevel();
}

Interval *IntervalLookAhead::getWindowInterval( TWindowLevel whichLevel,
                                                TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}

bool IntervalLookAhead::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}

TWindowLevel IntervalLookAhead::getComposeLevel( TTraceLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}

KTrace *IntervalLookAhead::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}
