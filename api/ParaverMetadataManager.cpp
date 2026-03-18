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

#include "ParaverMetadataManager.h"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#ifdef _WIN32
#  include <cctype>
#  include <functional>
#  include <locale>
#endif

#include "cfg.h"

using namespace std;

class cepba_tools
{
  public:
    ~cepba_tools();

    static std::string& ltrim( std::string& s );
    static std::string& rtrim( std::string& s );
    static std::string& trim( std::string& s );
};

// trim from start
std::string& cepba_tools::ltrim( std::string& s )
{
  auto tmpf = []( int c )
  {
    return !std::isspace( c );
  };
  s.erase( s.begin(), std::find_if( s.begin(), s.end(), tmpf ) );
  return s;
}

// trim from end
std::string& cepba_tools::rtrim( std::string& s )
{
  auto tmpf = []( int c )
  {
    return !std::isspace( c );
  };
  s.erase( std::find_if( s.rbegin(), s.rend(), tmpf ).base(), s.end() );
  return s;
}

// trim from both ends
std::string& cepba_tools::trim( std::string& s )
{
  return ltrim( rtrim( s ) );
}


/******************************************************************************
 ***                      MetadataAction                                    ***
 ******************************************************************************/
constexpr char MetadataAction::RUNAPP_ORIGIN_ID[];
constexpr char MetadataAction::MANUAL_ORIGIN_ID[];

MetadataAction::MetadataAction( string date, string action, string application, string originalTrace )
  : date( date ), action( action ), origin( application ), originalTrace( originalTrace )
{
}

void MetadataAction::write( ostream& os ) const
{
  os << "#" << date << ":" << action << ":" << origin << ":" << originalTrace << ":";

  flushSpecificFields( os );
}

ostream& operator<<( ostream& os, const MetadataAction& metadataRecord )
{
  metadataRecord.write( os );
  return os;
}


/******************************************************************************
 ***                      CutterMetadataAction                              ***
 ******************************************************************************/
constexpr char CutterMetadataAction::ACTION_ID[];

CutterMetadataAction::CutterMetadataAction( string date,
                                            string origin,
                                            string originalTrace,
                                            PRV_UINT64 offset,
                                            PRV_UINT64 beginTime,
                                            PRV_UINT64 endTime )
  : MetadataAction( date, CutterMetadataAction::ACTION_ID, origin, originalTrace )
{
  this->offset    = offset;
  this->beginTime = beginTime;
  this->endTime   = endTime;
}

CutterMetadataAction::CutterMetadataAction( vector< string >& cutterMetadataFields )
{
  if( cutterMetadataFields.size() != FIELD_COUNT )
  {
    ostringstream errorMessageSStr;
    errorMessageSStr << "wrong number of fields value in cutter metadata record (";
    errorMessageSStr << cutterMetadataFields.size() << " read, ";
    errorMessageSStr << FIELD_COUNT << " expected)";

    error        = true;
    errorMessage = errorMessageSStr.str();

    return;
  }

  istringstream converter;

  this->date          = cutterMetadataFields[ 0 ];
  this->action        = cutterMetadataFields[ 1 ];
  this->origin        = cutterMetadataFields[ 2 ];
  this->originalTrace = cutterMetadataFields[ 3 ];

  /* FIELD 5: Offset */
  converter.clear();
  converter.str( cutterMetadataFields[ 4 ] );
  if( !( converter >> this->offset ) )
  {
    ostringstream errorMessageSStr;
    errorMessageSStr << "wrong offset value in cutter metadata record (";
    errorMessageSStr << cutterMetadataFields[ 4 ] << ")";

    error        = true;
    errorMessage = errorMessageSStr.str();

    return;
  }

  /* FIELD 6: BeginTime */
  converter.clear();
  converter.str( cutterMetadataFields[ 5 ] );
  if( !( converter >> this->beginTime ) )
  {
    ostringstream errorMessageSStr;
    errorMessageSStr << "wrong cut begin time value in cutter metadata record (";
    errorMessageSStr << cutterMetadataFields[ 5 ] << ")";

    error        = true;
    errorMessage = errorMessageSStr.str();

    return;
  }

  /* FIELD 7: EndTime */
  converter.clear();
  converter.str( cutterMetadataFields[ 6 ] );
  if( !( converter >> this->endTime ) )
  {
    ostringstream errorMessageSStr;
    errorMessageSStr << "wrong cut end time value in cutter metadata record (";
    errorMessageSStr << cutterMetadataFields[ 6 ] << ")";

    error        = true;
    errorMessage = errorMessageSStr.str();

    return;
  }
}

void CutterMetadataAction::flushSpecificFields( ostream& os ) const
{
  os << offset << ":" << beginTime << ":" << endTime;
}


/******************************************************************************
 ***                      CSVMetadataAction                                 ***
 ******************************************************************************/
constexpr char CSVMetadataAction::ACTION_ID[];

CSVMetadataAction::CSVMetadataAction( string date,
                                      string origin,
                                      string originalTrace,
                                      TTimeUnit timeUnits,
                                      TColorFunction colorMode,
                                      TSemanticValue semanticMin,
                                      TSemanticValue semanticMax )
  : MetadataAction( date, CSVMetadataAction::ACTION_ID, origin, originalTrace )
{
  this->timeUnits   = timeUnits;
  this->colorMode   = colorMode;
  this->semanticMin = semanticMin;
  this->semanticMax = semanticMax;
}

CSVMetadataAction::CSVMetadataAction( vector< string >& csvMetadataFields )
{
  if( csvMetadataFields.size() != FIELD_COUNT )
  {
    ostringstream errorMessageSStr;
    errorMessageSStr << "wrong number of fields value in carm metadata record (";
    errorMessageSStr << csvMetadataFields.size() << " read, ";
    errorMessageSStr << FIELD_COUNT << " expected)";

    error        = true;
    errorMessage = errorMessageSStr.str() + "\n";
  }

  this->date          = csvMetadataFields[ 0 ];
  this->action        = csvMetadataFields[ 1 ];
  this->origin        = csvMetadataFields[ 2 ];
  this->originalTrace = csvMetadataFields[ 3 ];

  this->timeUnits = CFGLoader::parseTimeUnit( csvMetadataFields[ 4 ] );
  this->colorMode = CFGLoader::parseColorMode( csvMetadataFields[ 5 ] );

  TSemanticValue tmpValue;
  stringstream auxSStr;
  locale mylocale( "C" );
  auxSStr.imbue( mylocale );
  auxSStr.str( csvMetadataFields[ 6 ] );
  if( auxSStr >> tmpValue )
    this->semanticMin = tmpValue;
  else
  {
    error = true;
    errorMessage += "Semantic Minimum value can't be converted to double.";
  }

  auxSStr.clear();
  auxSStr.str( csvMetadataFields[ 7 ] );
  if( auxSStr >> tmpValue )
    this->semanticMax = tmpValue;
  else
  {
    error = true;
    errorMessage += "Semantic Maximum value can't be converted to double.";
  }
}

void CSVMetadataAction::flushSpecificFields( ostream& os ) const
{
  CFGLoader::dumpTimeUnits( timeUnits, os );
  os << ":";
  CFGLoader::dumpColorMode( colorMode, os );
  os << std::fixed;
  os << ":" << semanticMin << ":" << semanticMax;
}


/******************************************************************************
 ***                      CutterMetadata                                    ***
 ******************************************************************************/
bool CutterMetadata::newMetadata( std::vector< std::string > metadataFields )
{
  CutterMetadataAction newCutterAction( metadataFields );

  if( newCutterAction.getError() )
  {
    // error = true;
    // errorMessage = newCutterMetadata->getErrorMessage();


    return false;
  }

  totalOffset += newCutterAction.getOffset();

  metadataActions.push_back( newCutterAction );

  return true;
}

PRV_UINT64 CutterMetadata::getCutterLastOffset() const
{
  if( metadataActions.empty() )
    return 0;

  return metadataActions.back().getOffset();
}

PRV_UINT64 CutterMetadata::getCutterLastBeginTime() const
{
  if( metadataActions.empty() )
    return 0;

  return metadataActions.back().getBeginTime();
}

PRV_UINT64 CutterMetadata::getCutterLastEndTime() const
{
  if( metadataActions.empty() )
    return 0;

  return metadataActions.back().getEndTime();
}

PRV_UINT64 CutterMetadata::getCutterTotalOffset() const
{
  return totalOffset;
}


/******************************************************************************
 ***                      CSVMetadata                                       ***
 ******************************************************************************/
bool CSVMetadata::newMetadata( std::vector< std::string > metadataFields )
{
  CSVMetadataAction newCSVAction( metadataFields );

  if( newCSVAction.getError() )
  {
    // error = true;
    // errorMessage = newCutterMetadata->getErrorMessage();

    return false;
  }

  metadataActions.push_back( newCSVAction );

  return true;
}

TTimeUnit CSVMetadata::getTimeUnits() const
{
  if( metadataActions.empty() )
    return 0;

  return metadataActions.back().getTimeUnits();
}

TColorFunction CSVMetadata::getColorMode() const
{
  if( metadataActions.empty() )
    return {};

  return metadataActions.back().getColorMode();
}

TSemanticValue CSVMetadata::getSemanticMin() const
{
  if( metadataActions.empty() )
    return 0.0;

  return metadataActions.back().getSemanticMin();
}

TSemanticValue CSVMetadata::getSemanticMax() const
{
  if( metadataActions.empty() )
    return 0.0;

  return metadataActions.back().getSemanticMax();
}


/******************************************************************************
 ***                      MetadataManager                                   ***
 ******************************************************************************/
bool MetadataManager::newMetadata( string metadataStr )
{
  vector< string > metadataFields;

  populateRecord( metadataFields, metadataStr, ':' );

  if( metadataFields.size() < MetadataAction::FIELD_COUNT )
  {
    ostringstream errorMessageSStr;
    error = true;

    errorMessageSStr << "wrong metadata field (" << metadataFields.size() << " ";
    errorMessageSStr << "fields read, " << MetadataAction::FIELD_COUNT << " minimum)";
    errorMessage = errorMessageSStr.str();
    return false;
  }

  /*
   * Metadata Common Fields
   * FIELD 0: Date
   * FIELD 1: Action
   * FIELD 2: Application
   * FIELD 3: Original Trace
   */

  if( metadataFields[ 1 ].compare( CutterMetadataAction::ACTION_ID ) == 0 )
  {
    auto itCutter = metadataContainer.find( TAppID::CUTTER );
    if( itCutter == metadataContainer.end() )
      std::tie( itCutter, std::ignore ) = metadataContainer.insert( { TAppID::CUTTER, new CutterMetadata() } );

    if( !itCutter->second->newMetadata( metadataFields ) )
    {
      return false;
    }
  }
  else if( metadataFields[ 1 ].compare( CSVMetadataAction::ACTION_ID ) == 0 )
  {
    auto itCSV = metadataContainer.find( TAppID::CSV );
    if( itCSV == metadataContainer.end() )
      std::tie( itCSV, std::ignore ) = metadataContainer.insert( { TAppID::CSV, new CSVMetadata() } );

    if( !itCSV->second->newMetadata( metadataFields ) )
    {
      return false;
    }
  }

  return true;
}


bool MetadataManager::getError() const
{
  return error;
}


std::string MetadataManager::getErrorMessage() const
{
  return errorMessage;
}


string MetadataManager::getCurrentDate()
{
  stringstream currentDate;

  time_t t       = time( 0 ); // get time now
  struct tm* now = localtime( &t );

  currentDate << ( now->tm_year + 1900 );
  currentDate << std::setw( 2 ) << std::setfill( '0' );
  currentDate << ( now->tm_mon + 1 );
  currentDate << std::setw( 2 ) << std::setfill( '0' );
  currentDate << now->tm_mday;
  currentDate << std::setw( 2 ) << std::setfill( '0' );
  currentDate << now->tm_hour;
  currentDate << std::setw( 2 ) << std::setfill( '0' );
  currentDate << now->tm_min;
  currentDate << std::setw( 2 ) << std::setfill( '0' );
  currentDate << now->tm_sec;


  return currentDate.str();
}


void MetadataManager::populateRecord( vector< string >& record, const string& line, char delimiter )
{
  int linepos  = 0;
  int inquotes = false;
  char c;
  int i;
  int linemax = line.length();
  string curstring;
  record.clear();

  while( line[ linepos ] != 0 && linepos < linemax )
  {
    c = line[ linepos ];

    if( !inquotes && curstring.length() == 0 && c == '"' )
    {
      // beginquotechar
      inquotes = true;
    }
    else if( inquotes && c == '"' )
    {
      // quotechar
      if( ( linepos + 1 < linemax ) && ( line[ linepos + 1 ] == '"' ) )
      {
        // encountered 2 double quotes in a row (resolves to 1 double quote)
        curstring.push_back( c );
        ++linepos;
      }
      else
      {
        // endquotechar
        inquotes = false;
      }
    }
    else if( !inquotes && c == delimiter )
    {
      // end of field
      record.push_back( cepba_tools::trim( curstring ) );
      curstring = "";
    }
    else if( !inquotes && ( c == '\r' || c == '\n' ) )
    {
      record.push_back( cepba_tools::trim( curstring ) );
      return;
    }
    else
    {
      curstring.push_back( c );
    }
    ++linepos;
  }

  record.push_back( cepba_tools::trim( curstring ) );
  return;
}
