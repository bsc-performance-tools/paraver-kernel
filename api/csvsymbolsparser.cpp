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

#include <exception>
#include <fstream>
#include <sys/stat.h>

#include "csvsymbolsparser.h"
#include "utils/traceparser/pcffileparser.h"

CSVSymbolsParser::CSVSymbolsParser( std::string filename, bool isGradientColor )
{
  #ifndef _WIN32
  struct stat fileInfo;
#else
  struct _stat64 fileInfo;
#endif

#ifndef _WIN32
  if( stat( filename.c_str(), &fileInfo ) != 0 )
    return;
#else
  if( _stat64( filename.c_str(), &fileInfo ) != 0 )
    return;
#endif

  std::string strLine;

  std::ifstream csvFile( filename.c_str() );
  if ( !csvFile )
    throw std::ios_base::failure( "Error opening csv symbols file." );

  while ( !csvFile.eof() )
  {
    getline( csvFile, strLine );

    if ( strLine.length() == 0 )
      continue;

    PCFFileParser<>::trimAndCleanTabs( strLine );

    if ( strLine[ 0 ] == '#' )
      continue;

    parseLine( strLine, isGradientColor );
  }

  csvFile.close();
}

void CSVSymbolsParser::parseLine( const std::string& whichLine, bool isGradientColor )
{
  TSemanticValue value, secondValue;
  std::string label;
  
  std::string currentField;
  std::string::size_type fromPos = 0, secondPos = 0;

  std::string::size_type toPos = whichLine.find_first_of( " ", fromPos );
  currentField = whichLine.substr( fromPos, toPos - fromPos );
  value = std::stod( currentField );
  if( ( secondPos = currentField.find_first_of( "-" ) ) != std::string::npos )
    secondValue = std::stod( currentField.substr( secondPos + 1 ) );
  fromPos = toPos + 2;
  toPos = whichLine.find_first_of( "\"", fromPos );
  currentField = whichLine.substr( fromPos, toPos - fromPos );
  label = currentField;
  eventLabels.insert( { value, label } );
  if( isGradientColor && secondPos != std::string::npos )
    eventLabels.insert( { secondValue, label } );

  if( !isGradientColor )
  {
    fromPos = toPos + 2;
    currentField = whichLine.substr( fromPos );

    std::string currentColor;
    uint8_t red, green, blue;

    fromPos = 0;
    toPos = currentField.find_first_of( ",", fromPos );
    currentColor = currentField.substr( fromPos, toPos - fromPos );
    red = std::stoul( currentColor );

    fromPos = toPos + 1;
    toPos = currentField.find_first_of( ",", fromPos );
    currentColor = currentField.substr( fromPos, toPos - fromPos );
    green = std::stoul( currentColor );

    fromPos = toPos + 1;
    currentColor = currentField.substr( fromPos );
    blue = std::stoul( currentColor );

    CSVSymbolsParser::rgb tmprgb{ red, green, blue };
      semanticColors.insert( { value, tmprgb } );
  }
}

template< typename T >
void printEvent( std::ofstream& csvFile,
                 const std::map< TEventValue, std::string >& eventLabels,
                 const T& firstEvent,
                 const T& lastEvent = {},
                 bool isGradientColor = false )
{
  csvFile << std::fixed;

  csvFile << firstEvent.first;
  
  if( isGradientColor && firstEvent != lastEvent )
    csvFile << "-" << lastEvent.first;

  csvFile << " ";

  auto itLabel = eventLabels.find( firstEvent.first );
  csvFile << "\"";
  if( itLabel != eventLabels.end() )
    csvFile << itLabel->second;
  if( isGradientColor  && firstEvent != lastEvent )
  {
    itLabel = eventLabels.find( lastEvent.first );
    if( itLabel != eventLabels.end() )
      csvFile << "-" << itLabel->second;
  }
  csvFile << "\"" << " ";

  csvFile << std::to_string( std::get<0>( firstEvent.second ) ) << ",";
  csvFile << std::to_string( std::get<1>( firstEvent.second ) ) << ",";
  csvFile << std::to_string( std::get<2>( firstEvent.second ) );

  csvFile << std::endl;
}

void CSVSymbolsParser::dumpToFile( const std::string& filename, bool isGradientColor ) const
{
  std::ofstream csvFile( filename );

  std::pair< TSemanticValue, CSVSymbolsParser::rgb > rangeFirstColor = *semanticColors.begin();
  auto rangeEndColor = rangeFirstColor;

  for( auto& event : semanticColors )
  {
    if( isGradientColor && rangeFirstColor.second != event.second )
    {
      printEvent( csvFile, eventLabels, rangeFirstColor, rangeEndColor, isGradientColor );
      rangeFirstColor = rangeEndColor = event;
    }
    else if( isGradientColor )
      rangeEndColor = event;
    else
      printEvent( csvFile, eventLabels, event );
  }

  if( isGradientColor && rangeFirstColor == rangeEndColor )
    printEvent( csvFile, eventLabels, rangeFirstColor, rangeEndColor, isGradientColor );

  csvFile.close();
}

const std::map< TSemanticValue, CSVSymbolsParser::rgb >& CSVSymbolsParser::getSemanticColors() const
{
  return semanticColors;
}

void CSVSymbolsParser::getEventTypes( std::vector< TEventType >& onVector ) const
{
  onVector.push_back( 1 );
}

const std::string& CSVSymbolsParser::getEventLabel( TEventType eventType ) const
{
  static std::string dummyLabel{};
  return dummyLabel;
}

const std::map< TEventValue, std::string >& CSVSymbolsParser::getEventValues( TEventType eventType ) const
{
  return eventLabels;
}

unsigned int CSVSymbolsParser::getEventPrecision( TEventType eventType ) const
{
  return 0;
}

void CSVSymbolsParser::setSemanticColor( TSemanticValue semanticValue, CSVSymbolsParser::rgb color )
{
  semanticColors[ semanticValue ] = color;
}

void CSVSymbolsParser::setEventValueLabel( TEventType eventType, TEventValue eventValue, const std::string& label )
{
  if( eventType != 1 )
    throw std::out_of_range( "Only event type 1 is allowed for CSV symbols." );

  eventLabels[ eventValue ] = label;
}
