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

#include <functional>
#include <map>
#include <string>
#include <tuple>
#include "tracetypes.h"

template< typename dummy = nullptr_t >
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


  private:
    struct EventType
    {
      unsigned int precision;
      std::string label;
      std::map< TEventValue, std::string > values;
    };

    std::string level; // VALOR POR DEFECTO HARDCODED NO GET/SET
    std::string units; // VALOR POR DEFECTO HARDCODED NO GET/SET
    std::string look_back; // VALOR POR DEFECTO HARDCODED NO GET/SET
    std::string speed; // VALOR POR DEFECTO HARDCODED NO GET/SET
    std::string flag_icons; // VALOR POR DEFECTO HARDCODED NO GET/SET
    // int num_of_state_colors; se ignora y a la hora de volcar se usa el size del contenedor que tenga los colores
    std::string ymax_scale; // Recuperarlo desde los nuevos timelines creados
    std::string thread_func; // VALOR POR DEFECTO HARDCODED NO GET/SET
    std::map< TState, std::string > states;
    std::map< TSemanticValue, PCFFileParser::rgb > semanticColors;
    std::map< TEventType, PCFFileParser::EventType > events;

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
