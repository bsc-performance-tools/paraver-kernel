#include "eventlabels.h"
#include "pcfparser/ParaverEventType.h"
#include "pcfparser/ParaverEventValue.h"

const string EventLabels::unknownLabel = "Unknown";

EventLabels::EventLabels()
{}

EventLabels::EventLabels( const set<TEventType>& eventsLoaded )
{
  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventTypeLabel[ *it ] = unknownLabel + " type";
}

EventLabels::EventLabels( const domain::ParaverTraceConfig& config,
                          const set<TEventType>& eventsLoaded )
{
  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventTypeLabel[ *it ] = unknownLabel + " type";

  const vector<ParaverEventType *>& types = config.get_eventTypes();
  for ( vector<ParaverEventType *>::const_iterator it = types.begin();
        it != types.end(); ++it )
  {
    eventTypeLabel[ ( *it )->get_key() ] = ( *it )->get_description();
    eventValueLabel[ ( *it )->get_key() ] = map<TEventValue, string>();
    const vector<ParaverEventValue *>& values = ( *it )->get_eventValues();
    for ( vector<ParaverEventValue *>::const_iterator itVal = values.begin();
          itVal != values.end(); ++itVal )
    {
      ( eventValueLabel[ ( *it )->get_key() ] )[ ( *itVal )->get_key() ] =
        ( *itVal )->get_value();
    }
  }
}

EventLabels::~EventLabels()
{}

void EventLabels::getTypes( vector<TEventType>& onVector ) const
{
  for ( map<TEventType, string>::const_iterator it = eventTypeLabel.begin();
        it != eventTypeLabel.end(); ++it )
    onVector.push_back( ( *it ).first );
}

bool EventLabels::getEventTypeLabel( TEventType type, string& onStr ) const
{
  map<TEventType, string>::const_iterator it = eventTypeLabel.find( type );
  if ( it == eventTypeLabel.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *it ).second;
  return true;
}

bool EventLabels::getEventValueLabel( TEventType type, TEventValue value, string& onStr ) const
{
  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValueLabel.find( type );
  if ( it == eventValueLabel.end() )
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

  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValueLabel.begin();
  while ( !found && it != eventValueLabel.end() )
  {
    map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
    if ( itVal != ( *it ).second.end() )
    {
      onStr = ( *itVal ).second;
      found = true;
    }
    it++;
  }

  if ( found )
    return true;
  onStr = unknownLabel;
  return false;
}
