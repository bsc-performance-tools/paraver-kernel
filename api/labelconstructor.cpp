#include <sstream>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "histogram.h"
#include "paraverconfig.h"
#include "window.h"

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
    label << LabelConstructor::semanticLabel( whichHisto->getControlWindow(),
        ( whichColumn * delta ) + min, true );
  }

  return label.str();
}

inline string chomp( INT64& number )
{
  INT64 remainder = number % 1000;
  number /= 1000;

  if ( number == 0 )
    return boost::lexical_cast<std::string>( remainder );
  else if ( remainder > 99 )
    return boost::lexical_cast<std::string>( remainder );
  else if ( remainder > 9 )
    return "0" + boost::lexical_cast<std::string>( remainder );
  else if ( remainder > 0 )
    return "00" + boost::lexical_cast<std::string>( remainder );
  return "000";
}

string LabelConstructor::histoCellLabel( const Histogram *whichHisto,
    TSemanticValue value )
{
  stringstream label;

  if ( value == numeric_limits<double>::infinity() )
    return "inf";

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

    if ( origValue >= 1.0 )
    {
      while ( intValue > 0.0 )
        strNum = chomp( intValue ) + "," + strNum;
      strNum.erase( strNum.size() - 1, 1 );
      label << strNum;
    }

    stringstream tmp;
    if ( whichHisto->getScientificNotation() )
      tmp << scientific;
    else
      tmp << fixed;
    tmp.precision( ParaverConfig::getInstance()->getPrecision() );
    value -= INT64( origValue );
    if ( value > 0 )
    {
      tmp << value;
      strNum = tmp.str();
      if ( origValue >= 1.0 )
        strNum.erase( strNum.begin() );

      label << strNum;
    }
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
  switch ( whichTotal )
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

string LabelConstructor::timeLabel( TTime value, TTimeUnit unit )
{
  stringstream label;

  label << fixed;
  label.precision( ParaverConfig::getInstance()->getPrecision() );

  label << value;
  label << " " << LABEL_TIMEUNIT[ unit ];

  return label.str();
}

string LabelConstructor::semanticLabel( Window * whichWindow,
                                        TSemanticValue value,
                                        bool text )
{
  stringstream label;
  SemanticInfoType infoType = whichWindow->getSemanticInfoType();

  label << fixed;
  if ( ( value - INT64( value ) ) > 0.0 )
    label.precision( ParaverConfig::getInstance()->getPrecision() );
  else
    label.precision( 0 );

  if ( infoType == NO_TYPE || !text )
  {
    label << value;
  }
  else
  {
    if ( infoType == OBJECT_TYPE )
      label << LabelConstructor::objectLabel( value, whichWindow->getLevel(),
                                              whichWindow->getTrace() );
    else if ( infoType == APPL_TYPE )
      label << LabelConstructor::objectLabel( value, APPLICATION,
                                              whichWindow->getTrace() );
    else if ( infoType == TASK_TYPE )
      label << LabelConstructor::objectLabel( value, TASK,
                                              whichWindow->getTrace() );
    else if ( infoType == THREAD_TYPE )
      label << LabelConstructor::objectLabel( value, THREAD,
                                              whichWindow->getTrace() );
    else if ( infoType == NODE_TYPE )
      label << LabelConstructor::objectLabel( value, NODE,
                                              whichWindow->getTrace() );
    else if ( infoType == CPU_TYPE )
      label << LabelConstructor::objectLabel( value, CPU,
                                              whichWindow->getTrace() );
    else if ( infoType == TIME_TYPE )
      label << LabelConstructor::timeLabel( value, whichWindow->getTimeUnit() );
    else if ( infoType == STATE_TYPE )
    {
      string tmpstr;
      if ( !whichWindow->getTrace()->getStateLabels().getStateLabel( value, tmpstr ) )
        label << tmpstr << " state " << value;
      else
        label << tmpstr;
    }
    else if ( infoType == EVENTTYPE_TYPE )
    {
      string tmpstr;
      if ( !whichWindow->getTrace()->getEventLabels().getEventTypeLabel( value, tmpstr ) )
        label << tmpstr << " type " << value;
      else
        label << tmpstr;
    }
    else if ( infoType == EVENTVALUE_TYPE )
    {
      string tmpstr;
      if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( value, tmpstr ) )
        label << tmpstr << " type " << value;
      else
        label << tmpstr;
    }
    else if ( infoType == COMMSIZE_TYPE )
      label << value << " bytes";
    else if ( infoType == COMMTAG_TYPE )
      label << value;
    else if ( infoType == BANDWIDTH_TYPE )
      label << value << " bytes/sec";
    else
      label << "unknown " << value;
  }

  return label.str();
}
