#include "eventlabels.h"
#include "pcfparser/ParaverEventType.h"

const string EventLabels::unknownLabel = "Unknown";

EventLabels::EventLabels()
{}

EventLabels::EventLabels( const set<TEventType>& eventsLoaded )
{
  for( set<TEventType>::const_iterator it = eventsLoaded.begin();
       it != eventsLoaded.end(); it ++ )
    eventTypeLabel[ *it ] = unknownLabel;
}

EventLabels::EventLabels( const domain::ParaverTraceConfig& config,
                          const set<TEventType>& eventsLoaded )
{
  for( set<TEventType>::const_iterator it = eventsLoaded.begin();
       it != eventsLoaded.end(); it ++ )
    eventTypeLabel[ *it ] = unknownLabel;

  const vector<ParaverEventType *> types = config.get_eventTypes();
  for( vector<ParaverEventType *>::const_iterator it = types.begin();
       it != types.end(); it++ )
    eventTypeLabel[ (*it)->get_key() ] = (*it)->get_description();
}

EventLabels::~EventLabels()
{}

void EventLabels::getTypes( vector<TEventType>& onVector ) const
{
  for( map<TEventType, string>::const_iterator it = eventTypeLabel.begin();
       it != eventTypeLabel.end(); it++ )
    onVector.push_back( (*it).first );
}

string EventLabels::getEventTypeLabel( TEventType type ) const
{
  map<TEventType, string>::const_iterator it = eventTypeLabel.find( type );
  if ( it == eventTypeLabel.end() )
    return unknownLabel;
  return ( *it ).second;
}

string EventLabels::getEventValueLabel( TEventType type, TEventValue value ) const
{
  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValueLabel.find( type );
  if ( it == eventValueLabel.end() )
    return unknownLabel;
  map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
  if ( itVal == ( *it ).second.end() )
    return unknownLabel;
  return ( *itVal ).second;
}
