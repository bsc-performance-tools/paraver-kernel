#ifndef STATELABELS_H_INCLUDED
#define STATELABELS_H_INCLUDED

#include <map>
#include "paraverkerneltypes.h"
#include "pcfparser/ParaverTraceConfig.h"

using namespace domain;

class StateLabels
{
  public:
    StateLabels();
    StateLabels( const ParaverTraceConfig& config );
    ~StateLabels();

    void getStates( vector<TState>& onVector ) const;
    bool getStateLabel( TState state, string& onStr ) const;

  protected:

  private:
    static const string unknownLabel;

    map<TState, string> stateLabel;
};



#endif // STATELABELS_H_INCLUDED
