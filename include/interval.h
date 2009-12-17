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

#ifndef INTERVAL_H_INCLUDED
#define INTERVAL_H_INCLUDED

#include <set>
#include "memorytrace.h"
#include "krecordlist.h"

class KWindow;

using namespace std;

class Interval
{
  public:
    Interval()
    {
      begin = NULL;
      end = NULL;
      currentValue = 0.0;
    }

    Interval( TWindowLevel whichLevel, TObjectOrder whichOrder ):
        level( whichLevel ), order( whichOrder )
    {
      begin = NULL;
      end = NULL;
      currentValue = 0.0;
    }

    virtual ~Interval()
    {
      myDisplayList.clear();
    }

    TRecordTime getBeginTime() const
    {
      return begin->getTime();
    }

    TRecordTime getEndTime() const
    {
      return end->getTime();
    }

    TSemanticValue getValue() const
    {
      return currentValue;
    }

    MemoryTrace::iterator *getBegin() const
    {
      return begin;
    }

    MemoryTrace::iterator *getEnd() const
    {
      return end;
    }

    TWindowLevel getLevel()
    {
      return level;
    }

    TObjectOrder getOrder()
    {
      return order;
    }

    KRecordList *getRecordList()
    {
      return &myDisplayList;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL ) = 0;
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false ) = 0;
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false ) = 0;

    virtual KWindow *getWindow() = 0;

  protected:
    TWindowLevel level;
    TObjectOrder order;
    MemoryTrace::iterator *begin;
    MemoryTrace::iterator *end;
    TSemanticValue currentValue;
    KRecordList myDisplayList;
  private:

};


#endif // INTERVAL_H_INCLUDED
