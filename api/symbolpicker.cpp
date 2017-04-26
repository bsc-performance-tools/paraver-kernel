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

#include "symbolpicker.h"

using std::vector;
using std::string;

EventTypeSymbolPicker::EventTypeSymbolPicker()
{}

EventTypeSymbolPicker::~EventTypeSymbolPicker()
{}

void EventTypeSymbolPicker::insert( TEventType whichType )
{
  eventTypes.push_back( whichType );
}

void EventTypeSymbolPicker::insert( string whichLabel )
{
  eventTypeLabels.push_back( whichLabel );
}

bool EventTypeSymbolPicker::pick( const EventLabels& eventLabels, vector<TEventType>& onVector ) const
{
  if( eventTypeLabels.size() == 0 )
  {
    onVector = eventTypes;
    return true;
  }

  if( eventTypes.size() != eventTypeLabels.size() )
    return false;

  vector<string>::const_iterator itLabel = eventTypeLabels.begin();
  for( vector<TEventType>::const_iterator itType = eventTypes.begin();
       itType != eventTypes.end(); ++itType, ++itLabel )
  {
    TEventType tmpType;
    if( makepick( eventLabels, *itType, *itLabel, tmpType ) )
      onVector.push_back( tmpType );
    else
      return false;
  }

  return true;
}


bool EventTypeSymbolPicker::makepick( const EventLabels& eventLabels, TEventType eventType, const string& eventLabel, TEventType& onEvent ) const
{



  return true;
}
