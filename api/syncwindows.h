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


#include <map>
#include "paraverkerneltypes.h"

typedef unsigned int TGroupId;

class Timeline;
class Histogram;

class SyncWindows
{
  public:
    static SyncWindows *getInstance();
    ~SyncWindows();

    bool addWindow( Timeline *whichWindow, TGroupId whichGroup = 0 );
    bool addWindow( Histogram *whichWindow, TGroupId whichGroup = 0 );
    void removeWindow( Timeline *whichWindow, TGroupId whichGroup = 0 );
    void removeWindow( Histogram *whichWindow, TGroupId whichGroup = 0 );
    void removeAllWindows( TGroupId whichGroup = 0 );
    void removeAllGroups();

    TGroupId newGroup();
    TGroupId getNumGroups() const;
    void getGroups( std::vector< TGroupId >& groups ) const;

    void broadcastTime( TGroupId whichGroup, Timeline *sendWindow, TTime beginTime, TTime endTime );
    void broadcastTime( TGroupId whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime );
    void getGroupTimes( TGroupId whichGroup, TTime& beginTime, TTime& endTime ) const;

  private:
    SyncWindows();

    static SyncWindows *instance;
    std::map<TGroupId, std::vector<Timeline *> > syncGroupsTimeline;
    std::map<TGroupId, std::vector<Histogram *> > syncGroupsHistogram;
    TGroupId lastNewGroup;
    bool removingAll;

    void broadcastTimeTimelines( TGroupId whichGroup, Timeline *sendWindow, TTime beginTime, TTime endTime );
    void broadcastTimeHistograms( TGroupId whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime );
};


