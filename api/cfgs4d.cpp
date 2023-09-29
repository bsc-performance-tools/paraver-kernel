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
#include "window.h"
#include "histogram.h"

using std::string;
using std::map;
using std::set;

/*!
 * CFGS4DPropertyWindowsList Methods
 */

void CFGS4DPropertyWindowsList::setOriginalNameGroup( bool isOriginal )
{
  originalNameGroup = isOriginal;
}

bool CFGS4DPropertyWindowsList::isOriginalNameGroup() const
{
  return originalNameGroup;
}

void CFGS4DPropertyWindowsList::setCustomName( string whichName )
{
  customName = whichName;
}

string CFGS4DPropertyWindowsList::getCustomName() const
{
  return customName;
}

void CFGS4DPropertyWindowsList::insertWindow( Timeline *whichWindow )
{
  windows.insert( whichWindow );
}

void CFGS4DPropertyWindowsList::insertWindow( Histogram *whichHistogram )
{
  histograms.insert( whichHistogram );
}

void CFGS4DPropertyWindowsList::removeWindow( Timeline *whichWindow )
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

bool CFGS4DPropertyWindowsList::existsWindow( Timeline *whichWindow ) const
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
  auto itGroup = propertyNameToGroup.find( originalName );
  if ( itGroup != propertyNameToGroup.end() )
  {
    for ( auto it : itGroup->second )
    {
      if ( enabledProperties[ it ].isOriginalNameGroup() )
      {
        enabledProperties[ it ].setCustomName( customName );
        break;
      }
    }
  }
}


void CFGS4DLinkedPropertiesManager::setCustomName( TCFGS4DGroup whichGroup, std::string customName )
{
  enabledProperties[ whichGroup ].setCustomName( customName );
}


// TODO: getCustomName using TCFGS4DGroup and maybe originalName+Timeline * also
std::string CFGS4DLinkedPropertiesManager::getCustomName( std::string originalName ) const
{
  auto itGroup = propertyNameToGroup.find( originalName );
  if ( itGroup != propertyNameToGroup.end() )
  {
    for ( auto it : itGroup->second )
    {
      auto itProperty = enabledProperties.find( it );
      if ( itProperty != enabledProperties.end() && itProperty->second.isOriginalNameGroup() )
        return itProperty->second.getCustomName();
    }
  }

  return "";
}

void CFGS4DLinkedPropertiesManager::getLinksName( std::set<std::string>& onSet ) const
{
  for( auto it : propertyNameToGroup )
    onSet.insert( it.first );
}

size_t CFGS4DLinkedPropertiesManager::getLinksSize( const std::string originalName ) const
{
  auto itGroup = propertyNameToGroup.find( originalName );
  if ( itGroup != propertyNameToGroup.end() )
  {
    for ( auto it : itGroup->second )
    {
      auto itProperty = enabledProperties.find( it );
      if ( itProperty != enabledProperties.end() && itProperty->second.isOriginalNameGroup() )
        return itProperty->second.getListSize();
    }
  }

  return 0;
}

/*
 * CFGS4DGlobalManager Methods
 */
CFGS4DGlobalManager *CFGS4DGlobalManager::instance = nullptr;

CFGS4DGlobalManager *CFGS4DGlobalManager::getInstance()
{
  if( CFGS4DGlobalManager::instance == nullptr )
    CFGS4DGlobalManager::instance = new CFGS4DGlobalManager();
  return CFGS4DGlobalManager::instance;
}


CFGS4DGlobalManager::CFGS4DGlobalManager()
{
  linkCounter = 0;
}


TCFGS4DIndexLink CFGS4DGlobalManager::newLinkManager()
{
  ++linkCounter;
  cfgsLinkedProperties[ linkCounter ] = CFGS4DLinkedPropertiesManager();
  return linkCounter;
}


void CFGS4DGlobalManager::setCustomName( TCFGS4DIndexLink index, std::string originalName, std::string customName )
{
  cfgsLinkedProperties[ index ].setCustomName( originalName, customName );
}


void CFGS4DGlobalManager::setCustomName( TCFGS4DIndexLink index, TCFGS4DGroup whichGroup, std::string customName )
{
  cfgsLinkedProperties[ index ].setCustomName( whichGroup, customName );
}
