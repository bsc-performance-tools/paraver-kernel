#include <fstream>
#include <sstream>
#include "trace.h"
#include "traceheaderexception.h"

using namespace std;

Trace::Trace( const string& whichFile ) : fileName( whichFile )
{
  string tmpstr;

  std::fstream file( fileName.c_str(), ios::out );

// Reading the header
  std::getline( file, tmpstr );
  istringstream header( tmpstr );

  std::getline( header, date, ':' );

  std::getline( header, tmpstr, ':' );
  size_t pos = tmpstr.find( '_' );
  if ( pos == string::npos )
  {
    // No '_' char found. The trace is in us.
    traceTimeUnit = US;
    istringstream stringEndTime( tmpstr );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTime,
                                  tmpstr.c_str() );
    }
  }
  else
  {
    // '_' char found. The trace is in ns.
    traceTimeUnit = NS;
    istringstream stringEndTime( tmpstr.substr( 0, pos ) );
    if ( !( stringEndTime >> traceEndTime ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTime,
                                  tmpstr.c_str() );
    }
  }

  traceResourceModel = ResourceModel( header );
  traceProcessModel = ProcessModel( header );

  // Communicators
  UINT32 numberComm;
  std::getline( header, tmpstr );
  istringstream streamComm( tmpstr );

  if ( !( streamComm >> numberComm ) )
  {
    throw TraceHeaderException( TraceHeaderException::invalidCommNumber,
                                tmpstr.c_str() );
  }

  for ( UINT32 count = 0; count < numberComm; count++ )
  {
    std::getline( file, tmpstr );
    if ( tmpstr[0] != 'C' && tmpstr[0] != 'c' )
    {
      throw TraceHeaderException( TraceHeaderException::unknownCommLine,
                                  tmpstr.c_str() );
    }
    communicators.push_back( tmpstr );
  }
  // End communicators

// End reading the header


  file.close();
}
