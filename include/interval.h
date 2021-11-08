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


#include <set>
#include "memorytrace.h"
#include "krecordlist.h"

class KTimeline;

class Interval
{
  public:
    Interval()
    {
      begin = nullptr;
      end = nullptr;
      currentValue = 0.0;
      notWindowInits = false;
    }

    Interval( TWindowLevel whichLevel, TObjectOrder whichOrder ):
        order( whichOrder )
    {
      begin = nullptr;
      end = nullptr;
      currentValue = 0.0;
      notWindowInits = false;
    }

    virtual ~Interval()
    {
      myDisplayList.clear();
    }

    virtual TRecordTime getBeginTime() const
    {
      return begin->getTime();
    }

    virtual TRecordTime getEndTime() const
    {
      return end->getTime();
    }

    virtual TSemanticValue getValue() const
    {
      return currentValue;
    }

    virtual MemoryTrace::iterator *getBegin() const
    {
      return begin;
    }

    virtual MemoryTrace::iterator *getEnd() const
    {
      return end;
    }

    virtual TWindowLevel getLevel() const = 0;

    TObjectOrder getOrder()
    {
      return order;
    }

    KRecordList *getRecordList()
    {
      return &myDisplayList;
    }

    bool getNotWindowInits() const
    {
      return notWindowInits;
    }

    void setNotWindowInits( bool whichValue )
    {
      notWindowInits = whichValue;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = nullptr ) = 0;
    virtual KRecordList *calcNext( KRecordList *displayList = nullptr, bool initCalc = false ) = 0;
    virtual KRecordList *calcPrev( KRecordList *displayList = nullptr, bool initCalc = false ) = 0;

    virtual KTimeline *getWindow() = 0;

  protected:
    TObjectOrder order;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    KRecordList myDisplayList;
    bool notWindowInits;

  private:

};



