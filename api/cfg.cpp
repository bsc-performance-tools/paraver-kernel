#include <fstream>
#include <sstream>

#include "cfg.h"

using namespace std;

bool CFGLoader::mapLoaded = false;
map<string, TagFunction *> CFGLoader::cfgTagFunctions;

bool CFGLoader::loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
                         TRecordTime& beginTime, TRecordTime& endTime )
{
  if ( !mapLoaded )
    loadMap();

  ifstream cfgFile( filename.c_str() );
  string strLine;

  windows.push_back( NULL );

  while ( !cfgFile.eof() )
  {
    string cfgTag;

    getline( cfgFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, cfgTag, ' ' );


    cout << cfgTag << endl;

    map<string, TagFunction *>::iterator it = cfgTagFunctions.find( cfgTag );

    if ( it != cfgTagFunctions.end() )
    {
      it->second->parseLine( auxStream, whichTrace, windows,
                             beginTime, endTime );
    }
  }

  cfgFile.close();

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  return true;
}


void CFGLoader::loadMap()
{
  cfgTagFunctions["window_type"] = new WindowType();
  /*  cfgTagFunctions["window_identifiers"]   = new WindowId();
    cfgTagFunctions["window_factors"]   = new Window();
    cfgTagFunctions["window_operation"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();
    cfgTagFunctions["window_"]   = new Window();*/

  mapLoaded = true;
}


bool WindowType::parseLine( istringstream& line, Trace *whichTrace,
                            vector<KWindow *>& windows,
                            TRecordTime& beginTime, TRecordTime& endTime )
{
  string type;
  KWindow *tmpWin;

  getline( line, type, ' ' );
  if ( type.compare( "single" ) == 0 )
  {
    tmpWin = new KSingleWindow( whichTrace );
  }
  else if ( type.compare( "composed" ) == 0 )
  {
    tmpWin = new KDerivedWindow( );
  }
  else
    return false;

  if ( windows.size() == 1 && windows[ 0 ] == NULL )
    windows[ 0 ] = tmpWin;
  else
    windows.push_back( tmpWin );

  return true;
}
