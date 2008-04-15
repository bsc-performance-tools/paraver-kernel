#include "histogramstatistic.h"
#include "statisticmanagement.h"
#include "functionmanagement.h"


void createStatistic()
{
  vector<string> names;
  vector<vector<HistogramStatistic *> > functions;

  // Communication statistics
  functions.push_back( vector<HistogramStatistic *>() );

  functions[0].push_back( new StatNumSends() );
  functions[0].push_back( new StatNumReceives() );
  functions[0].push_back( new StatBytesSent() );
  functions[0].push_back( new StatBytesReceived() );
  functions[0].push_back( new StatAvgBytesSent() );
  functions[0].push_back( new StatAvgBytesReceived() );
  functions[0].push_back( new StatMinBytesSent() );
  functions[0].push_back( new StatMinBytesReceived() );
  functions[0].push_back( new StatMaxBytesSent() );
  functions[0].push_back( new StatMaxBytesReceived() );

  // Semantic statistics
  functions.push_back( vector<HistogramStatistic *>() );

  functions[1].push_back( new StatTime() );
  functions[1].push_back( new StatPercTime() );
  functions[1].push_back( new StatPercTimeNotZero() );
  functions[1].push_back( new StatPercTimeWindow() );
  functions[1].push_back( new StatNumBursts() );
  functions[1].push_back( new StatPercNumBursts() );
  functions[1].push_back( new StatIntegral() );
  functions[1].push_back( new StatAvgValue() );
  functions[1].push_back( new StatMaximum() );
  functions[1].push_back( new StatAvgBurstTime() );
  functions[1].push_back( new StatStdevBurstTime() );
  functions[1].push_back( new StatAvgPerBurst() );
  functions[1].push_back( new StatAvgValueNotZero() );
  functions[1].push_back( new StatNumBurstsNotZero() );
  functions[1].push_back( new StatSumBursts() );

  for ( UINT16 iGroup = 0; iGroup < functions.size(); iGroup++ )
  {
    for ( UINT16 iFunction = 0; iFunction < functions[ iGroup ].size(); iFunction++ )
      names.push_back( functions[ iGroup ][ iFunction ]->getName() );
  }

  FunctionManagement<HistogramStatistic>::getInstance( names, functions );
}
