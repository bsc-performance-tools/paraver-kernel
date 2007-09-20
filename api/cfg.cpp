#include <fstream>
#include <sstream>

#include "cfg.h"

using namespace std;

bool loadCFG( string& filename, Trace *whichTrace, vector<KWindow *>& windows,
              TRecordTime& beginTime, TRecordTime& endTime )
{
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
  }

  cfgFile.close();

  if ( windows[ windows.size() -1 ] == NULL )
    return false;

  return true;
}
