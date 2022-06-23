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


#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// MAIN SECTIONS
constexpr char PCF_LABEL_DEFAULT_OPTIONS[] = "DEFAULT_OPTIONS";
constexpr char PCF_LABEL_DEFAULT_SEMANTIC[] = "DEFAULT_SEMANTIC";
constexpr char PCF_LABEL_STATES[] = "STATES";
constexpr char PCF_LABEL_STATES_COLOR[] = "STATES_COLOR";
constexpr char PCF_LABEL_EVENT_TYPE[] = "EVENT_TYPE";

// DEPRECATED SECTIONS
constexpr char PCF_LABEL_GRADIENT_COLOR[] = "GRADIENT_COLOR";
constexpr char PCF_LABEL_GRADIENT_NAMES[] = "GRADIENT_NAMES";

constexpr char PCF_EXCEPTION_UNKNOWN_TYPE[] = "Unknown event type";
constexpr char PCF_EXCEPTION_UNKNOWN_VALUE[] = "Unknown event value";

template<typename dummy>
bool PCFFileParser<dummy>::openPCFFileParser( const std::string& filename, PCFFileParser<dummy>& outPCFFile )
{
  static const std::regex prvFilenameRegex( R"((.+)\.(prv|prv\.gz)$)" );

  std::string pcfFilename = std::regex_replace( filename, prvFilenameRegex, "$1.pcf" );

  try
  {
    outPCFFile = PCFFileParser<dummy>( pcfFilename );
  }
  catch( const std::ios_base::failure& e )
  {
    std::cerr << e.what() << '\n';
    return false;
  }
  
  return true;
}

template<typename dummy>
void PCFFileParser<dummy>::trimAndCleanTabs( std::string& strLine )
{
  std::string strOutLine;
  std::replace_copy( strLine.begin() + strLine.find_first_not_of( " \t"  ),
                     strLine.begin() + strLine.find_last_not_of( " \t"  ) + 1,
                     std::back_inserter( strOutLine ),
                     '\t',
                     ' ' );
  strLine = strOutLine;
}

template<typename dummy>
PCFFileParser<dummy>::PCFFileParser( const std::string& filename )
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
  currentParser = nullptr;

  std::ifstream pcfFile( filename.c_str() );
  if ( !pcfFile )
    throw std::ios_base::failure( "Error opening pcf file." );

  initSectionParserFactory();

  while ( !pcfFile.eof() )
  {
    getline( pcfFile, strLine );

    if ( strLine.length() == 0 )
      continue;

    trimAndCleanTabs( strLine );

    if ( strLine[ 0 ] == '#' )
      continue;
    
    auto tmpParserIt = sectionParserFactory.find( strLine );
    if( tmpParserIt != sectionParserFactory.end() )
    {
      if( currentParser != nullptr )
        delete currentParser;
      currentParser = tmpParserIt->second();
      continue;
    }

    if( currentParser != nullptr )
      currentParser->parseLine( strLine );
  }

  if( currentParser != nullptr )
    delete currentParser;

  pcfFile.close();
}

template<typename dummy>
std::string PCFFileParser<dummy>::getLevel() const { return level; }
template<typename dummy>
void PCFFileParser<dummy>::setLevel( std::string& newValue ) { level = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getUnits() const { return units; }
template<typename dummy>
void PCFFileParser<dummy>::setUnits( std::string& newValue ) { units = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getLookBack() const { return lookBack; }
template<typename dummy>
void PCFFileParser<dummy>::setLookBack( std::string& newValue ) { lookBack = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getSpeed() const { return speed; }
template<typename dummy>
void PCFFileParser<dummy>::setSpeed( std::string& newValue ) { speed = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getFlagIcons() const { return flagIcons; }
template<typename dummy>
void PCFFileParser<dummy>::setFlagIcons( std::string& newValue ) { flagIcons = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getYmaxScale() const { return ymaxScale; }
template<typename dummy>
void PCFFileParser<dummy>::setYmaxScale( std::string& newValue ) { ymaxScale = newValue; }

template<typename dummy>
std::string PCFFileParser<dummy>::getThreadFunc() const { return threadFunc; }
template<typename dummy>
void PCFFileParser<dummy>::setThreadFunc( std::string& newValue ) { threadFunc = newValue; }

template<typename dummy>
const std::map< TState, std::string >& PCFFileParser<dummy>::getStates() const { return states; }
template<typename dummy>
void PCFFileParser<dummy>::setState( TState state, const std::string& label ) { states[ state ] = label; }

template<typename dummy>
const std::map< uint32_t, PCFFileParser<>::rgb >& PCFFileParser<dummy>::getSemanticColors() const { return semanticColors; }
template<typename dummy>
void PCFFileParser<dummy>::setSemanticColor( uint32_t semanticValue, PCFFileParser::rgb color ) { semanticColors[ semanticValue ] = color; }


template<typename dummy>
void PCFFileParser<dummy>::getEventTypes( std::vector< TEventType >& onVector ) const
{
  std::transform( std::begin( events ), std::end( events ), std::back_inserter( onVector ), 
                  [](auto const& pair) { return pair.first; } );
}

template<typename dummy>
unsigned int PCFFileParser<dummy>::getEventPrecision( TEventType eventType ) const
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  return it->second.precision;
}

template<typename dummy>
const std::string& PCFFileParser<dummy>::getEventLabel( TEventType eventType ) const
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  return it->second.label;
}

template<typename dummy>
const std::map< TEventValue, std::string >& PCFFileParser<dummy>::getEventValues( TEventType eventType ) const
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  return it->second.values;
}

template<typename dummy>
void PCFFileParser<dummy>::setEventType( TEventType eventType,
                                         unsigned int precision,
                                         const std::string& label,
                                         const std::map< TEventValue, std::string >& values )
{
  PCFFileParser::EventTypeData tmpEventData { precision, label, values };

  events[ eventType ] = tmpEventData;
}


template<typename dummy>
void PCFFileParser<dummy>::setEventPrecision( TEventType eventType, unsigned int precision )
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  it->second.precision = precision;
}

template<typename dummy>
void PCFFileParser<dummy>::setEventLabel( TEventType eventType, const std::string& label )
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  it->second.label = label;
}

template<typename dummy>
void PCFFileParser<dummy>::setEventValues( TEventType eventType, const std::map< TEventValue, std::string >& values )
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );

  it->second.values = values;
}

template<typename dummy>
void PCFFileParser<dummy>::setEventValueLabel( TEventType eventType, TEventValue eventValue, const std::string& label )
{
  auto it = events.find( eventType );
  if( it == events.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_TYPE );
 
  auto itValues = it->second.values.find( eventType );
  if( itValues == it->second.values.end() )
    throw std::out_of_range( PCF_EXCEPTION_UNKNOWN_VALUE );

  itValues->second.label = label;
}


// ----------------------------------------------------------------------------
// DefaultOptionsParser -------------------------------------------------------
// ----------------------------------------------------------------------------
constexpr char PCF_LABEL_LEVEL[]               = "LEVEL";
constexpr char PCF_LABEL_UNITS[]               = "UNITS";
constexpr char PCF_LABEL_LOOK_BACK[]           = "LOOK_BACK";
constexpr char PCF_LABEL_SPEED[]               = "SPEED";
constexpr char PCF_LABEL_FLAG_ICONS[]          = "FLAG_ICONS";
constexpr char PCF_LABEL_NUM_OF_STATE_COLORS[] = "NUM_OF_STATE_COLORS";
constexpr char PCF_LABEL_YMAX_SCALE[]          = "YMAX_SCALE";

template< typename dummyParser = std::nullptr_t >
class DefaultOptionsParser : public PCFFileParser<>::SectionParser<>
{
  public:
    DefaultOptionsParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {
      parameterSetter[ PCF_LABEL_LEVEL ]      = [this]( std::string line ) { mainParser->level = line; };
      parameterSetter[ PCF_LABEL_UNITS ]      = [this]( std::string line ) { mainParser->units = line; };
      parameterSetter[ PCF_LABEL_LOOK_BACK ]  = [this]( std::string line ) { mainParser->lookBack = line; };
      parameterSetter[ PCF_LABEL_SPEED ]      = [this]( std::string line ) { mainParser->speed = line; };
      parameterSetter[ PCF_LABEL_FLAG_ICONS ] = [this]( std::string line ) { mainParser->flagIcons = line; };
      parameterSetter[ PCF_LABEL_YMAX_SCALE ] = [this]( std::string line ) { mainParser->ymaxScale = line; };
    }

    virtual ~DefaultOptionsParser() = default;

    virtual void parseLine( const std::string& line );
    
    static void dumpToFile( std::ofstream&, const PCFFileParser<>& whichMainParser );

  private:
    std::map<std::string, std::function<void( std::string )> > parameterSetter;
};

template< typename dummyParser >
void DefaultOptionsParser<dummyParser>::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string index = line.substr( 0, firstSpacePos );
  std::string value = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  auto it = parameterSetter.find( index );
  if( it != parameterSetter.end() )
    it->second( value );
}

template< typename dummyParser >
void DefaultOptionsParser<dummyParser>::dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser )
{
  pcfFile << PCF_LABEL_DEFAULT_OPTIONS << std::endl;
  size_t width = std::strlen( PCF_LABEL_NUM_OF_STATE_COLORS ) + 1;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_LEVEL << std::setw(0) << whichMainParser.level << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_UNITS << std::setw(0) << whichMainParser.units << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_LOOK_BACK << std::setw(0) << whichMainParser.lookBack << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_SPEED << std::setw(0) << whichMainParser.speed << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_FLAG_ICONS << std::setw(0) << whichMainParser.flagIcons << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_NUM_OF_STATE_COLORS << std::setw(0) << whichMainParser.semanticColors.size() << std::endl;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_YMAX_SCALE << std::setw(0) << whichMainParser.ymaxScale << std::endl;

  pcfFile << std::endl;
}

// ----------------------------------------------------------------------------
// DefaultSemanticParser ------------------------------------------------------
// ----------------------------------------------------------------------------
constexpr char PCF_LABEL_THREAD_FUNC[] = "THREAD_FUNC";

template< typename dummyParser = std::nullptr_t >
class DefaultSemanticParser : public PCFFileParser<>::SectionParser<>
{
  public:
    DefaultSemanticParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {
      parameterSetter[ PCF_LABEL_THREAD_FUNC ] = [this]( std::string line ) { mainParser->threadFunc = line; };
    }

    virtual ~DefaultSemanticParser() = default;

    virtual void parseLine( const std::string& line );

    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );

  private:
    std::map<std::string, std::function<void( std::string )> > parameterSetter;
};

template< typename dummyParser >
void DefaultSemanticParser<dummyParser>::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string index = line.substr( 0, firstSpacePos );
  std::string value = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  auto it = parameterSetter.find( index );
  if( it != parameterSetter.end() )
    it->second( value );
}

template< typename dummyParser >
void DefaultSemanticParser<dummyParser>::dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser )
{
  pcfFile << PCF_LABEL_DEFAULT_SEMANTIC << std::endl;
  size_t width = std::strlen( PCF_LABEL_NUM_OF_STATE_COLORS ) + 1;
  pcfFile << std::left << std::setw(width) << PCF_LABEL_THREAD_FUNC << std::setw(0) << whichMainParser.threadFunc << std::endl;

  pcfFile << std::endl;
}

// ----------------------------------------------------------------------------
// StatesParser ---------------------------------------------------------------
// ----------------------------------------------------------------------------
template< typename dummyParser = std::nullptr_t >
class StatesParser : public PCFFileParser<>::SectionParser<>
{
  public:
    StatesParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {}

    virtual ~StatesParser() = default;

    virtual void parseLine( const std::string& line );

    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );
};

template< typename dummyParser >
void StatesParser<dummyParser>::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string strState = line.substr( 0, firstSpacePos );
  TState tmpState;
  try
  {
    tmpState = std::stoi( strState );
  }
  catch(...)
  {
    return;
  }  

  std::string label = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  mainParser->states[ tmpState ] = label;
}

template< typename dummyParser >
void StatesParser<dummyParser>::dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser )
{
  pcfFile << PCF_LABEL_STATES << std::endl;
  for( auto it : whichMainParser.states )
    pcfFile << it.first << '\t' << it.second << std::endl;
  
  pcfFile << std::endl;
}


// ----------------------------------------------------------------------------
// StatesColorParser ----------------------------------------------------------
// ----------------------------------------------------------------------------
template< typename dummyParser = std::nullptr_t >
class StatesColorParser : public PCFFileParser<>::SectionParser<>
{
  public:
    StatesColorParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {}

    virtual ~StatesColorParser() = default;

    virtual void parseLine( const std::string& line );
    
    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );
};

template< typename dummyParser >
void StatesColorParser<dummyParser>::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string strSemanticValue = line.substr( 0, firstSpacePos );
  uint32_t tmpSemanticValue;
  try
  {
    tmpSemanticValue = std::stoi( strSemanticValue );
  }
  catch(...)
  {
    return;
  }  

  std::string strRGB = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  size_t firstCommaPos  = strRGB.find_first_of( ',', 1 );
  std::string strRed    = strRGB.substr( 1, firstCommaPos );
  size_t secondCommaPos = strRGB.find_last_of( ',' );
  std::string strGreen  = strRGB.substr( firstCommaPos + 1, secondCommaPos );
  std::string strBlue   = strRGB.substr( secondCommaPos + 1, strRGB.length() - 1 );

  ParaverColor red, green, blue;
  try
  {
    red = std::stoi( strRed );
    green = std::stoi( strGreen );
    blue = std::stoi( strBlue );
  }
  catch(...)
  {
    return;
  }  

  mainParser->semanticColors[ tmpSemanticValue ] = std::tie( red, green, blue );
}

template< typename dummyParser >
void StatesColorParser<dummyParser>::dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser )
{
  pcfFile << PCF_LABEL_STATES_COLOR << std::endl;
  for ( auto it : whichMainParser.semanticColors )
    pcfFile << it.first << "\t{" << (int)std::get<0>( it.second ) << ","
                                 << (int)std::get<1>( it.second ) << ","
                                 << (int)std::get<2>( it.second ) << "}" << std::endl;

  pcfFile << std::endl;
}


// ----------------------------------------------------------------------------
// EventParser ----------------------------------------------------------------
// ----------------------------------------------------------------------------
constexpr char PCF_LABEL_EVENT_VALUES[] = "VALUES";
constexpr char PCF_LABEL_EVENT_PRECISION[] = "PRECISION";

template< typename dummyParser = std::nullptr_t >
class EventParser : public PCFFileParser<>::SectionParser<>
{
  public:
    EventParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {
      readingEventValues = false;
    }

    virtual ~EventParser() = default;

    virtual void parseLine( const std::string& line );

    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );

  private:
    bool readingEventValues;
    std::vector<TEventType> currentTypes;
};

template< typename dummyParser >
void EventParser<dummyParser>::parseLine( const std::string& line )
{
  if( line == PCF_LABEL_EVENT_VALUES )
  {
    readingEventValues = true;
    return;
  }

  if( line.substr( 0, strlen( PCF_LABEL_EVENT_PRECISION ) ) == PCF_LABEL_EVENT_PRECISION )
  {
    std::string precision = line.substr( line.find_first_not_of( ' ', strlen( PCF_LABEL_EVENT_PRECISION ) ), std::string::npos );
    unsigned int tmpPrecision;
    try
    {
      tmpPrecision = std::stoul( precision );
    }
    catch(...)
    {
      return;
    }

    for( auto it : currentTypes )
      mainParser->events[ it ].precision = tmpPrecision;
  }
  else if ( !readingEventValues )
  {
    size_t firstSpacePos = line.find_first_of( ' ' );
    std::string dummyColor = line.substr( 0, firstSpacePos );
    size_t initTypePos = line.find_first_not_of( ' ', firstSpacePos );
    size_t secondSpacePos = line.find_first_of( ' ', initTypePos );
    std::string eventType = line.substr( initTypePos, secondSpacePos );
    std::string label = line.substr( line.find_first_not_of( ' ', secondSpacePos ), std::string::npos );
    TEventType tmpEventType;
    try
    {
      tmpEventType = std::stoi( eventType );
    }
    catch(...)
    {
      return;
    }
    
    currentTypes.push_back( tmpEventType );
    mainParser->events[ tmpEventType ].label = label;
  }
  else
  {
    size_t firstSpacePos = line.find_first_of( ' ' );
    std::string value = line.substr( 0, firstSpacePos );
    std::string label = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );
    TEventValue tmpValue;
    try
    {
      tmpValue = std::stoi( value );
    }
    catch(...)
    {
      return;
    }  
    
    for( auto it : currentTypes )
      mainParser->events[ it ].values[ tmpValue ] = label;
  }
}

template< typename dummyParser >
void EventParser<dummyParser>::dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser )
{
  for( auto it : whichMainParser.events )
  {
    pcfFile << PCF_LABEL_EVENT_TYPE << std::endl;
    pcfFile << 0 << "   " << it.first << "    " << it.second.label << std::endl;
    if( !it.second.values.empty() )
    {
      pcfFile << PCF_LABEL_EVENT_VALUES << std::endl;
      for( auto value : it.second.values )
        pcfFile << value.first << "  " << value.second << std::endl;
    }
    if( it.second.precision != 0 )
      pcfFile << PCF_LABEL_EVENT_PRECISION << " " << it.second.precision << std::endl;
    
    pcfFile << std::endl;
  }
}

// ----------------------------------------------------------------------------
// GradientColorParser --------------------------------------------------------
// ----------------------------------------------------------------------------
template< typename dummyParser = std::nullptr_t >
class GradientColorParser : public PCFFileParser<>::SectionParser<>
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION

  public:
    GradientColorParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {}

    virtual ~GradientColorParser() = default;

    virtual void parseLine( const std::string& line );

    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );
};

template< typename dummyParser >
void GradientColorParser<dummyParser>::parseLine( const std::string& line )
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
}


// ----------------------------------------------------------------------------
// GradientNamesParser --------------------------------------------------------
// ----------------------------------------------------------------------------
template< typename dummyParser = std::nullptr_t >
class GradientNamesParser : public PCFFileParser<>::SectionParser<>
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION

  public:
    GradientNamesParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser<>( whichMainParser ) 
    {}

    virtual ~GradientNamesParser() = default;

    virtual void parseLine( const std::string& line );

    static void dumpToFile( std::ofstream& pcfFile, const PCFFileParser<>& whichMainParser );
};

template< typename dummyParser >
void GradientNamesParser<dummyParser>::parseLine( const std::string& line )
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
}


template<typename dummy>
void PCFFileParser<dummy>::initSectionParserFactory()
{
  sectionParserFactory[ PCF_LABEL_DEFAULT_OPTIONS ]  = [this]() { return new DefaultOptionsParser<>( this ); };
  sectionParserFactory[ PCF_LABEL_DEFAULT_SEMANTIC ] = [this]() { return new DefaultSemanticParser<>( this ); };
  sectionParserFactory[ PCF_LABEL_STATES ]           = [this]() { return new StatesParser<>( this ); };
  sectionParserFactory[ PCF_LABEL_STATES_COLOR ]     = [this]() { return new StatesColorParser<>( this ); };
  sectionParserFactory[ PCF_LABEL_EVENT_TYPE ]       = [this]() { return new EventParser<>( this ); };

  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
  // sectionParserFactory[ PCF_LABEL_GRADIENT_COLOR ]   = [this]() { return new GradientColorParser<>( this ); };
  // sectionParserFactory[ PCF_LABEL_GRADIENT_NAMES ]   = [this]() { return new GradientNamesParser<>( this ); };
}


template<typename dummy>
void PCFFileParser<dummy>::dumpToFile( const std::string& filename ) const
{
  std::ofstream pcfFile( filename.c_str() );
  if ( !pcfFile )
    throw std::ios_base::failure( "Error opening pcf file." );

  DefaultOptionsParser<>::dumpToFile( pcfFile, *this );
  DefaultSemanticParser<>::dumpToFile( pcfFile, *this );
  StatesParser<>::dumpToFile( pcfFile, *this );
  StatesColorParser<>::dumpToFile( pcfFile, *this );
  EventParser<>::dumpToFile( pcfFile, *this );
  // GradientColorParser::dumpToFile( pcfFile, *this );
  // GradientNamesParser::dumpToFile( pcfFile, *this );
}
