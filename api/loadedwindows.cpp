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

LoadedWindows *LoadedWindows::instance = nullptr;

LoadedWindows::LoadedWindows()
{
  currentID = 0;
  currentHistoID = 0;
}

LoadedWindows *LoadedWindows::getInstance()
{
  if ( LoadedWindows::instance == nullptr )
    LoadedWindows::instance = new LoadedWindows();
  return LoadedWindows::instance;
}

LoadedWindows::~LoadedWindows()
{
  for ( map<TWindowID, Timeline *>::iterator it = windows.begin();
        it != windows.end(); ++it )
    delete ( *it ).second;
  for ( map<TWindowID, Histogram *>::iterator it = histograms.begin();
        it != histograms.end(); ++it )
    delete ( *it ).second;
}

TWindowID LoadedWindows::add( Timeline *whichWindow )
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

void LoadedWindows::eraseWindow( Timeline *whichWindow )
{
  map<TWindowID, Timeline *>::iterator it;
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

Timeline *LoadedWindows::getWindow( TWindowID id ) const
{
  return windows.find( id )->second;
}

Histogram *LoadedWindows::getHisto( TWindowID id ) const
{
  return histograms.find( id )->second;
}

void LoadedWindows::getAll( vector<Timeline *>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    onVector.push_back( ( *it ).second );
}

void LoadedWindows::getAll( vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    onVector.push_back( ( *it ).first );
}

void LoadedWindows::getAll( vector<Histogram *>& onVector ) const
{
  for ( map<TWindowID, Histogram *>::const_iterator it = histograms.begin();
        it != histograms.end(); ++it )
    onVector.push_back( ( *it ).second );
}


void LoadedWindows::getAll( Trace *whichTrace, vector< Timeline *>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
    if ( ( *it ).second->getTrace() == whichTrace )
      onVector.push_back( ( *it ).second );
}


void LoadedWindows::getDerivedCompatible( Trace *whichTrace, vector< Timeline *>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {  
    bool isProcessModel = ( *it ).second->isLevelProcessModel();
    if ( ( *it ).second->getTrace()->isSameObjectStruct( whichTrace, isProcessModel ) )
      onVector.push_back( ( *it ).second );
  }
}


void LoadedWindows::getDerivedCompatible( Trace *whichTrace, vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {
    bool isProcessModel = ( *it ).second->isLevelProcessModel();
    if ( ( *it ).second->getTrace()->isSameObjectStruct( whichTrace, isProcessModel ) )
      onVector.push_back( ( *it ).first );
  }
}


void LoadedWindows::getAll( Trace *whichTrace, vector< Histogram *>& onVector ) const
{
  for ( map<TWindowID, Histogram *>::const_iterator it = histograms.begin();
        it != histograms.end(); ++it )
    if ( ( *it ).second->getTrace() == whichTrace )
      onVector.push_back( ( *it ).second );
}


// Histogram windows selection related methods
void LoadedWindows::getValidControlWindow( Timeline *dataWindow,
    Timeline *controlWindow,
    vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {
    if ( LoadedWindows::validDataWindow( dataWindow, ( *it ).second )
         && LoadedWindows::validDataWindow( controlWindow, ( *it ).second ) )
      onVector.push_back( ( *it ).first );
  }
}

void LoadedWindows::getValidDataWindow( Timeline *controlWindow,
                                        Timeline *extraWindow,
                                        vector<TWindowID>& onVector ) const
{
  for ( map<TWindowID, Timeline *>::const_iterator it = windows.begin();
        it != windows.end(); ++it )
  {
    if ( LoadedWindows::validDataWindow( ( *it ).second, controlWindow )
         && LoadedWindows::validDataWindow( ( *it ).second, extraWindow ) )
      onVector.push_back( ( *it ).first );
  }
}

bool LoadedWindows::validDataWindow( Timeline *dataWindow, Timeline *controlWindow )
{
  if ( dataWindow == controlWindow )
    return true;
  if ( controlWindow == nullptr )
    return true;
  if ( dataWindow == nullptr )
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
    if( dataWindow->getTrace()->isSameObjectStruct( controlWindow->getTrace(), controlWindow->isLevelProcessModel() ) )
      if ( LoadedWindows::validLevelDataWindow( dataWindow, controlWindow ) )
      {
        return LoadedWindows::notInParents( dataWindow, controlWindow )
               && LoadedWindows::notInParents( controlWindow, dataWindow );
      }
  }

  return false;
}

bool LoadedWindows::validLevelDataWindow( Timeline *dataWindow, Timeline *controlWindow )
{
  return ( dataWindow->getLevel() == controlWindow->getLevel() )
         ||
         ( Timeline::compatibleLevels( dataWindow, controlWindow ) &&
           dataWindow->getLevel() < controlWindow->getLevel() );
}

bool LoadedWindows::notInParents( Timeline *whichWindow, Timeline *inParents )
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
