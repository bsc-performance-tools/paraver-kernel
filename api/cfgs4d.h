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

#pragma once


#include <string>
#include <map>
#include <set>
#include "paraverkerneltypes.h"

typedef PRV_UINT32 TCFGS4DGroup;
static const TCFGS4DGroup NO_GROUP_LINK = 0;
typedef PRV_UINT32 TCFGS4DIndexLink;
static const TCFGS4DIndexLink NO_INDEX_LINK = 0;

class Timeline;
class Histogram;

template<typename TWindow>
struct lessWinCompare
{
  bool operator()( TWindow *leftWin, TWindow *rightWin ) const
  {
    if( leftWin->getName() == rightWin->getName() )
      return leftWin < rightWin;

    return leftWin->getName() < rightWin->getName();
  }
};

using TWindowsSet = std::set<Timeline *, lessWinCompare<Timeline> >;
using THistogramsSet = std::set<Histogram *, lessWinCompare<Histogram> >;

class CFGS4DPropertyWindowsList
{
  public:

    CFGS4DPropertyWindowsList()
    {
      originalNameGroup = true;
    }

    ~CFGS4DPropertyWindowsList()
    {}
    
    void setOriginalNameGroup( bool isOriginal );
    bool isOriginalNameGroup() const;
    void setCustomName( std::string whichName );
    std::string getCustomName() const;

    void insertWindow( Timeline *whichWindow );
    void insertWindow( Histogram *whichHistogram );
    void removeWindow( Timeline *whichWindow );
    void removeWindow( Histogram *whichHistogram );
    
    void getWindowList( TWindowsSet& onSet ) const;
    void getWindowList( THistogramsSet& onSet ) const;
    
    bool existsWindow( Timeline *whichWindow ) const;
    bool existsWindow( Histogram *whichHistogram ) const;
    
    size_t getListSize() const;
    
  private:
    bool originalNameGroup;
    std::string customName;
    TWindowsSet windows;
    THistogramsSet histograms;
};


class CFGS4DLinkedPropertiesManager
{
  public:
    CFGS4DLinkedPropertiesManager()
    {
      groupCounter = 0;
    }

    ~CFGS4DLinkedPropertiesManager()
    {}
    
    void setCustomName( std::string originalName, std::string customName );
    void setCustomName( TCFGS4DGroup whichGroup, std::string customName );
    std::string getCustomName( std::string originalName ) const;

    template< typename T >
    void insertLink( std::string originalName, T *whichWindow )
    {
      TCFGS4DGroup tmpGroup;
      auto itGroup = propertyNameToGroup.find( originalName );
      if( itGroup != propertyNameToGroup.end() )
      {
        for( auto it : itGroup->second )
        {
          if( enabledProperties[ it ].isOriginalNameGroup() )
          {
            tmpGroup = it;
            break;
          }
        }
      }
      else
      {
        tmpGroup = ++groupCounter;
      }

      enabledProperties[ tmpGroup ].insertWindow( whichWindow );
      propertyNameToGroup[ originalName ].insert( tmpGroup );
      insertWindowPropertyToGroup( whichWindow, originalName, tmpGroup );
    }

    template< typename T >
    void insertLink( TCFGS4DGroup whichGroup, std::string originalName, T *whichWindow )
    {
      enabledProperties[ whichGroup ].insertWindow( whichWindow );
      propertyNameToGroup[ originalName ].insert( whichGroup );
      insertWindowPropertyToGroup( whichWindow, originalName, whichGroup );
    }
    
    template< typename T >
    void removeLink( std::string originalName, T *whichWindow )
    {
      std::map< std::string, TCFGS4DGroup >::iterator itGroup;
      if( findWindowPropertyToGroup( whichWindow, originalName, itGroup ) )
      {
        enabledProperties[ itGroup->second ].removeWindow( whichWindow );
        if( enabledProperties[ itGroup->second ].getListSize() == 0 )
        {
          enabledProperties.erase( itGroup->second );
          propertyNameToGroup[ originalName ].erase( itGroup->second );
          if( propertyNameToGroup[ originalName ].empty() )
            propertyNameToGroup.erase( originalName );
        }
        removeWindowPropertyToGroup( whichWindow, itGroup );
      }
    }

    template< typename T >
    void getLinks( std::string originalName, T& onSet ) const
    {
      auto itGroup = propertyNameToGroup.find( originalName );
      if ( itGroup != propertyNameToGroup.end() )
      {
        for( auto it : itGroup->second )
        {
          auto itProperty = enabledProperties.find( it );
          if ( itProperty != enabledProperties.end() && itProperty->second.isOriginalNameGroup() )
          {
            itProperty->second.getWindowList( onSet );
            break;
          }
        }
      }
    }

    template< typename T >
    void getLinks( TCFGS4DGroup whichGroup, T& onSet ) const
    {
      auto itGroup = enabledProperties.find( whichGroup );
      if ( itGroup != enabledProperties.end() )
        itGroup->second.getWindowList( onSet );
    }

    template< typename T >
    bool existsWindow( std::string originalName, T *whichWindow ) const
    {
      return propertyNameToGroup.find( originalName ) != propertyNameToGroup.end();
    }

    void getLinksName( std::set<std::string>& onSet ) const;
    
    template< typename T >
    void getLinksName( const T *window, std::set<std::string>& onSet ) const
    {
      getWindowLinkNames( window, onSet );
    }
    
    template< typename T >
    TCFGS4DGroup getGroup( const T *window, const std::string originalName ) const
    {
      return getPropertyGroup( window, originalName );
    }

    size_t getLinksSize( const std::string originalName ) const;

  private:
    // Map with the list of windows sharing a linked property (numeric key)
    std::map< TCFGS4DGroup, CFGS4DPropertyWindowsList > enabledProperties;
    
    // Map with the list of groups for each property (original name string is the key)
    std::map< std::string, std::set< TCFGS4DGroup > > propertyNameToGroup;

    // Map containing the numeric key for every combination of Timeline+property (original name string)
    std::map< Timeline *, std::map< std::string, TCFGS4DGroup > > windowPropertyToGroup;
    
    // Map containing the numeric key for every combination of Histogram+property (original name string)
    std::map< Histogram *, std::map< std::string, TCFGS4DGroup > > histogramPropertyToGroup;

    TCFGS4DGroup groupCounter;


    void insertWindowPropertyToGroup( Timeline *whichWindow, std::string originalName, TCFGS4DGroup whichGroup )
    {
      windowPropertyToGroup[ whichWindow ][ originalName ] = whichGroup;
    }


    void insertWindowPropertyToGroup( Histogram *whichWindow, std::string originalName, TCFGS4DGroup whichGroup )
    {
      histogramPropertyToGroup[ whichWindow ][ originalName ] = whichGroup;
    }


    void getWindowLinkNames( const Timeline *window, std::set<std::string>& onSet ) const
    {
      auto itWindow = windowPropertyToGroup.find( const_cast<Timeline *>( window ) );
      if ( itWindow != windowPropertyToGroup.end() )
      {
        for ( auto it: itWindow->second )
        {
          onSet.insert( it.first );
        }
      }
    }


    void getWindowLinkNames( const Histogram *window, std::set<std::string>& onSet ) const
    {
      auto itHistogram = histogramPropertyToGroup.find( const_cast<Histogram *>( window ) );
      if ( itHistogram != histogramPropertyToGroup.end() )
      {
        for ( auto it: itHistogram->second )
        {
          onSet.insert( it.first );
        }
      }
    }


    TCFGS4DGroup getPropertyGroup( const Timeline *window, const std::string originalName ) const
    {
      auto itWindow = windowPropertyToGroup.find( const_cast<Timeline *>( window ) );
      if ( itWindow != windowPropertyToGroup.end() )
      {
        auto itProperty = itWindow->second.find( originalName );
        if( itProperty != itWindow->second.end() )
          return itProperty->second;
      }

      return 0;
    }


    TCFGS4DGroup getPropertyGroup( const Histogram *window, const std::string originalName ) const
    {
      auto itWindow = histogramPropertyToGroup.find( const_cast<Histogram *>( window ) );
      if ( itWindow != histogramPropertyToGroup.end() )
      {
        auto itProperty = itWindow->second.find( originalName );
        if( itProperty != itWindow->second.end() )
          return itProperty->second;
      }

      return 0;
    }


    bool findWindowPropertyToGroup( Timeline *whichWindow,
                                    std::string originalName,
                                    std::map< std::string, TCFGS4DGroup >::iterator& onGroup )
    {
      std::map< Timeline *, std::map< std::string, TCFGS4DGroup > >::iterator it;
      
      it = windowPropertyToGroup.find( whichWindow );
      if ( it != windowPropertyToGroup.end() )
      {
        onGroup = it->second.find( originalName );
        return onGroup != it->second.end();
      } 

      return false;
    }


    bool findWindowPropertyToGroup( Histogram *whichWindow,
                                    std::string originalName,
                                    std::map< std::string, TCFGS4DGroup >::iterator& onGroup )
    {
      std::map< Histogram *, std::map< std::string, TCFGS4DGroup > >::iterator it;

      it = histogramPropertyToGroup.find( whichWindow );
      if ( it != histogramPropertyToGroup.end() ) 
      { 
        onGroup = it->second.find( originalName );
        return onGroup != it->second.end();
      }

      return false;
    }


    void removeWindowPropertyToGroup( Timeline *whichWindow, std::map< std::string, TCFGS4DGroup >::iterator& whichGroup )
    {
      windowPropertyToGroup[ whichWindow ].erase( whichGroup );
    }


    void removeWindowPropertyToGroup( Histogram *whichWindow, std::map< std::string, TCFGS4DGroup >::iterator& whichGroup )
    {
      histogramPropertyToGroup[ whichWindow ].erase( whichGroup );
    }

};


class CFGS4DGlobalManager
{
  public:
    static CFGS4DGlobalManager *getInstance();

    ~CFGS4DGlobalManager()
    {}

    TCFGS4DIndexLink newLinkManager(); 
    void setCustomName( TCFGS4DIndexLink index, std::string originalName, std::string customName );
    void setCustomName( TCFGS4DIndexLink index, TCFGS4DGroup whichGroup, std::string customName );

    template< typename T >
    void insertLink( TCFGS4DIndexLink index, std::string originalName, T *whichWindow )
    { 
      cfgsLinkedProperties[ index ].insertLink( originalName, whichWindow );
    }

    template< typename T >
    void insertLink( TCFGS4DIndexLink index, TCFGS4DGroup whichGroup, std::string originalName, T *whichWindow )
    { 
      cfgsLinkedProperties[ index ].insertLink( whichGroup, originalName, whichWindow );
    }

    template< typename T >
    void getLinks( TCFGS4DIndexLink index, std::string originalName, T& onSet ) const
    {
      std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager >::const_iterator it = cfgsLinkedProperties.find( index );
      if( it != cfgsLinkedProperties.end() )
        it->second.getLinks( originalName, onSet );
    }

    template< typename T >
    void getLinks( TCFGS4DIndexLink index, TCFGS4DGroup whichGroup, T& onSet ) const
    {
      std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager >::const_iterator it = cfgsLinkedProperties.find( index );
      if( it != cfgsLinkedProperties.end() )
        it->second.getLinks( whichGroup, onSet );
    }

  private:
    static CFGS4DGlobalManager *instance;
    
    std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager > cfgsLinkedProperties;
    
    TCFGS4DIndexLink linkCounter;

    CFGS4DGlobalManager();
};


