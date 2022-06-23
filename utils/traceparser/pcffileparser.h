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

#include <cstddef>
#include <functional>
#include <map>
#include <string>
#include <tuple>
#include "tracetypes.h"

template< typename dummy = std::nullptr_t >
class PCFFileParser
{
  public:
    using rgb = std::tuple< ParaverColor, ParaverColor, ParaverColor >;

    class SectionParser
    {
      public:
        SectionParser() = delete;
        SectionParser( PCFFileParser<> *whichMainParser ) : mainParser( whichMainParser )
        {}

        virtual ~SectionParser() = default;

        virtual void parseLine( const std::string& line ) = 0;

      protected:
        PCFFileParser<> *mainParser;
    };

    static bool openPCFFileParser( const std::string& filename, PCFFileParser<dummy>& outPCFFile );

    PCFFileParser() = delete;
    PCFFileParser( const std::string& filename );
    ~PCFFileParser() = default;

    void dumpToFile( const std::string& filename ) const;

    std::string getLevel() const;
    void setLevel( std::string& newValue );

    std::string getUnits() const;
    void setUnits( std::string& newValue );

    std::string getLookBack() const;
    void setLookBack( std::string& newValue );

    std::string getSpeed() const;
    void setSpeed( std::string& newValue );

    std::string getFlagIcons() const;
    void setFlagIcons( std::string& newValue );

    std::string getYmaxScale() const;
    void setYmaxScale( std::string& newValue );

    std::string getThreadFunc() const;
    void setThreadFunc( std::string& newValue );

    const std::map< TState, std::string >& getStates() const;
    void setState( TState state, const std::string& label );
    
    const std::map< TSemanticValue, PCFFileParser::rgb >& getSemanticColors() const;
    void setSemanticColor( TSemanticValue semanticValue, PCFFileParser::rgb color );

    void                                        getEventTypes( std::vector< TEventType >& onVector ) const;
    unsigned int                                getEventPrecision( TEventType eventType ) const;
    const std::string&                          getEventLabel( TEventType eventType ) const;
    const std::map< TEventValue, std::string >& getEventValues( TEventType eventType ) const;
    void setEventType( TEventType eventType,
                       unsigned int precision,
                       const std::string& label,
                       const std::map< TEventValue, std::string >& values );
    void setEventPrecision( TEventType eventType, unsigned int precision );
    void setEventLabel( TEventType eventType, const std::string& label );
    void setEventValues( TEventType eventType, const std::map< TEventValue, std::string >& values );
    void setEventValueLabel( TEventType eventType, TEventValue eventValue, const std::string& label );

  private:
    struct EventTypeData
    {
      unsigned int precision;
      std::string label;
      std::map< TEventValue, std::string > values;
    };

    std::string level;
    std::string units;
    std::string lookBack;
    std::string speed;
    std::string flagIcons;
    std::string ymaxScale;
    std::string threadFunc;
    std::map< TState, std::string > states;
    std::map< TSemanticValue, PCFFileParser::rgb > semanticColors;
    std::map< TEventType, PCFFileParser::EventTypeData > events;
    // TODO: Create class to manage same values and precission for a set of types

    std::map< std::string, std::function<PCFFileParser::SectionParser*(void)> > sectionParserFactory;
    SectionParser *currentParser;

    void initSectionParserFactory();
    void trimAndCleanTabs( std::string& strLine );

    friend class DefaultOptionsParser;
    friend class DefaultSemanticParser;
    friend class StatesParser;
    friend class StatesColorParser;
    friend class EventParser;
    friend class GradientColorParser;
    friend class GradientNamesParser;
};

#include "pcffileparser.cpp"
