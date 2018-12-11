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


#ifndef SYMBOLPICKER_H_INCLUDED
#define SYMBOLPICKER_H_INCLUDED

#include <vector>
#include <set>
#include "paraverkerneltypes.h"

class EventLabels;

/*****************************************************************************
 *                                                                           *
 *                       EventTypeSymbolPicker                               *
 *                                                                           *
 *****************************************************************************/
class EventTypeSymbolPicker
{
  public:
    EventTypeSymbolPicker();
    ~EventTypeSymbolPicker();

    void clear();

    void insert( TEventType whichType );
    void insert( std::string whichLabel );

    bool pick( const EventLabels& eventLabels, std::vector<TEventType>& onVector ) const;

  private:
    std::vector<TEventType> eventTypes;
    std::vector<std::string> eventTypeLabels;

    bool makepick( const EventLabels& eventLabels, TEventType eventType, const std::string& eventLabel, TEventType& onEvent ) const;
};


/*****************************************************************************
 *                                                                           *
 *                       EventValueSymbolPicker                              *
 *                                                                           *
 *****************************************************************************/
class EventValueSymbolPicker
{
  public:
    EventValueSymbolPicker();
    ~EventValueSymbolPicker();

    void clear();

    void insert( TSemanticValue whichValue );
    void insert( std::string whichLabel );

    bool pick( const EventLabels& eventLabels, std::vector<TSemanticValue>& onVector ) const;

    bool getMultipleValuesFound() const;

  private:
    std::vector<TSemanticValue> eventValues;
    std::vector<std::string> eventValueLabels;
    bool multipleValuesFound;

    bool makepick( const EventLabels& eventLabels, TSemanticValue eventValue,
                   const std::string& eventLabel,
                   std::set<TSemanticValue>& onValues ) const;
};

#endif // SYMBOLPICKER_H_INCLUDED
