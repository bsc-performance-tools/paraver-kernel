#include "histogramstatistic.h"
#include "statisticmanagement.h"
#include "functionmanagement.h"


void createStatistic()
{
  vector<string> names;
  vector<HistogramStatistic *> functions;

  functions.push_back( new StatNumSends() );
  functions.push_back( new StatNumReceives() );
  functions.push_back( new StatBytesSent() );
  functions.push_back( new StatBytesReceived() );
  functions.push_back( new StatAvgBytesSent() );
  functions.push_back( new StatAvgBytesReceived() );
  functions.push_back( new StatMinBytesSent() );
  functions.push_back( new StatMinBytesReceived() );
  functions.push_back( new StatMaxBytesSent() );
  functions.push_back( new StatMaxBytesReceived() );

  functions.push_back( new StatTime() );
  functions.push_back( new StatPercTime() );
  functions.push_back( new StatPercTimeNotZero() );
  functions.push_back( new StatPercTimeWindow() );
  functions.push_back( new StatNumBursts() );
  functions.push_back( new StatPercNumBursts() );
  functions.push_back( new StatIntegral() );
  functions.push_back( new StatAvgValue() );
  functions.push_back( new StatMaximum() );
  functions.push_back( new StatAvgBurstTime() );
  functions.push_back( new StatStdevBurstTime() );
  functions.push_back( new StatAvgPerBurst() );
  functions.push_back( new StatAvgValueNotZero() );
  functions.push_back( new StatNumBurstsNotZero() );
  functions.push_back( new StatSumBursts() );

  for ( UINT16 i = 0; i < functions.size(); i++ )
    names.push_back( functions[i]->getName() );

  FunctionManagement<HistogramStatistic>::getInstance( names, functions );
}
