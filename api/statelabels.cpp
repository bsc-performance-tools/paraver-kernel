#include "statelabels.h"
#include "../common-files/pcfparser/ParaverState.h"

const string StateLabels::unknownLabel = "Unknown";

StateLabels::StateLabels()
{}

StateLabels::StateLabels( const ParaverTraceConfig& config )
{
  const vector<ParaverState *>& states = config.get_states();
  for ( vector<ParaverState *>::const_iterator it = states.begin();
        it != states.end(); it++ )
    stateLabel[ ( *it )->get_key() ] = ( *it )->get_value();
}

StateLabels::~StateLabels()
{}

void StateLabels::getStates( vector<TState>& onVector ) const
{
  for ( map<TState, string>::const_iterator it = stateLabel.begin();
        it != stateLabel.end(); it++ )
    onVector.push_back( ( *it ).first );
}

bool StateLabels::getStateLabel( TState state, string& onStr ) const
{
  map<TState, string>::const_iterator it = stateLabel.find( state );
  if ( it == stateLabel.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *it ).second;
  return true;
}
