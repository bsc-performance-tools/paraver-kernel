/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                            ptools_common_files                            *
 *                   Performance tools common files library                  *
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

#ifndef EVENT_LIST_H_INCLUDED
#define EVENT_LIST_H_INCLUDED

#include <string>
#include <vector>
#include <map>
#include "ptools_prv_types.h"

using std::string;
using std::vector;
using std::map;

class EventDescription;

class EventList
{
  public:
    static EventList *getInstance();

    ~EventList();

    void init();

    bool insert( string          stringID,
                 bool            usedInExtrae,
                 prvEventType_t  mpitID,
                 prvEventType_t  whichType,
                 prvEventValue_t whichValue,
                 string          whichStrType,
                 string          whichStrValue,
                 bool            isChangingState,
                 prvState_t      whichStateTransition );

    bool insert( string            stringID,
                 prvEventType_t    mpitID,
                 EventDescription *whichEvent );

    EventDescription *getByStringID( string whichStringID ) const;
    EventDescription *getByMpitID( prvEventType_t whichMpitID ) const;
    EventDescription *getByTypeValue( prvEventType_t  whichType,
                                      prvEventValue_t whichValue ) const;

    void getUsed( vector<EventDescription *>& onVector ) const;

  protected:

  private:
    EventList();

    static EventList *instance;

    vector<EventDescription *> events;
    map<string, EventDescription *> stringMap;
    map<prvEventType_t, EventDescription *> mpitMap;
    map<prvEventType_t, map<prvEventValue_t, EventDescription *> > typeValueMap;
};


#endif // EVENT_LIST_H_INCLUDED
