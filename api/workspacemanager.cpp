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

#include "workspacemanager.h"
#include "paraverkernelexception.h"

#ifdef _WIN32
  #include <shlobj.h>
  #include <Shlwapi.h>

  #define MAX_LEN_PATH 2048
#else
  #include <sys/stat.h>
  #include <pwd.h>
  #include <sys/types.h>
#endif

#ifdef __APPLE__
  #include "CoreFoundation/CoreFoundation.h"
#endif



using std::string;
using std::vector;


WorkspaceManager *WorkspaceManager::instance = nullptr;


WorkspaceManager *WorkspaceManager::getInstance( KernelConnection *whichKernel )
{
  if ( WorkspaceManager::instance == nullptr )
    WorkspaceManager::instance = new WorkspaceManager( whichKernel );
  return WorkspaceManager::instance;
}


WorkspaceManager::WorkspaceManager( KernelConnection *whichKernel )
{
  myKernel = whichKernel;
}


WorkspaceManager::~WorkspaceManager()
{
}


void WorkspaceManager::clear()
{
  userWorkspaces.clear();
  userWorkspacesOrder.clear();
}


bool WorkspaceManager::existWorkspace( std::string name, TWorkspaceSet whichSet ) const
{
  switch ( whichSet )
  {
    case TWorkspaceSet::ALL:
      return ( distWorkspaces.find( name ) != distWorkspaces.end() ) ||
             ( userWorkspaces.find( name ) != userWorkspaces.end() );
      break;

    case TWorkspaceSet::DISTRIBUTED:
      return distWorkspaces.find( name ) != distWorkspaces.end();
      break;

    case TWorkspaceSet::USER_DEFINED:
      return userWorkspaces.find( name ) != userWorkspaces.end();
      break;

    default:
      throw ParaverKernelException();
      break;
  }

  return false;
}


vector<string> WorkspaceManager::getWorkspaces( TWorkspaceSet whichSet ) const
{
  vector< string > tmpWorkspacesOrder;

  switch ( whichSet )
  {
    case TWorkspaceSet::ALL:
      tmpWorkspacesOrder = distWorkspacesOrder;
      tmpWorkspacesOrder.insert( tmpWorkspacesOrder.end(), userWorkspacesOrder.begin(), userWorkspacesOrder.end() );
      return tmpWorkspacesOrder;
      break;

    case TWorkspaceSet::DISTRIBUTED:
      return distWorkspacesOrder;
      break;

    case TWorkspaceSet::USER_DEFINED:
      return userWorkspacesOrder;
      break;

    default:
      throw ParaverKernelException();
      break;
  }

  return vector< string >();
}


void WorkspaceManager::getMergedWorkspaces( const std::set<TState>& loadedStates,
                                            const std::set<TEventType>& loadedTypes,
                                            std::vector<std::string>& onWorkspaceVector,
                                            size_t& userDefined )
{
  vector<WorkspaceValue> tmpLoadedValues;
  for( std::set<TState>::const_iterator it = loadedStates.begin(); it != loadedStates.end(); ++it )
  {
    WorkspaceValue tmpWorkspaceValue;
    tmpWorkspaceValue.myType = WorkspaceType::STATE;
    tmpWorkspaceValue.UInfo.state = *it;
    tmpLoadedValues.push_back( tmpWorkspaceValue );
  }

  for( std::set<TEventType>::const_iterator it = loadedTypes.begin(); it != loadedTypes.end(); ++it )
  {
    WorkspaceValue tmpWorkspaceValue;
    tmpWorkspaceValue.myType = WorkspaceType::EVENT;
    tmpWorkspaceValue.UInfo.eventType = *it;
    tmpLoadedValues.push_back( tmpWorkspaceValue );
  }

  for( vector< string >::iterator it = distWorkspacesOrder.begin(); it != distWorkspacesOrder.end(); ++it )
  {
    vector< WorkspaceValue > tmpAutoTypes = distWorkspaces[ *it ].getAutoTypes();
    if ( find_first_of( tmpLoadedValues.begin(), tmpLoadedValues.end(),
                        tmpAutoTypes.begin(), tmpAutoTypes.end() ) !=  tmpLoadedValues.end() )
      onWorkspaceVector.push_back( *it );
  }
  userDefined = onWorkspaceVector.size();

  vector<string> tmpUserDefined;
  for( vector< string >::iterator it = userWorkspacesOrder.begin(); it != userWorkspacesOrder.end(); ++it )
  {
    vector< WorkspaceValue > tmpAutoTypes = userWorkspaces[ *it ].getAutoTypes();
    if( tmpAutoTypes.empty() )
      tmpUserDefined.push_back( *it );
    else if( find_first_of( tmpLoadedValues.begin(), tmpLoadedValues.end(),
                            tmpAutoTypes.begin(), tmpAutoTypes.end() ) !=  tmpLoadedValues.end() )
    {
      if( find( onWorkspaceVector.begin(), onWorkspaceVector.end(), *it ) != onWorkspaceVector.end() )
      {
        vector< WorkspaceValue > tmpDistAutoTypes = distWorkspaces[ *it ].getAutoTypes();
        sort( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end() );
        vector< WorkspaceValue > tmpUserAutoTypes = userWorkspaces[ *it ].getAutoTypes();
        sort( tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() );
        if( includes( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end(),
                      tmpUserAutoTypes.begin(), tmpUserAutoTypes.end() ) )
          continue;
      }

      tmpUserDefined.push_back( *it );
    }
  }

  onWorkspaceVector.insert( onWorkspaceVector.end(), tmpUserDefined.begin(), tmpUserDefined.end() );
}

Workspace& WorkspaceManager::getWorkspace( std::string name, TWorkspaceSet whichSet )
{
  switch ( whichSet )
  {
    case TWorkspaceSet::ALL:
      if ( existWorkspace( name, TWorkspaceSet::DISTRIBUTED ) )
        return distWorkspaces[ name ];
      else
        return userWorkspaces[ name ];
      break;

    case TWorkspaceSet::DISTRIBUTED:
      return distWorkspaces[ name ];
      break;

    case TWorkspaceSet::USER_DEFINED:
      return userWorkspaces[ name ];
      break;

    default:
      throw ParaverKernelException();
      break;
  }

  return distWorkspaces["0"];
}


void WorkspaceManager::addWorkspace( std::string whichName )
{
  userWorkspaces[ whichName ] = Workspace();
  userWorkspacesOrder.push_back( whichName );
}


void WorkspaceManager::addWorkspace( Workspace& whichWorkspace )
{
  userWorkspaces[ whichWorkspace.getName() ] = whichWorkspace;
  userWorkspacesOrder.push_back( whichWorkspace.getName() );
}


void WorkspaceManager::loadXML()
{
  string baseDir;
  string fullPath;

  // Read distributed
#ifdef _WIN32
  baseDir = getenv( "HOMEDRIVE" );
  baseDir.append( getenv( "HOMEPATH" ) );

  char myPath[ MAX_LEN_PATH ];
  HMODULE hModule = GetModuleHandle( nullptr );
  if ( hModule != nullptr )
  {
    GetModuleFileName( nullptr, myPath, ( sizeof( myPath ) ));
    PathRemoveFileSpec( myPath );
    string tmpParaverPath( myPath );
    baseDir = tmpParaverPath.substr( 0, tmpParaverPath.size() - 4 );
  }
#elif defined( __APPLE__ )
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char tmpPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)tmpPath, PATH_MAX))
    {
        throw ParaverKernelException();
    }
    CFRelease(resourcesURL);

    baseDir = tmpPath;
#else
  if( getenv( "PARAVER_HOME" ) == nullptr )
    baseDir = "";
  else
    baseDir = getenv( "PARAVER_HOME" );
#endif

  fullPath = baseDir;
#ifdef _WIN32
  fullPath.append( "\\share\\workspaces.xml" );
#else
  fullPath.append( "/share/workspaces.xml" );
#endif

  std::ifstream ifs( fullPath.c_str() );
  if( ifs.good() )
  {
    boost::archive::xml_iarchive ia( ifs );
    serializeBufferWorkspaces = &distWorkspaces;
    serializeBufferWorkspacesOrder = &distWorkspacesOrder;
    ia >> boost::serialization::make_nvp( "workspace_manager", *this );
  }
  ifs.close();

  // Read user defined
  ifs.clear();
  baseDir.clear();
  fullPath.clear();

#ifdef _WIN32
  baseDir = getenv( "HOMEDRIVE" );
  baseDir.append( getenv( "HOMEPATH" ) );
#else
  baseDir = getenv( "HOME" );
#endif
  fullPath.append( baseDir );
#ifdef _WIN32
  fullPath.append( "\\paraver\\workspaces" );
#else
  fullPath.append( "/.paraver/workspaces" );
#endif
  fullPath.append( ".xml" );

  ifs.open( fullPath.c_str() );
  if( !ifs.good() )
  {
    saveXML();
    ifs.open( fullPath.c_str() );
  }

  if( ifs.good() ) // due to xml_iarchive needs to be destroyed before fstream is closed
  {
    boost::archive::xml_iarchive ia( ifs );
    serializeBufferWorkspaces = &userWorkspaces;
    serializeBufferWorkspacesOrder = &userWorkspacesOrder;
    ia >> boost::serialization::make_nvp( "workspace_manager", *this );
  }
  ifs.close();
}


void WorkspaceManager::saveXML()
{
  string strFile;

  strFile.append( myKernel->getParaverUserDir() );

#ifdef _WIN32
  strFile.append( "\\workspaces" );
#else
  strFile.append( "/workspaces" );
#endif
  strFile.append( ".xml" );

  std::ofstream ofs( strFile.c_str() );
  if( ofs.good() ) // due to xml_oarchive needs to be destroyed before fstream is closed
  {
    boost::archive::xml_oarchive oa( ofs );
    serializeBufferWorkspaces = &userWorkspaces;
    serializeBufferWorkspacesOrder = &userWorkspacesOrder;
    oa << boost::serialization::make_nvp( "workspace_manager", *this );
  }
  ofs.close();
}



