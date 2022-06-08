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


#include "syncwindows.h"
#include "window.h"
#include "histogram.h"

using std::vector;
using std::map;

SyncWindows *SyncWindows::instance = nullptr;

SyncWindows *SyncWindows::getInstance()
{
  if( SyncWindows::instance == nullptr )
    SyncWindows::instance = new SyncWindows();
  return SyncWindows::instance;
}

SyncWindows::SyncWindows()
{
  lastNewGroup = 0;
  syncGroupsTimeline[ lastNewGroup ] = vector<Timeline *>();
  syncGroupsHistogram[ lastNewGroup ] = vector<Histogram *>();
  removingAll = false;
}

SyncWindows::~SyncWindows()
{
}

bool SyncWindows::addWindow( Timeline *whichWindow, TGroupId whichGroup )
{
  if( syncGroupsTimeline.find( whichGroup ) == syncGroupsTimeline.end() )
    return false;

  if( syncGroupsTimeline[ whichGroup ].size() > 0 || syncGroupsHistogram[ whichGroup ].size() > 0 )
  {
    TTime nanoBeginTime, nanoEndTime;
    getGroupTimes( whichGroup, nanoBeginTime, nanoEndTime );
    if( whichWindow->traceUnitsToCustomUnits( whichWindow->getWindowBeginTime(), NS ) != nanoBeginTime ||
        whichWindow->traceUnitsToCustomUnits( whichWindow->getWindowEndTime(), NS )   != nanoEndTime )
    {
      whichWindow->addZoom( nanoBeginTime, nanoEndTime, true );
      whichWindow->setWindowBeginTime( whichWindow->customUnitsToTraceUnits( nanoBeginTime, NS ), true );
      whichWindow->setWindowEndTime( whichWindow->customUnitsToTraceUnits( nanoEndTime, NS ), true );
      whichWindow->setChanged( true );
      whichWindow->setRedraw( true );
    }
  }
  syncGroupsTimeline[ whichGroup ].push_back( whichWindow );

  return true;
}

bool SyncWindows::addWindow( Histogram *whichWindow, TGroupId whichGroup )
{
  if( syncGroupsHistogram.find( whichGroup ) == syncGroupsHistogram.end() )
    return false;

  if( syncGroupsHistogram[ whichGroup ].size() > 0 || syncGroupsTimeline[ whichGroup ].size() > 0 )
  {
    TTime nanoBeginTime, nanoEndTime;
    getGroupTimes( whichGroup, nanoBeginTime, nanoEndTime );
    if( whichWindow->getControlWindow()->traceUnitsToCustomUnits( whichWindow->getBeginTime(), NS ) != nanoBeginTime ||
        whichWindow->getControlWindow()->traceUnitsToCustomUnits( whichWindow->getEndTime(), NS )   != nanoEndTime )
    {
      whichWindow->setWindowBeginTime( whichWindow->getControlWindow()->customUnitsToTraceUnits( nanoBeginTime, NS ), true );
      whichWindow->setWindowEndTime( whichWindow->getControlWindow()->customUnitsToTraceUnits( nanoEndTime, NS ), true );
      whichWindow->setChanged( true );
      whichWindow->setRecalc( true );
      whichWindow->setRedraw( true );
    }
  }
  syncGroupsHistogram[ whichGroup ].push_back( whichWindow );

  return true;
}

void SyncWindows::removeWindow( Timeline *whichWindow, TGroupId whichGroup )
{
  if( syncGroupsTimeline.find( whichGroup ) == syncGroupsTimeline.end() || removingAll )
    return;

  for( vector<Timeline *>::iterator it = syncGroupsTimeline[ whichGroup ].begin();
       it != syncGroupsTimeline[ whichGroup ].end(); ++it )
  {
    if( *it == whichWindow )
    {
      syncGroupsTimeline[ whichGroup ].erase( it );
      break;
    }
  }
}

void SyncWindows::removeWindow( Histogram *whichWindow, TGroupId whichGroup )
{
  if( syncGroupsHistogram.find( whichGroup ) == syncGroupsHistogram.end() || removingAll )
    return;

  for( vector<Histogram *>::iterator it = syncGroupsHistogram[ whichGroup ].begin();
       it != syncGroupsHistogram[ whichGroup ].end(); ++it )
  {
    if( *it == whichWindow )
    {
      syncGroupsHistogram[ whichGroup ].erase( it );
      break;
    }
  }
}

void SyncWindows::removeAllWindows( TGroupId whichGroup )
{
  if( syncGroupsTimeline.find( whichGroup ) == syncGroupsTimeline.end() )
    return;

  removingAll = true;

  for( vector<Timeline *>::iterator it = syncGroupsTimeline[ whichGroup ].begin();
       it != syncGroupsTimeline[ whichGroup ].end(); ++it )
    (*it)->removeFromSync();

  syncGroupsTimeline[ whichGroup ].clear();
  if( whichGroup != 0 )
    syncGroupsTimeline.erase( whichGroup );

  for( vector<Histogram *>::iterator it = syncGroupsHistogram[ whichGroup ].begin();
       it != syncGroupsHistogram[ whichGroup ].end(); ++it )
    (*it)->removeFromSync();

  syncGroupsHistogram[ whichGroup ].clear();
  if( whichGroup != 0 )
    syncGroupsHistogram.erase( whichGroup );

  removingAll = false;
}

int SyncWindows::getNumWindows( TGroupId whichGroup ) const
{
  int numGroups = 0;
  
  if( syncGroupsTimeline.find( whichGroup ) != syncGroupsTimeline.end() )
    numGroups += syncGroupsTimeline.find( whichGroup )->second.size() +
                 syncGroupsHistogram.find( whichGroup )->second.size();

  return numGroups;
}


void SyncWindows::removeAllGroups()
{
  for( std::map<TGroupId, std::vector<Timeline *> >::iterator it = syncGroupsTimeline.begin(); 
       it != syncGroupsTimeline.end(); ++it )
    removeAllWindows( it->first );
}

TGroupId SyncWindows::newGroup()
{
  for( size_t i = 0; i <= lastNewGroup; ++i )
  {
    map< TGroupId, std::vector< Timeline *> >::iterator it = syncGroupsTimeline.find( i );
    if( it == syncGroupsTimeline.end() )
    {
      syncGroupsTimeline[ i ]  = vector<Timeline *>();
      syncGroupsHistogram[ i ] = vector<Histogram *>();
      return i;
    }
    else if ( it->second.size() == 0 && syncGroupsHistogram[ it->first ].size() == 0 )
      return i;
  }

  ++lastNewGroup;
  syncGroupsTimeline[ lastNewGroup ]  = vector<Timeline *>();
  syncGroupsHistogram[ lastNewGroup ] = vector<Histogram *>();

  return lastNewGroup;
}

TGroupId SyncWindows::getNumGroups() const
{
  return syncGroupsTimeline.size();
}

void SyncWindows::getGroups( vector< TGroupId >& groups ) const
{
  for( std::map<TGroupId, std::vector<Timeline *> >::const_iterator it = syncGroupsTimeline.begin();
       it != syncGroupsTimeline.end(); ++it )
    groups.push_back( it->first );
}

void SyncWindows::broadcastTime( TGroupId whichGroup, Timeline *sendWindow, TTime beginTime, TTime endTime )
{
  if( syncGroupsTimeline.find( whichGroup ) == syncGroupsTimeline.end() )
    return;

  broadcastTimeTimelines( whichGroup, sendWindow, beginTime, endTime );
  broadcastTimeHistograms( whichGroup, nullptr, beginTime, endTime );
}

void SyncWindows::broadcastTime( TGroupId whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime )
{
  if( syncGroupsHistogram.find( whichGroup ) == syncGroupsHistogram.end() )
    return;

  broadcastTimeTimelines( whichGroup, nullptr, beginTime, endTime );
  broadcastTimeHistograms( whichGroup, sendWindow, beginTime, endTime );
}

void SyncWindows::broadcastTimeTimelines( TGroupId whichGroup, Timeline *sendWindow, TTime beginTime, TTime endTime )
{
  for( vector<Timeline *>::iterator it = syncGroupsTimeline[ whichGroup ].begin();
       it != syncGroupsTimeline[ whichGroup ].end(); ++it )
  {
    TTime tmpBeginTime, tmpEndTime;
    tmpBeginTime = ( *it )->customUnitsToTraceUnits( beginTime, NS );
    tmpEndTime = ( *it )->customUnitsToTraceUnits( endTime, NS );
    if( ( *it ) != sendWindow  &&
        ( ( *it )->getWindowBeginTime() != tmpBeginTime ||
          ( *it )->getWindowEndTime()   != tmpEndTime )
      )
    {
      ( *it )->addZoom( tmpBeginTime, tmpEndTime, true );
      ( *it )->setWindowBeginTime( tmpBeginTime, true );
      ( *it )->setWindowEndTime( tmpEndTime, true );
      ( *it )->setChanged( true );
      ( *it )->setRedraw( true );
    }
  }
}

void SyncWindows::broadcastTimeHistograms( TGroupId whichGroup, Histogram *sendWindow, TTime beginTime, TTime endTime )
{
  for( vector<Histogram *>::iterator it = syncGroupsHistogram[ whichGroup ].begin();
       it != syncGroupsHistogram[ whichGroup ].end(); ++it )
  {
    TTime tmpBeginTime, tmpEndTime;
    tmpBeginTime = ( *it )->getControlWindow()->customUnitsToTraceUnits( beginTime, NS );
    tmpEndTime = ( *it )->getControlWindow()->customUnitsToTraceUnits( endTime, NS );
    if( ( *it ) != sendWindow  &&
        ( ( *it )->getBeginTime() != tmpBeginTime ||
          ( *it )->getEndTime()   != tmpEndTime )
      )
    {
      ( *it )->setWindowBeginTime( tmpBeginTime, true );
      ( *it )->setWindowEndTime( tmpEndTime, true );
      ( *it )->setChanged( true );
      ( *it )->setRecalc( true );
    }
  }
}

void SyncWindows::getGroupTimes( TGroupId whichGroup, TTime& beginTime, TTime& endTime ) const
{
  if( syncGroupsTimeline.find( whichGroup ) == syncGroupsTimeline.end() )
    return;

  std::map<TGroupId, std::vector<Timeline *> >::const_iterator itTimeline = syncGroupsTimeline.find( whichGroup );
  if( (*itTimeline).second.size() > 0 )
  {
    beginTime = (*itTimeline).second[ 0 ]->traceUnitsToCustomUnits( (*itTimeline).second[ 0 ]->getWindowBeginTime(), NS );
    endTime   = (*itTimeline).second[ 0 ]->traceUnitsToCustomUnits( (*itTimeline).second[ 0 ]->getWindowEndTime(), NS );
    return;
  }

  std::map<TGroupId, std::vector<Histogram *> >::const_iterator itHistogram = syncGroupsHistogram.find( whichGroup );
  if( (*itHistogram).second.size() > 0 )
  {
    const Timeline *tmpControl = (*itHistogram).second[ 0 ]->getControlWindow();
    beginTime = tmpControl->traceUnitsToCustomUnits( (*itHistogram).second[ 0 ]->getBeginTime(), NS );
    endTime   = tmpControl->traceUnitsToCustomUnits( (*itHistogram).second[ 0 ]->getEndTime(), NS );
  }
}
