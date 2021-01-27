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
//#include "window.h"
//#include "histogram.h"

typedef PRV_UINT32 TCFGS4DIndexLink;

class Window;
class Histogram;

struct lessWinCompare
{
  bool operator()( Window *win1, Window *win2 ) const
  {
    return win1->getName() < win2->getName();
  }
};

struct lessHistoCompare
{
  bool operator()( Histogram *histo1, Histogram *histo2 ) const
  {
    return histo1->getName() < histo2->getName();
  }
};

typedef std::set<Window *, lessWinCompare> TWindowsSet;
typedef std::set<Histogram *, lessHistoCompare> THistogramsSet;

class CFGS4DPropertyWindowsList
{
  public:

    CFGS4DPropertyWindowsList()
    {}

    ~CFGS4DPropertyWindowsList()
    {}
    
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

    void insertLink( std::string originalName, Window *whichWindow );
    void insertLink( std::string originalName, Histogram *whichHistogram );
    void removeLink( std::string originalName, Window *whichWindow );
    void removeLink( std::string originalName, Histogram *whichHistogram );

    void getLinks( std::string whichName, TWindowsSet& onSet ) const;
    void getLinks( std::string whichName, THistogramsSet& onSet ) const;

    bool existsWindow( std::string whichName, Window *whichWindow ) const;
    bool existsWindow( std::string whichName, Histogram *whichHistogram ) const;

    void getLinksName( std::set<std::string>& onSet ) const;
    
    size_t getLinksSize( const std::string whichName ) const;

  private:
    std::map< std::string, CFGS4DPropertyWindowsList > enabledProperties;
};


class CFGS4DGlobalManager
{
  public:
    static CFGS4DGlobalManager *getInstance();

    ~CFGS4DGlobalManager()
    {}

    TCFGS4DIndexLink newLinkManager(); 
    void setCustomName( TCFGS4DIndexLink index, std::string originalName, std::string customName );
    void insertLink( TCFGS4DIndexLink index, std::string originalName, Window *whichWindow );
    void insertLink( TCFGS4DIndexLink index, std::string originalName, Histogram *whichHistogram );

  private:
    static CFGS4DGlobalManager *instance;
    
    std::map< TCFGS4DIndexLink, CFGS4DLinkedPropertiesManager > cfgsLinkedProperties;
    
    TCFGS4DIndexLink linkCounter;

    CFGS4DGlobalManager();
};

#endif // CFGS4D_H_INCLUDED
