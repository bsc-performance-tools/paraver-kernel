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
#include <vector>
#include <map>
#include <set>
#include "kernelconnection.h"
#include "paraverkerneltypes.h"
#include "workspace.h"

enum class TWorkspaceSet { ALL = 0, DISTRIBUTED, USER_DEFINED };

class WorkspaceManager
{
  public:

    static WorkspaceManager *getInstance( KernelConnection *whichKernel );

    ~WorkspaceManager();

    void clear();

    bool existWorkspace( std::string name, TWorkspaceSet whichSet ) const;
    std::vector<std::string> getWorkspaces( TWorkspaceSet whichSet ) const;
    void getMergedWorkspaces( const std::set<TState>& loadedStates,
                              const std::set<TEventType>& loadedTypes,
                              std::vector<std::string>& onWorkspaceVector,
                              size_t& userDefined );
    Workspace& getWorkspace( std::string whichName, TWorkspaceSet whichSet  );
    void addWorkspace( std::string whichName );
    void addWorkspace( Workspace& whichWorkspace );
    void loadXML();
    void saveXML();

    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
      ar & boost::serialization::make_nvp( "workspaces", *serializeBufferWorkspaces  );
      ar & boost::serialization::make_nvp( "workspacesOrder", *serializeBufferWorkspacesOrder );
    }


  protected:

    WorkspaceManager( KernelConnection *whichKernel );

  private:
    static WorkspaceManager *instance;

    KernelConnection *myKernel;

    std::map<std::string, Workspace> distWorkspaces;
    std::vector<std::string> distWorkspacesOrder;
    std::map<std::string, Workspace> userWorkspaces;
    std::vector<std::string> userWorkspacesOrder;

    std::map<std::string, Workspace> *serializeBufferWorkspaces;
    std::vector<std::string> *serializeBufferWorkspacesOrder;
};

BOOST_CLASS_VERSION( WorkspaceManager, 0)


