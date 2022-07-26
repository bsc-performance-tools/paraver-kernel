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

#include <algorithm>
#include <iostream>
#include <regex>

#include "eventlabels.h"

using namespace std;

const string EventLabels::unknownLabel = "Unknown";

EventLabels::EventLabels()
{}

EventLabels::EventLabels( const PCFFileParser<>& pcfParser )
{
  vector< TEventType > types;
  pcfParser.getEventTypes( types );
  for ( auto it : types )
  {
    eventType2Label[ it ] = pcfParser.getEventLabel( it );
    label2eventType[ pcfParser.getEventLabel( it ) ] = it;
    eventValue2Label[ it ] = pcfParser.getEventValues( it );
    for( auto itValue : eventValue2Label[ it ] )
      label2eventValue[ itValue.second ].insert( make_pair( it, itValue.first ) );
  }
/*
      vector<unsigned int> values = config.getEventValues( *it );
      for ( vector<unsigned int>::const_iterator itVal = values.begin(); itVal != values.end(); ++itVal )
      {
        ( eventValue2Label[ *it ] )[ *itVal ] = config.getEventValue( *it, *itVal );

        multimap< TEventType, TEventValue >& tmpEventMap = label2eventValue[ config.getEventValue( *it, *itVal ) ];
        tmpEventMap.insert( make_pair( *it, *itVal ) );
      }
  */
}

EventLabels::~EventLabels()
{}

void EventLabels::getTypes( vector<TEventType>& onVector ) const
{
  for ( map<TEventType, string>::const_iterator it = eventType2Label.begin();
        it != eventType2Label.end(); ++it )
    onVector.push_back( ( *it ).first );
}

void EventLabels::getTypes( std::function<void( TEventType, const std::string& )> insert ) const
{
  for( auto el : eventType2Label )
    insert( el.first, el.second );
}

bool EventLabels::getEventTypeLabel( TEventType type, string& onStr ) const
{
  map<TEventType, string>::const_iterator it = eventType2Label.find( type );
  if ( it == eventType2Label.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *it ).second;
  return true;
}

bool EventLabels::getEventValueLabel( TEventType type, TEventValue value, string& onStr ) const
{
  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
  if ( itVal == ( *it ).second.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *itVal ).second;
  return true;
}

bool EventLabels::getEventValueLabel( TEventValue value, string& onStr ) const
{
  bool found = false;
  onStr = unknownLabel;

  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValue2Label.begin();
  while ( !found && it != eventValue2Label.end() )
  {
    map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
    if ( itVal != ( *it ).second.end() )
    {
      onStr = ( *itVal ).second;
      found = true;
    }
    ++it;
  }

  return found;
}


bool EventLabels::getValues( TEventType type, vector<string> &values ) const
{
  map< TEventType, map< TEventValue, string > >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    return false;
  }
  else
  {
    for ( map< TEventValue, string >::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2 )
      values.push_back((*it2).second );
  }

  return true;
}


bool EventLabels::getValues( TEventType type, map<TEventValue, string> &values ) const
{
  map< TEventType, map< TEventValue, string > >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    return false;
  }
  else
  {
    for ( map<TEventValue, string>::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2 )
      values[ (*it2).first ] = (*it2).second;
  }

  return true;
}


void EventLabels::getValues( TEventType type, std::function<void( TEventValue, const std::string& )> insert ) const
{
  map< TEventType, map< TEventValue, string > >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
    return;
  else
  {
    for( auto el : it->second )
      insert( el.first, el.second );
  }
}

bool EventLabels::getEventType( const string& whichTypeLabel, vector<TEventType>& onVector ) const
{
  if( whichTypeLabel.find( "*" ) == string::npos )
  {
    map<string, TEventType>::const_iterator it = label2eventType.find( whichTypeLabel );
    if( it == label2eventType.end() )
      return false;

    onVector.push_back( (*it).second );
    return true;
  }
  else
  {
    string strRegex = "";
    std::for_each( whichTypeLabel.begin(), whichTypeLabel.end(),
                   [&strRegex]( char elem ) { switch( elem )
                                              {
                                                case '*':
                                                  strRegex += ".*";
                                                  break;
                                                case '.':
                                                case '+':
                                                case '?':
                                                case '$':
                                                case '^':
                                                case '|':
                                                case '[':
                                                case ']':
                                                case '(':
                                                case ')':
                                                case '{':
                                                case '}':
                                                  strRegex += '\\';
                                                  strRegex += elem;
                                                  break;
                                                default:
                                                  strRegex += elem;
                                                  break;
                                              }
                                            } );
    std::regex tmpRegex( strRegex );
    
    size_t prevSize = onVector.size();
    std::for_each( eventType2Label.begin(), eventType2Label.end(),
                   [&onVector, &tmpRegex]( auto elem ) { if( std::regex_match( elem.second, tmpRegex ) ) onVector.push_back( elem.first ); } );

    return prevSize != onVector.size();
  }

  return false;
}


bool EventLabels::getEventValue( const string& whichValueLabel, multimap< TEventType, TEventValue >& onMap ) const
{
  map<string, multimap< TEventType, TEventValue > >::const_iterator it = label2eventValue.find( whichValueLabel );

  if( it == label2eventValue.end() )
    return false;

  onMap = (*it).second;
  return true;
}
