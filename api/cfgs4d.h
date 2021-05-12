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

#ifndef CFGS4D_H_INCLUDED
#define CFGS4D_H_INCLUDED

#include <string>
#include <map>
#include <set>
#include "paraverkerneltypes.h"

typedef PRV_UINT32 TCFGS4DGroup;
typedef PRV_UINT32 TCFGS4DIndexLink;
static const TCFGS4DIndexLink NO_INDEX_LINK = 0;

class Window;
class Histogram;

struct lessWinCompare
{
  bool operator()( Window *win1, Window *win2 ) const;
};

struct lessHistoCompare
{
  bool operator()( Histogram *histo1, Histogram *histo2 ) const;
};

typedef std::set<Window *, lessWinCompare> TWindowsSet;
typedef std::set<Histogram *, lessHistoCompare> THistogramsSet;

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

    void insertWindow( Window *whichWindow );
    void insertWindow( Histogram *whichHistogram );
    void removeWindow( Window *whichWindow );
    void removeWindow( Histogram *whichHistogram );
    
    void getWindowList( TWindowsSet& onSet ) const;
    void getWindowList( THistogramsSet& onSet ) const;
    
    bool existsWindow( Window *whichWindow ) const;
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
    {}

    ~CFGS4DLinkedPropertiesManager()
    {}
    
    void setCustomName( std::string originalName, std::string customName );
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
            tmpGroup = it;
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
    void removeLink( std::string originalName, T *whichWindow )
    {
      typename std::map< std::pair< T *, std::string >, TCFGS4DGroup >::iterator itGroup;
      if( findWindowPropertyToGroup( whichWindow, originalName, itGroup ) )
      {
        enabledProperties[ itGroup->second ].removeWindow( whichWindow );
        if( enabledProperties[ itGroup->second ].getListSize() == 0 )
          enabledProperties.erase( itGroup->second );
        propertyNameToGroup[ originalName ].erase( itGroup->second );
        removeWindowPropertyToGroup( itGroup );
      }
    }

    // TODO: getLinks using TCFGS4DGroup and maybe originalName+Window* also
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
    bool existsWindow( std::string originalName, T *whichWindow ) const
    {
      return propertyNameToGroup.find( originalName ) != propertyNameToGroup.end();
    }

    void getLinksName( std::set<std::string>& onSet ) const;

    size_t getLinksSize( const std::string originalName ) const;

  private:
    std::map< TCFGS4DGroup, CFGS4DPropertyWindowsList > enabledProperties;
    std::map< std::string, std::set< TCFGS4DGroup > > propertyNameToGroup;
    std::map< std::pair< Window *, std::string >, TCFGS4DGroup > windowPropertyToGroup;
    std::map< std::pair< Histogram *, std::string >, TCFGS4DGroup > histogramPropertyToGroup;

    TCFGS4DGroup groupCounter;

    void insertWindowPropertyToGroup( Window *whichWindow, std::string originalName, TCFGS4DGroup whichGroup )
    {
      windowPropertyToGroup[ make_pair( whichWindow, originalName ) ] = whichGroup;
    }

    void insertWindowPropertyToGroup( Histogram *whichWindow, std::string originalName, TCFGS4DGroup whichGroup )
    {
      histogramPropertyToGroup[ make_pair( whichWindow, originalName ) ] = whichGroup;
    }

    bool findWindowPropertyToGroup( Window *whichWindow,
                                    std::string originalName,
                                    std::map< std::pair< Window *, std::string >, TCFGS4DGroup >::iterator& onIterator )
    {
      onIterator = windowPropertyToGroup.find( make_pair( whichWindow, originalName ) );
      return onIterator != windowPropertyToGroup.end();
    }

    bool findWindowPropertyToGroup( Histogram *whichWindow,
                                    std::string originalName,
                                    std::map< std::pair< Histogram *, std::string >, TCFGS4DGroup >::iterator& onIterator )
    {
      onIterator = histogramPropertyToGroup.find( make_pair( whichWindow, originalName ) );
      return onIterator != histogramPropertyToGroup.end();
    }

    void removeWindowPropertyToGroup( std::map< std::pair< Window *, std::string >, TCFGS4DGroup >::iterator  whichGroup )
    {
      windowPropertyToGroup.erase( whichGroup );
    }
    
    void removeWindowPropertyToGroup( std::map< std::pair< Histogram *, std::string >, TCFGS4DGroup >::iterator  whichGroup )
    {
      histogramPropertyToGroup.erase( whichGroup );
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
    
    template< typename T >
    void insertLink( TCFGS4DIndexLink index, std::string originalName, T *whichWindow )
    { 
      cfgsLinkedProperties[ index ].insertLink( originalName, whichWindow );
    }


    template< typename T >
    void getLinks( TCFGS4DIndexLink index, std::string originalName, T& onSet ) const
    {
      std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager >::const_iterator it = cfgsLinkedProperties.find( index );
      if( it != cfgsLinkedProperties.end() )
        it->second.getLinks( originalName, onSet );
    }

  private:
    static CFGS4DGlobalManager *instance;
    
    std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager > cfgsLinkedProperties;
    
    TCFGS4DIndexLink linkCounter;

    CFGS4DGlobalManager();
};

#endif // CFGS4D_H_INCLUDED
