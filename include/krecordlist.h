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
#include "recordlist.h"
#include "memorytrace.h"

class KTimeline;

class KRecordList: public RecordList
{
  public:
    KRecordList();
    virtual ~KRecordList();

    virtual void clear();
    virtual void erase( iterator first, iterator last );
    virtual iterator begin();
    virtual iterator end();
    virtual bool newRecords() const;

    // Specific for KRecordList
    virtual void insert( KTimeline *window, MemoryTrace::iterator *it );
    virtual RecordList *clone();

  protected:

  private:
    std::multiset<RLRecord, ltrecord> list;
    bool newRec;
};



