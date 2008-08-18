#include <sstream>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "histogram.h"
#include "paraverconfig.h"
string LabelConstructor::objectLabel( TObjectOrder globalOrder,
                                      TWindowLevel level,
                                      Trace *whichTrace )
{
  stringstream label;

  if ( level == WORKLOAD )
    label << LEVEL_WORKLOAD;
  else if ( level == APPLICATION )
    label << LEVEL_APPLICATION << ' ' << globalOrder + 1;
  else if ( level == TASK )
  {
    TApplOrder appl;
    TTaskOrder task;
    whichTrace->getTaskLocation( globalOrder, appl, task );
    label << LEVEL_TASK << ' ' << appl + 1 << '.' << task + 1;
  }
  else if ( level == THREAD )
  {
    TApplOrder appl;
    TTaskOrder task;
    TThreadOrder thread;
    whichTrace->getThreadLocation( globalOrder, appl, task, thread );
    label << LEVEL_THREAD << ' ' << appl + 1 << '.' << task + 1 << '.' << thread + 1;
  }
  else if ( level == SYSTEM )
    label << LEVEL_SYSTEM;
  else if ( level == NODE )
    label << LEVEL_NODE << ' ' << globalOrder + 1;
  else if ( level == CPU )
  {
    TNodeOrder node;
    TCPUOrder cpu;
    whichTrace->getCPULocation( globalOrder, node, cpu );
    label << LEVEL_CPU << ' ' << node + 1 << '.' << cpu + 1;
  }

  return label.str();
}


string LabelConstructor::histoColumnLabel( THistogramColumn whichColumn,
    const Histogram *whichHisto,
    THistogramLimit min,
    THistogramLimit max,
    THistogramLimit delta )
{
  stringstream label;
  double tmp;

  if ( modf( min, &tmp ) != 0.0 || delta != 1.0 )
  {
    // Column range values
    label << '[' << ( whichColumn * delta ) + min << "..";
    if ( ( ( whichColumn * delta ) + min + delta ) > max )
    {
      label << max;
      label << ']';
    }
    else
    {
      label << ( whichColumn * delta ) + min + delta;
      label << ')';
    }
  }
  else
  {
    // Discrete integer value
    label << ( whichColumn * delta ) + min;
  }

  return label.str();
}

inline string chomp( INT64& number )
{
  INT64 remainder = number % 1000;
  number /= 1000;

  if( number == 0 )
    return boost::lexical_cast<std::string>( remainder );
  else if( remainder == 0 )
    return "000";
  else if( remainder < 10 )
    return "00" + boost::lexical_cast<std::string>( remainder );
  else if( remainder < 100 )
    return "0" + boost::lexical_cast<std::string>( remainder );
  return boost::lexical_cast<std::string>( remainder );
}

string LabelConstructor::histoCellLabel( const Histogram *whichHisto,
    TSemanticValue value )
{
  stringstream label;

  if ( whichHisto->getScientificNotation() )
    label << scientific;
  else
    label << fixed;

  label.precision( ParaverConfig::getInstance()->getPrecision() );

  if ( whichHisto->getThousandSeparator() &&
       !whichHisto->getScientificNotation() )
  {
    string strNum;
    TSemanticValue origValue = value;
    INT64 intValue = INT64( value );

    if( origValue >= 1.0 )
    {
      while ( intValue > 0.0 )
        strNum = chomp( intValue ) + "," + strNum;
      strNum.erase( strNum.size() - 1, 1 );
      label << strNum;
    }

    stringstream tmp;
    tmp.precision( ParaverConfig::getInstance()->getPrecision() );
    value -= int( origValue );
    tmp << value;
    strNum = tmp.str();
    if( origValue >= 1.0 )
      strNum.erase( strNum.begin() );

    label << strNum;
  }
  else
    label << value;

  if ( whichHisto->getShowUnits() &&
       !whichHisto->itsCommunicationStat( whichHisto->getCurrentStat() ) )
    label << " " << whichHisto->getUnitsLabel( whichHisto->getCurrentStat() );

  return label.str();
}

string LabelConstructor::histoTotalLabel( THistoTotals whichTotal )
{
  switch( whichTotal )
  {
    case TOTAL:
      return "Total";
    case AVERAGE:
      return "Average";
    case MAXIMUM:
      return "Maximum";
    case MINIMUM:
      return "Minimum";
    case STDEV:
      return "StDev";
    case AVGDIVMAX:
      return "Avg/Max";
    case NUMTOTALS:
      return "";
  }

  return "";
}
