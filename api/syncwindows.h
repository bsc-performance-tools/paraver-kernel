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


#ifndef SYNCWINDOWS_H_INCLUDED
#define SYNCWINDOWS_H_INCLUDED

#include <map>
#include "paraverkerneltypes.h"

class Window;
class Histogram;

class SyncWindows
{
  public:
    static SyncWindows *getInstance();
    ~SyncWindows();

    bool addWindow( Window *whichWindow, unsigned int whichGroup = 0 );
    bool addWindow( Histogram *whichWindow, unsigned int whichGroup = 0 );
    void removeWindow( Window *whichWindow, unsigned int whichGroup = 0 );
    void removeWindow( Histogram *whichWindow, unsigned int whichGroup = 0 );
    void removeAll( unsigned int whichGroup = 0 );

    unsigned int newGroup();
    unsigned int getNumGroups() const;
    void getGroups( std::vector<unsigned int>& groups ) const;

    void broadcastTime( unsigned int whichGroup, Window *sendWindow, TTime beginTime, TTime endTime );
    void broadcastTime( unsigned int whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime );
    void getGroupTimes( unsigned int whichGroup, TTime& beginTime, TTime& endTime ) const;

  private:
    SyncWindows();

    static SyncWindows *instance;
    std::map<unsigned int, std::vector<Window *> > syncGroupsTimeline;
    std::map<unsigned int, std::vector<Histogram *> > syncGroupsHistogram;
    unsigned int lastNewGroup;
    bool removingAll;

    void broadcastTimeTimelines( unsigned int whichGroup, Window *sendWindow, TTime beginTime, TTime endTime );
    void broadcastTimeHistograms( unsigned int whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime );
};

#endif // SYNCWINDOWS_H_INCLUDED
