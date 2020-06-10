/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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


#include "loadedwindows.h"
#include "window.h"
#include "histogram.h"

using namespace std;

LoadedWindows *LoadedWindows::instance = NULL;

LoadedWindows::LoadedWindows()
{
  currentID = 0;
  currentHistoID = 0;
}

LoadedWindows *LoadedWindows::getInstance()
{
  if ( LoadedWindows::instance == NULL )
    LoadedWindows::instance = new LoadedWindows();
  return LoadedWindows::instance;
}

LoadedWindows::~LoadedWindows()
{
  for ( map<TWindowID, Window *>::iterator it = windows.begin();
        it != windows.end(); ++it )
    delete ( *it ).second;
  for ( map<TWindowID, Histogram *>::iterator it = histograms.begin();
        it != histograms.end(); ++it )
    delete ( *it ).second;
}

TWindowID LoadedWindows::add( Window *whichWindow )
{
  currentID++;
  windows[ currentID ] = whichWindow;

  return currentID;
}

TWindowID LoadedWindows::add( Histogram *whichHisto )
{
  currentHistoID++;
  histograms[ currentHistoID ] = whichHisto;

  return currentHistoID;
}

void LoadedWindows::eraseWindow( TWindowID id )
{
  windows.erase( id );
}

void LoadedWindows::eraseWindow( Window *whichWindow )
{
  map<TWindowID, Window *>::iterator it;
  for (  it = windows.begin(); it != windows.end(); ++it )
    if ( it->second == whichWindow )
      break;

  if ( it != windows.end() )
    windows.erase( it );
}

void LoadedWindows::eraseHisto( TWindowID id )
{
  histograms.erase( id );
}

void LoadedWindows::eraseHisto( Histogram *whichHisto )
{
  map<TWindowID, Histogram *>::iterator it;
  for ( it = histograms.begin(); it != histograms.end(); ++it )
    if ( it->second == whichHisto )
      break;

  if ( it != histograms.end() )
    histograms.erase( it );
}

Window *LoadedWindows::getWindow( TWindowID id ) const
{
  return windows.find( id )->second;
}

Histogram *LoadedWindows::getHisto( TWindowID id ) const
{
  return histograms.find( id )->second;
}

void LoadedWindows::getAll( vector<Window *>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    onVector.push_back( ( *it ).second );
}

void LoadedWindows::getAll( vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    onVector.push_back( ( *it ).first );
}

void LoadedWindows::getAll( vector<Histogram *>& onVector ) const
{
  for ( map<TWindowID, Histogram *>::const_iterator it = histograms.begin();
        it != histograms.end(); ++it )
    onVector.push_back( ( *it ).second );
}


void LoadedWindows::getAll( Trace *whichTrace, vector< Window *>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    if ( ( *it ).second->getTrace() == whichTrace )
      onVector.push_back( ( *it ).second );
}


void LoadedWindows::getDerivedCompatible( Trace *whichTrace, vector< Window *>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    if ( ( *it ).second->getTrace()->isSameObjectStruct( whichTrace ) )
      onVector.push_back( ( *it ).second );
}


void LoadedWindows::getDerivedCompatible( Trace *whichTrace, vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    if ( ( *it ).second->getTrace()->isSameObjectStruct( whichTrace ) )
      onVector.push_back( ( *it ).first );
}


void LoadedWindows::getAll( Trace *whichTrace, vector< Histogram *>& onVector ) const
{
  for ( map<TWindowID, Histogram *>::const_iterator it = histograms.begin();
        it != histograms.end(); ++it )
    if ( ( *it ).second->getTrace() == whichTrace )
      onVector.push_back( ( *it ).second );
}


// Histogram windows selection related methods
void LoadedWindows::getValidControlWindow( Window *dataWindow,
    Window *controlWindow,
    vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {
    if ( LoadedWindows::validDataWindow( dataWindow, ( *it ).second )
         && LoadedWindows::validDataWindow( controlWindow, ( *it ).second ) )
      onVector.push_back( ( *it ).first );
  }
}

void LoadedWindows::getValidDataWindow( Window *controlWindow,
                                        Window *extraWindow,
                                        vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {
    if ( LoadedWindows::validDataWindow( ( *it ).second, controlWindow )
         && LoadedWindows::validDataWindow( ( *it ).second, extraWindow ) )
      onVector.push_back( ( *it ).first );
  }
}

bool LoadedWindows::validDataWindow( Window *dataWindow, Window *controlWindow )
{
  if ( dataWindow == controlWindow )
    return true;
  if ( controlWindow == NULL )
    return true;
  if ( dataWindow == NULL )
    return true;

  else if ( dataWindow->getTrace() == controlWindow->getTrace() )
  {
    if ( LoadedWindows::validLevelDataWindow( dataWindow, controlWindow ) )
    {
      return LoadedWindows::notInParents( dataWindow, controlWindow )
             && LoadedWindows::notInParents( controlWindow, dataWindow );
    }
  }
  else
  {
    // TODO -->
    if( dataWindow->getTrace()->isSameObjectStruct( controlWindow->getTrace() ) )
      if ( LoadedWindows::validLevelDataWindow( dataWindow, controlWindow ) )
      {
        return LoadedWindows::notInParents( dataWindow, controlWindow )
               && LoadedWindows::notInParents( controlWindow, dataWindow );
      }
  }

  return false;
}

bool LoadedWindows::validLevelDataWindow( Window *dataWindow, Window *controlWindow )
{
  if ( dataWindow->getLevel() == controlWindow->getLevel() )
    return true;
  else
  {
    if ( dataWindow->getLevel() >= WORKLOAD && dataWindow->getLevel() <= THREAD
         &&
         controlWindow->getLevel() >= WORKLOAD && controlWindow->getLevel() <= THREAD )
    {
      if ( dataWindow->getLevel() < controlWindow->getLevel() )
        return true;
    }
    else if ( dataWindow->getLevel() >= SYSTEM && dataWindow->getLevel() <= CPU
              &&
              controlWindow->getLevel() >= SYSTEM && controlWindow->getLevel() <= CPU )
    {
      if ( dataWindow->getLevel() < controlWindow->getLevel() )
        return true;
    }
  }
  return false;
}

bool LoadedWindows::notInParents( Window *whichWindow, Window *inParents )
{
  bool result = true;

  if ( whichWindow == inParents )
    result = false;
  else if ( inParents->isDerivedWindow() )
  {
    result = LoadedWindows::notInParents( whichWindow, inParents->getParent( 0 ) );
    if ( result )
      result = LoadedWindows::notInParents( whichWindow, inParents->getParent( 1 ) );
  }

  return result;
}

bool LoadedWindows::emptyWindows() const
{
  return windows.empty();
}

bool LoadedWindows::emptyHistograms() const
{
  return histograms.empty();
}
