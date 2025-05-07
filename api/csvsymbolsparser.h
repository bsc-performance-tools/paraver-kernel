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

#include <map>
#include <string>
#include <tuple>

#include "paravertypes.h"


class CSVSymbolsParser
{
  public:
    using rgb = std::tuple< ParaverColor, ParaverColor, ParaverColor >;

    
    CSVSymbolsParser() = default;

    CSVSymbolsParser( std::string filename );
    
    ~CSVSymbolsParser() = default;

    void dumpToFile( const std::string& filename, bool isGradientColor ) const;

    const std::map< uint32_t, CSVSymbolsParser::rgb >& getSemanticColors() const;
    void                                               getEventTypes( std::vector< TEventType >& onVector ) const;
    const std::string&                                 getEventLabel( TEventType eventType ) const;
    const std::map< TEventValue, std::string >&        getEventValues( TEventType eventType ) const;
    unsigned int                                       getEventPrecision( TEventType eventType ) const;
    
    void setSemanticColor( uint32_t semanticValue, CSVSymbolsParser::rgb color );
    void setEventValueLabel( TEventType eventType, TEventValue eventValue, const std::string& label );

  private:
    void parseLine( const std::string& whichLine );
    
    std::map< uint32_t, CSVSymbolsParser::rgb > semanticColors;
    std::map< TEventValue, std::string > eventLabels;
};