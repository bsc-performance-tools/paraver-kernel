#include <sstream>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "histogram.h"
#include "paraverconfig.h"
#include "window.h"
#include "filter.h"
#include "kfilter.h"

string LabelConstructor::objectLabel( TObjectOrder globalOrder,
                                      TWindowLevel level,
                                      Trace *whichTrace )
{
  string rowStr;
  stringstream label;

  rowStr = whichTrace->getRowLabel( level, globalOrder );
  if ( rowStr != "" )
    return rowStr;

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
    const Window *whichWindow,
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
    label << LabelConstructor::semanticLabel( whichWindow,
        ( whichColumn * delta ) + min, true, ParaverConfig::getInstance()->getHistogramPrecision() );
  }

  return label.str();
}

inline string chomp( TSemanticValue& number )
{
  TSemanticValue origValue = number;
  number /= 1000.0;
  number = floor( number );
  TSemanticValue remainder = origValue - number * 1000.0;
  INT32 intRemainder = INT32( remainder );

  if ( number == 0 )
    return boost::lexical_cast<std::string>( intRemainder );
  else if ( remainder > 99 )
    return boost::lexical_cast<std::string>( intRemainder );
  else if ( remainder > 9 )
    return "0" + boost::lexical_cast<std::string>( intRemainder );
  else if ( remainder > 0 )
    return "00" + boost::lexical_cast<std::string>( intRemainder );
  return "000";
}

string LabelConstructor::histoCellLabel( const Histogram *whichHisto,
    TSemanticValue value, bool showUnits )
{
  stringstream label;

  if ( value == numeric_limits<double>::infinity() )
    return "inf";

  if ( whichHisto->getScientificNotation() )
    label << scientific;
  else
    label << fixed;

  label.precision( ParaverConfig::getInstance()->getHistogramPrecision() );

  if ( value == 0 )
    label << "0";
  else if ( whichHisto->getThousandSeparator() &&
            !whichHisto->getScientificNotation() )
  {
    string strNum;
    TSemanticValue origValue = value;
    TSemanticValue intValue = floor( value );

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
    tmp.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
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

  if ( showUnits && whichHisto->getShowUnits() &&
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

string numberWithSeparators( TSemanticValue value, UINT32 precision, TTimeUnit unit = MS )
{
  stringstream label;

  string strNum;
  TSemanticValue origValue = value;
  TSemanticValue intValue = floor( value );

  if ( value == 0 )
    label << "0";
  else
  {
    if ( origValue >= 1.0 )
    {
      while ( intValue > 0.0 )
        strNum = chomp( intValue ) + "," + strNum;
      strNum.erase( strNum.size() - 1, 1 );
      label << strNum;
    }

    stringstream tmp;
    tmp << fixed;
    tmp.precision( precision );
    value -= INT64( origValue );
    if ( unit != NS && value > 0 )
    {
      tmp << value;
      strNum = tmp.str();
      if ( origValue >= 1.0 )
        strNum.erase( strNum.begin() );

      label << strNum;
    }
  }
  return label.str();
}
string LabelConstructor::timeLabel( TTime value, TTimeUnit unit, UINT32 precision )
{
  stringstream label;

  label << fixed;
  label.precision( precision );

  label << numberWithSeparators( value, precision, unit );

  label << " " << LABEL_TIMEUNIT[ unit ];

  return label.str();
}

string LabelConstructor::semanticLabel( const Window * whichWindow,
                                        TSemanticValue value,
                                        bool text, UINT32 precision )
{
  stringstream label;
  SemanticInfoType infoType = whichWindow->getSemanticInfoType();

  label << fixed;
  if ( ( value - INT64( value ) ) > 0.0 )
    label.precision( precision );
  else
    label.precision( 0 );

  if ( infoType == NO_TYPE || !text )
  {
    label << numberWithSeparators( value, precision );
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
      label << LabelConstructor::timeLabel( value, whichWindow->getTimeUnit(), precision );
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
      vector<TEventType> types;
      bool found = false;
      whichWindow->getFilter()->getValidEvents( types,
          whichWindow->getTrace()->getLoadedEvents() );

      for ( vector<TEventType>::iterator it = types.begin(); it != types.end(); ++it )
      {
        if ( whichWindow->getTrace()->getEventLabels().getEventValueLabel(
               ( *it ), value, tmpstr ) )
        {
          found = true;
          label << tmpstr;
          break;
        }
      }
      if ( !found )
      {
        if ( types.begin() == types.end() )
        {
          if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( value, tmpstr ) )
            label << tmpstr << " value " << value;
          else
            label << tmpstr;
        }
        else
          label << EventLabels::unknownLabel << " value " << value;
      }
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


string LabelConstructor::eventLabel( Window *whichWindow,
                                     TEventType whichType,
                                     TEventValue whichValue,
                                     bool text )
{
  stringstream label;
  string tmpstr;

  if ( !text )
    label << "Type is " << whichType;
  else
  {
    if ( !whichWindow->getTrace()->getEventLabels().getEventTypeLabel( whichType, tmpstr ) )
      label << tmpstr << " type " << whichType;
    else
      label << tmpstr;
  }

  if ( !text )
    label << " Value is " << whichValue;
  else
  {
    label << " ";
    if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( whichType, whichValue, tmpstr ) )
      label << tmpstr << " value " << whichValue;
    else
      label << tmpstr;
  }

  return label.str();
}

string LabelConstructor::eventValueLabel( Window *whichWindow,
                                          TEventType whichType,
                                          TEventValue whichValue )
{
  string tmpstr;

  if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( whichType, whichValue, tmpstr ) )
    return "";

  return tmpstr;
}


void LabelConstructor::getGUIGroupLabels( const TGroupID groupID, vector< string > &labels )
{
  switch( groupID )
  {
    case COLOR:
      labels.push_back( GUI_COLORS_CODE_COLOR );
      labels.push_back( GUI_COLORS_GRADIENT_COLOR );
      labels.push_back( GUI_COLORS_NOT_NULL_GRADIENT );
      break;

    case GRADIENT_FUNCTION:
      labels.push_back( GUI_GRADIENT_FUNCTION_LINEAR );
      labels.push_back( GUI_GRADIENT_FUNCTION_STEPS );
      labels.push_back( GUI_GRADIENT_FUNCTION_LOGARITHMIC );
      labels.push_back( GUI_GRADIENT_FUNCTION_EXPONENTIAL );
      break;

    case DRAWMODE:
      labels.push_back( GUI_DRAWMODE_LAST );
      labels.push_back( GUI_DRAWMODE_MAXIMUM );
      labels.push_back( GUI_DRAWMODE_MINIMUM_NOT_ZERO );
      labels.push_back( GUI_DRAWMODE_RANDOM );
      labels.push_back( GUI_DRAWMODE_RANDOM_NOT_ZERO );
      labels.push_back( GUI_DRAWMODE_AVERAGE );
      break;

    case PIXEL_SIZE:
      labels.push_back( GUI_PIXEL_SIZE_X1 );
      labels.push_back( GUI_PIXEL_SIZE_X2 );
      labels.push_back( GUI_PIXEL_SIZE_X4 );
      labels.push_back( GUI_PIXEL_SIZE_X8 );
      break;

    case IMAGE_FORMAT:
      labels.push_back( GUI_IMAGE_FORMAT_BMP );
      labels.push_back( GUI_IMAGE_FORMAT_JPEG );
      labels.push_back( GUI_IMAGE_FORMAT_PNG );
      labels.push_back( GUI_IMAGE_FORMAT_XPM );
      break;

    case TEXT_FORMAT:
      labels.push_back( GUI_TEXT_FORMAT_CSV );
      labels.push_back( GUI_TEXT_FORMAT_PLAIN );
      break;

    default:
      break;
  }
}
