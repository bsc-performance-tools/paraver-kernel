#ifndef EVENTLABELS_H_INCLUDED
#define EVENTLABELS_H_INCLUDED

#include <set>
#include <map>
#include "paraverkerneltypes.h"
#include "pcfparser/ParaverTraceConfig.h"

using namespace domain;

class EventLabels
{
  public:
    EventLabels();
    EventLabels( const set<TEventType>& eventsLoaded );
    EventLabels( const ParaverTraceConfig& config,
                 const set<TEventType>& eventsLoaded );
    ~EventLabels();

    void getTypes( vector<TEventType>& onVector ) const;
    string getEventTypeLabel( TEventType type ) const;
    string getEventValueLabel( TEventType type, TEventValue value ) const;

  protected:

  private:
    static const string unknownLabel;

    map<TEventType, string> eventTypeLabel;
    map<TEventType, map<TEventValue, string> > eventValueLabel;
};


#endif // EVENTLABELS_H_INCLUDED
