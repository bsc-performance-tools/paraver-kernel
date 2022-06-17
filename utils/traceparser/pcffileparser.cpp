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


#include "pcffileparser.h" //DELETE ME!
#include <iostream> //DELETE ME!

// MAIN SECTIONS
constexpr char PCF_LABEL_DEFAULT_OPTIONS[] = "DEFAULT_OPTIONS";
constexpr char PCF_LABEL_DEFAULT_SEMANTIC[] = "DEFAULT_SEMANTIC";
constexpr char PCF_LABEL_STATES[] = "STATES";
constexpr char PCF_LABEL_STATES_COLOR[] = "STATES_COLOR";
constexpr char PCF_LABEL_EVENT_TYPE[] = "EVENT_TYPE";

// DEPRECATED SECTIONS
constexpr char PCF_LABEL_GRADIENT_COLOR[] = "GRADIENT_COLOR";
constexpr char PCF_LABEL_GRADIENT_NAMES[] = "GRADIENT_NAMES";


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
  std::replace( strLine.begin(), strLine.end(), '\t', ' ' );
  strLine.erase( 0, strLine.find_first_not_of( ' ' ) );
  strLine.erase( strLine.find_last_not_of( ' ' ) + 1 );
}

template<typename dummy>
PCFFileParser<dummy>::PCFFileParser( const std::string& filename )
{
  std::string strLine;
  currentParser = nullptr;

  std::ifstream pcfFile( filename.c_str() );
  if ( !pcfFile )
    throw std::ios_base::failure( "Error opening pcf file." );

  initSectionParserFactory();

  while ( !pcfFile.eof() )
  {
    getline( pcfFile, strLine );

    trimAndCleanTabs( strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' )
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

class DefaultOptionsParser : public PCFFileParser<>::SectionParser
{
  public:
    DefaultOptionsParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {
      parameterSetter[ PCF_LABEL_LEVEL ]      = [this]( std::string line ) { mainParser->level = line; };
      parameterSetter[ PCF_LABEL_UNITS ]      = [this]( std::string line ) { mainParser->units = line; };
      parameterSetter[ PCF_LABEL_LOOK_BACK ]  = [this]( std::string line ) { mainParser->look_back = line; };
      parameterSetter[ PCF_LABEL_SPEED ]      = [this]( std::string line ) { mainParser->speed = line; };
      parameterSetter[ PCF_LABEL_FLAG_ICONS ] = [this]( std::string line ) { mainParser->flag_icons = line; };
      parameterSetter[ PCF_LABEL_YMAX_SCALE ] = [this]( std::string line ) { mainParser->ymax_scale = line; };
    }

    virtual ~DefaultOptionsParser() = default;

    virtual void parseLine( const std::string& line );

  private:
    std::map<std::string, std::function<void( std::string )> > parameterSetter;
};

void DefaultOptionsParser::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string index = line.substr( 0, firstSpacePos );
  std::string value = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  auto it = parameterSetter.find( index );
  if( it != parameterSetter.end() )
    it->second( value );
}


// ----------------------------------------------------------------------------
// DefaultSemanticParser ------------------------------------------------------
// ----------------------------------------------------------------------------
constexpr char PCF_LABEL_THREAD_FUNC[] = "THREAD_FUNC";

class DefaultSemanticParser : public PCFFileParser<>::SectionParser
{
  public:
    DefaultSemanticParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {
      parameterSetter[ PCF_LABEL_THREAD_FUNC ]      = [this]( std::string line ) { mainParser->thread_func = line; };
    }

    virtual ~DefaultSemanticParser() = default;

    virtual void parseLine( const std::string& line );

  private:
    std::map<std::string, std::function<void( std::string )> > parameterSetter;
};

void DefaultSemanticParser::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string index = line.substr( 0, firstSpacePos );
  std::string value = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  auto it = parameterSetter.find( index );
  if( it != parameterSetter.end() )
    it->second( value );
}


// ----------------------------------------------------------------------------
// StatesParser ---------------------------------------------------------------
// ----------------------------------------------------------------------------
class StatesParser : public PCFFileParser<>::SectionParser
{
  public:
    StatesParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {}

    virtual ~StatesParser() = default;

    virtual void parseLine( const std::string& line );
};

void StatesParser::parseLine( const std::string& line )
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


// ----------------------------------------------------------------------------
// StatesColorParser ----------------------------------------------------------
// ----------------------------------------------------------------------------
class StatesColorParser : public PCFFileParser<>::SectionParser
{
  public:
    StatesColorParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {}

    virtual ~StatesColorParser() = default;

    virtual void parseLine( const std::string& line );
};

void StatesColorParser::parseLine( const std::string& line )
{
  size_t firstSpacePos = line.find_first_of( ' ' );
  std::string strSemanticValue = line.substr( 0, firstSpacePos );
  TSemanticValue tmpSemanticValue;
  try
  {
    tmpSemanticValue = std::stoi( strSemanticValue );
  }
  catch(...)
  {
    return;
  }  

  std::string strRGB = line.substr( line.find_first_not_of( ' ', firstSpacePos ), std::string::npos );

  size_t firstCommaPos = strRGB.find_first_of( ',', 1 );
  std::string strRed   = strRGB.substr( 1, firstCommaPos );
  size_t secondCommaPos = strRGB.find_last_of( ',' );
  std::string strGreen = strRGB.substr( firstCommaPos + 1, secondCommaPos );
  std::string strBlue  = strRGB.substr( secondCommaPos + 1, strRGB.length() - 1 );

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


// ----------------------------------------------------------------------------
// EventParser ----------------------------------------------------------------
// ----------------------------------------------------------------------------
class EventParser : public PCFFileParser<>::SectionParser
{
  public:
    EventParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {}

    virtual ~EventParser() = default;

    virtual void parseLine( const std::string& line );
};

void EventParser::parseLine( const std::string& line )
{

}


// ----------------------------------------------------------------------------
// GradientColorParser --------------------------------------------------------
// ----------------------------------------------------------------------------
class GradientColorParser : public PCFFileParser<>::SectionParser
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION

  public:
    GradientColorParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {}

    virtual ~GradientColorParser() = default;

    virtual void parseLine( const std::string& line );
};

void GradientColorParser::parseLine( const std::string& line )
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
}


// ----------------------------------------------------------------------------
// GradientNamesParser --------------------------------------------------------
// ----------------------------------------------------------------------------
class GradientNamesParser : public PCFFileParser<>::SectionParser
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION

  public:
    GradientNamesParser( PCFFileParser<> *whichMainParser ) : PCFFileParser<>::SectionParser( whichMainParser ) 
    {}

    virtual ~GradientNamesParser() = default;

    virtual void parseLine( const std::string& line );
};

void GradientNamesParser::parseLine( const std::string& line )
{
  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
}


template<typename dummy>
void PCFFileParser<dummy>::initSectionParserFactory()
{
  sectionParserFactory[ PCF_LABEL_DEFAULT_OPTIONS ]  = [this]() { return new DefaultOptionsParser( this ); };
  sectionParserFactory[ PCF_LABEL_DEFAULT_SEMANTIC ] = [this]() { return new DefaultSemanticParser( this ); };
  sectionParserFactory[ PCF_LABEL_STATES ]           = [this]() { return new StatesParser( this ); };
  sectionParserFactory[ PCF_LABEL_STATES_COLOR ]     = [this]() { return new StatesColorParser( this ); };
  sectionParserFactory[ PCF_LABEL_EVENT_TYPE ]       = [this]() { return new EventParser( this ); };

  // IGNORED: NOT USEFUL FOR CURRENT PARAVER VERSION
  sectionParserFactory[ PCF_LABEL_GRADIENT_COLOR ]   = [this]() { return new GradientColorParser( this ); };
  sectionParserFactory[ PCF_LABEL_GRADIENT_NAMES ]   = [this]() { return new GradientNamesParser( this ); };
}
