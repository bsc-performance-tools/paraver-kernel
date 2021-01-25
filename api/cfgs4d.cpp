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

#include "cfgs4d.h"

using std::string;
using std::map;
using std::set;

/*!
 * CFGS4DPropertyWindowsList Methods
 */

void CFGS4DPropertyWindowsList::setCustomName( string whichName )
{
  customName = whichName;
}

string CFGS4DPropertyWindowsList::getCustomName() const
{
  return customName;
}

void CFGS4DPropertyWindowsList::insertWindow( Window *whichWindow )
{
  windows.insert( whichWindow );
}

void CFGS4DPropertyWindowsList::insertWindow( Histogram *whichHistogram )
{
  histograms.insert( whichHistogram );
}

void CFGS4DPropertyWindowsList::removeWindow( Window *whichWindow )
{
  windows.erase( whichWindow );
}

void CFGS4DPropertyWindowsList::removeWindow( Histogram *whichHistogram )
{
  histograms.erase( whichHistogram );
}

void CFGS4DPropertyWindowsList::getWindowList( TWindowsSet& onSet ) const
{
  onSet = windows;
}

void CFGS4DPropertyWindowsList::getWindowList( THistogramsSet& onSet ) const
{
  onSet = histograms;
}

bool CFGS4DPropertyWindowsList::existsWindow( Window *whichWindow ) const
{
  return windows.find( whichWindow ) != windows.end();
}

bool CFGS4DPropertyWindowsList::existsWindow( Histogram *whichHistogram ) const
{
  return histograms.find( whichHistogram ) != histograms.end();
}

size_t CFGS4DPropertyWindowsList::getListSize() const
{
  return windows.size() + histograms.size();
}

/*
 * CFGS4DLinkedPropertiesManager Methods
 */

void CFGS4DLinkedPropertiesManager::setCustomName( std::string originalName, std::string customName )
{
  if( enabledProperties.find( originalName ) != enabledProperties.end() )
    enabledProperties[ originalName ].setCustomName( customName );
}

std::string CFGS4DLinkedPropertiesManager::getCustomName( std::string originalName ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( originalName );
  if ( it != enabledProperties.end() )
    return it->second.getCustomName();

  return "";
}

void CFGS4DLinkedPropertiesManager::insertLink( std::string originalName, Window *whichWindow )
{
  enabledProperties[ originalName ].insertWindow( whichWindow );
}

void CFGS4DLinkedPropertiesManager::insertLink( std::string originalName, Histogram *whichHistogram )
{
  enabledProperties[ originalName ].insertWindow( whichHistogram );
}

void CFGS4DLinkedPropertiesManager::removeLink( std::string originalName, Window *whichWindow )
{
  enabledProperties[ originalName ].removeWindow( whichWindow );
  if( enabledProperties[ originalName ].getListSize() == 0 )
    enabledProperties.erase( originalName );
}

void CFGS4DLinkedPropertiesManager::removeLink( std::string originalName, Histogram *whichHistogram )
{
  enabledProperties[ originalName ].removeWindow( whichHistogram );
  if( enabledProperties[ originalName ].getListSize() == 0 )
    enabledProperties.erase( originalName );
}

void CFGS4DLinkedPropertiesManager::getLinks( std::string whichName, TWindowsSet& onSet ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    it->second.getWindowList( onSet );  
}

void CFGS4DLinkedPropertiesManager::getLinks( std::string whichName, THistogramsSet& onSet ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    it->second.getWindowList( onSet );  
}

bool CFGS4DLinkedPropertiesManager::existsWindow( std::string whichName, Window *whichWindow ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    return it->second.existsWindow( whichWindow );
  return false;
}

bool CFGS4DLinkedPropertiesManager::existsWindow( std::string whichName, Histogram *whichHistogram ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    return it->second.existsWindow( whichHistogram ); 
  return false;
}

void CFGS4DLinkedPropertiesManager::getLinksName( std::set<std::string>& onSet ) const
{
  for( std::map< std::string, CFGS4DPropertyWindowsList >::const_iterator it = enabledProperties.begin();
       it != enabledProperties.end(); ++it )
  {
    onSet.insert( it->first );
  }
}

size_t CFGS4DLinkedPropertiesManager::getLinksSize( const std::string whichName ) const
{
  map<string, CFGS4DPropertyWindowsList>::const_iterator it = enabledProperties.find( whichName );
  if ( it != enabledProperties.end() )
    return  it->second.getListSize();

  return 0;
}

/*
 * CFGS4DGlobalManager Methods
 */
CFGS4DGlobalManager *CFGS4DGlobalManager::instance = NULL;

CFGS4DGlobalManager *CFGS4DGlobalManager::getInstance()
{
  if( instance == NULL )
    instance = new CFGS4DGlobalManager();
  return instance;
}
