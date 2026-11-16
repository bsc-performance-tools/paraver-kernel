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


#include <deque>
#include <memory>
#include "intervalhigh.h"

class KTimeline;
class KDerivedWindow;

// Wraps a child interval and gives exactly the same intervals as the child,
// but it can read the child ahead (keeping what it reads in a buffer) to tell
// how much time the child is != 0 inside the current interval of a reference
// interval.
// Used by IntervalDerived for "distribute by time": child = function, reference = counter.
class IntervalLookAhead : public IntervalHigh
{
  public:
    IntervalLookAhead()
    {}

    IntervalLookAhead( KDerivedWindow *whichWindow, TWindowLevel whichLevel,
                       TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {}

    virtual ~IntervalLookAhead()
    {}

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                               KRecordList *displayList = nullptr ) override;
    virtual KRecordList *calcNext( KRecordList *displayList = nullptr, bool initCalc = false ) override;
    virtual KRecordList *calcPrev( KRecordList *displayList = nullptr, bool initCalc = false ) override;

    virtual KTimeline *getWindow() override
    {
      return ( KTimeline * ) window;
    }

    void setIntervals( Interval *whichChild, Interval *whichReference );

    // Time with child value != 0 inside the current reference interval
    TRecordTime getActiveTime( KRecordList *displayList );

    virtual TRecordTime getBeginTime() const override;
    virtual TRecordTime getEndTime() const override;
    virtual TSemanticValue getValue() const override;
    virtual MemoryTrace::iterator *getBegin() const override;
    virtual MemoryTrace::iterator *getEnd() const override;

  protected:
    virtual void setChildren() override
    {}

    virtual KTrace *getWindowTrace() const override;
    virtual TTraceLevel getWindowLevel() const override;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel, TObjectOrder whichOrder ) override;
    virtual bool IsDerivedWindow() const override;
    virtual TWindowLevel getComposeLevel( TTraceLevel whichLevel ) const override;

  private:
    struct TPiece
    {
      // IntervalDerived is copied (std::vector<IntervalDerived> in KDerivedWindow),
      // so we need to use shared_ptr since this class must be copyable
      std::shared_ptr< MemoryTrace::iterator > begin;
      std::shared_ptr< MemoryTrace::iterator > end;
      TSemanticValue value;
    };

    KDerivedWindow *window = nullptr;
    Interval *reference = nullptr;

    // front: current interval; back: where the child is now
    std::deque< TPiece > buffer;

    // Active time is computed once for every reference interval
    bool validActiveTime = false;
    TRecordTime activeTime = 0;
    TRecordTime activeTimeBegin = 0;
    TRecordTime activeTimeEnd = 0;

    void pushChild();
    void dropReadAhead( KRecordList *displayList );
};
