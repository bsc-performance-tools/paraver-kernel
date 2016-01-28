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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "workspacemanager.h"
#include "paraverkernelexception.h"

#ifdef WIN32
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


WorkspaceManager *WorkspaceManager::instance = NULL;


WorkspaceManager *WorkspaceManager::getInstance()
{
  if ( WorkspaceManager::instance == NULL )
    WorkspaceManager::instance = new WorkspaceManager();
  return WorkspaceManager::instance;
}


WorkspaceManager::WorkspaceManager()
{
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
    case ALL:
      return ( distWorkspaces.find( name ) != distWorkspaces.end() ) ||
             ( userWorkspaces.find( name ) != userWorkspaces.end() );
      break;

    case DISTRIBUTED:
      return distWorkspaces.find( name ) != distWorkspaces.end();
      break;

    case USER_DEFINED:
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
    case ALL:
      tmpWorkspacesOrder = distWorkspacesOrder;
      tmpWorkspacesOrder.insert( tmpWorkspacesOrder.end(), userWorkspacesOrder.begin(), userWorkspacesOrder.end() );
      return tmpWorkspacesOrder;
      break;

    case DISTRIBUTED:
      return distWorkspacesOrder;
      break;

    case USER_DEFINED:
      return userWorkspacesOrder;
      break;

    default:
      throw ParaverKernelException();
      break;
  }

  return vector< string >();
}


void WorkspaceManager::getMergedWorkspaces( const std::set<TEventType>& loadedTypes,
                                            std::vector<std::string>& onWorkspaceVector,
                                            size_t& userDefined )
{
  for ( vector< string >::iterator it = distWorkspacesOrder.begin(); it != distWorkspacesOrder.end(); ++it )
  {
    vector< TEventType > tmpAutoTypes = distWorkspaces[ *it ].getAutoTypes();
    if ( find_first_of( loadedTypes.begin(), loadedTypes.end(),
                        tmpAutoTypes.begin(), tmpAutoTypes.end() ) !=  loadedTypes.end() )
      onWorkspaceVector.push_back( *it );
  }
  userDefined = onWorkspaceVector.size();

  vector<string> tmpUserDefined;
  for ( vector< string >::iterator it = userWorkspacesOrder.begin(); it != userWorkspacesOrder.end(); ++it )
  {
    vector< TEventType > tmpAutoTypes = userWorkspaces[ *it ].getAutoTypes();
    if( tmpAutoTypes.empty() )
      tmpUserDefined.push_back( *it );
    else if( find_first_of( loadedTypes.begin(), loadedTypes.end(),
                            tmpAutoTypes.begin(), tmpAutoTypes.end() ) !=  loadedTypes.end() )
    {
      if( find( onWorkspaceVector.begin(), onWorkspaceVector.end(), *it ) != onWorkspaceVector.end() )
      {
        vector< TEventType > tmpDistAutoTypes = distWorkspaces[ *it ].getAutoTypes();
        sort( tmpDistAutoTypes.begin(), tmpDistAutoTypes.end() );
        vector< TEventType > tmpUserAutoTypes = userWorkspaces[ *it ].getAutoTypes();
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
    case ALL:
      if ( existWorkspace( name, DISTRIBUTED ) )
        return distWorkspaces[ name ];
      else
        return userWorkspaces[ name ];
      break;

    case DISTRIBUTED:
      return distWorkspaces[ name ];
      break;

    case USER_DEFINED:
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
#ifdef WIN32
  baseDir = getenv( "HOMEDRIVE" );
  baseDir.append( getenv( "HOMEPATH" ) );

  WCHAR myPath[ MAX_LEN_PATH ];
  HMODULE hModule = GetModuleHandle( NULL );
  if ( hModule != NULL )
  {
    GetModuleFileName( NULL, myPath, ( sizeof( myPath ) ));
    PathRemoveFileSpec( myPath );
    char tmpMyPath[ MAX_LEN_PATH ];
    size_t tmpSize;
    wcstombs_s( &tmpSize, tmpMyPath, MAX_LEN_PATH, myPath, MAX_LEN_PATH );
    baseDir = tmpMyPath;
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
  baseDir = getenv( "PARAVER_HOME" );
#endif

  fullPath = baseDir;
#ifdef WIN32
  fullPath.append( "\\share\\workspaces" );
#else
  fullPath.append( "/share/workspaces" );
#endif
  fullPath.append( ".xml" );

  std::ifstream ifs( fullPath.c_str() );
  if( ifs.good() )
  {
    boost::archive::xml_iarchive ia( ifs );
    serializeBufferWorkspaces = &distWorkspaces;
    serializeBufferWorkspacesOrder = &distWorkspacesOrder;
    ia >> boost::serialization::make_nvp( "workspace_manager", *this );
    ifs.close();
  }

  // Read user defined
  ifs.clear();
  baseDir.clear();
  fullPath.clear();

#ifdef WIN32
  baseDir = getenv( "HOMEDRIVE" );
  baseDir.append( getenv( "HOMEPATH" ) );
#else
  baseDir = getenv( "HOME" );
#endif
  fullPath.append( baseDir );
#ifdef WIN32
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
  boost::archive::xml_iarchive ia( ifs );
  serializeBufferWorkspaces = &userWorkspaces;
  serializeBufferWorkspacesOrder = &userWorkspacesOrder;
  ia >> boost::serialization::make_nvp( "workspace_manager", *this );
  ifs.close();
}


void WorkspaceManager::saveXML()
{
  string homedir;
  string strFile;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );

#ifdef WIN32
  strFile.append( "\\paraver\\workspaces" );
  string tmpPath( homedir + "\\workspaces" );

  int len = tmpPath.length() + 1;
  wchar_t *wText = new wchar_t[len];
  memset(wText,0,len);
  ::MultiByteToWideChar( CP_ACP, NULL, tmpPath.c_str(), -1, wText, len );

  SHCreateDirectoryEx( NULL, wText, NULL );
  delete []wText;
#else
  strFile.append( "/.paraver/workspaces" );
  mkdir( ( homedir + "/.paraver" ).c_str(), (mode_t)0700 );
#endif
  strFile.append( ".xml" );

  std::ofstream ofs( strFile.c_str() );
  boost::archive::xml_oarchive oa( ofs );
  serializeBufferWorkspaces = &userWorkspaces;
  serializeBufferWorkspacesOrder = &userWorkspacesOrder;
  oa << boost::serialization::make_nvp( "workspace_manager", *this );
  ofs.close();
}



