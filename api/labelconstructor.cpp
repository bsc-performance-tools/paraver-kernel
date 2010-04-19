/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <math.h>
#include <locale>
#include <boost/lexical_cast.hpp>
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "histogram.h"
#include "paraverconfig.h"
#include "window.h"
#include "filter.h"
#include "kfilter.h"

stringstream LabelConstructor::label;
stringstream LabelConstructor::columnLabel;
stringstream LabelConstructor::tmp;
stringstream LabelConstructor::sstrTimeLabel;
stringstream LabelConstructor::sstrSemanticLabel;
char LabelConstructor::separator;
char LabelConstructor::point;

void LabelConstructor::init()
{
  locale myLocale( "" );
  point = use_facet<numpunct<char> >(myLocale).decimal_point();
  label.imbue( myLocale );
  columnLabel.imbue( myLocale );
  tmp.imbue( myLocale );
  sstrTimeLabel.imbue( myLocale );
  sstrSemanticLabel.imbue( myLocale );

  if ( point == ',' )
    separator = '.';
  else
    separator = ',';
}


string LabelConstructor::objectLabel( TObjectOrder globalOrder,
                                      TWindowLevel level,
                                      Trace *whichTrace )
{
  string rowStr( whichTrace->getRowLabel( level, globalOrder ) );
  label.clear();
  label.str( "" );

  if ( rowStr != "" )
    return rowStr;

  if ( level == THREAD )
  {
    TApplOrder appl;
    TTaskOrder task;
    TThreadOrder thread;
    whichTrace->getThreadLocation( globalOrder, appl, task, thread );
    label << LEVEL_THREAD << ' ' << appl + 1 << '.' << task + 1 << '.' << thread + 1;
  }
  else if ( level == TASK )
  {
    TApplOrder appl;
    TTaskOrder task;
    whichTrace->getTaskLocation( globalOrder, appl, task );
    label << LEVEL_TASK << ' ' << appl + 1 << '.' << task + 1;
  }
  else if ( level == APPLICATION )
    label << LEVEL_APPLICATION << ' ' << globalOrder + 1;
  else if ( level == WORKLOAD )
    label << LEVEL_WORKLOAD;
  else if ( level == CPU )
  {
    TNodeOrder node;
    TCPUOrder cpu;
    whichTrace->getCPULocation( globalOrder, node, cpu );
    label << LEVEL_CPU << ' ' << node + 1 << '.' << cpu + 1;
  }
  else if ( level == NODE )
    label << LEVEL_NODE << ' ' << globalOrder + 1;
  else if ( level == SYSTEM )
    label << LEVEL_SYSTEM;

  return label.str();
}


string LabelConstructor::histoColumnLabel( THistogramColumn whichColumn,
    const Window *whichWindow,
    THistogramLimit min,
    THistogramLimit max,
    THistogramLimit delta )
{
  columnLabel.clear();
  columnLabel.str( "" );
  double tmp;

  if ( modf( min, &tmp ) != 0.0 || delta != 1.0 )
  {
    // Column range values
    columnLabel << '[' << ( whichColumn * delta ) + min << "..";
    if ( ( ( whichColumn * delta ) + min + delta ) >= max )
    {
      columnLabel << max;
      columnLabel << ']';
    }
    else
    {
      columnLabel << ( whichColumn * delta ) + min + delta;
      columnLabel << ')';
    }
  }
  else
  {
    // Discrete integer value
    columnLabel << LabelConstructor::semanticLabel( whichWindow,
        ( whichColumn * delta ) + min, true, ParaverConfig::getInstance()->getHistogramPrecision() );
  }

  return columnLabel.str();
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
  label.clear();
  label.str( "" );

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
        strNum = chomp( intValue ) + separator + strNum;
      strNum.erase( strNum.size() - 1, 1 );
      label << strNum;
    }

    tmp.clear();
    tmp.str( "" );
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

string LabelConstructor::numberWithSeparators( TSemanticValue value, UINT32 precision, TTimeUnit unit )
{
  label.clear();
  label.str( "" );

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
        strNum = chomp( intValue ) + separator + strNum;
      strNum.erase( strNum.size() - 1, 1 );
      label << strNum;
    }

    tmp.clear();
    tmp.str( "" );
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
  sstrTimeLabel.clear();
  sstrTimeLabel.str( "" );

  sstrTimeLabel << fixed;
  sstrTimeLabel.precision( precision );

  sstrTimeLabel << numberWithSeparators( value, precision, unit );

  sstrTimeLabel << " " << LABEL_TIMEUNIT[ unit ];

  return sstrTimeLabel.str();
}


bool LabelConstructor::getTimeValue( const string& timeLabel,
                                     TTimeUnit unit, UINT32 precision,
                                     TTime& value )
{
  bool done = false;
  sstrTimeLabel.clear();
  sstrTimeLabel.str( "" );

  if ( !sstrTimeLabel.eof() )
  {
    string sub;

    sstrTimeLabel << timeLabel;
    tmp.clear();
    tmp.str( "" );

    while ( !sstrTimeLabel.eof() )
    {
      std::getline( sstrTimeLabel, sub, separator );
      tmp << sub;
    }

    if( !( tmp >> value ) )
      done = false;
    else
      done = true;
  }

  return done;
}


string LabelConstructor::semanticLabel( const Window * whichWindow,
                                        TSemanticValue value,
                                        bool text, UINT32 precision )
{
  sstrSemanticLabel.clear();
  sstrSemanticLabel.str( "" );
  SemanticInfoType infoType = whichWindow->getSemanticInfoType();

  sstrSemanticLabel << fixed;
  if ( ( value - INT64( value ) ) > 0.0 )
    sstrSemanticLabel.precision( precision );
  else
    sstrSemanticLabel.precision( 0 );

  if ( infoType == NO_TYPE || !text )
  {
    sstrSemanticLabel << numberWithSeparators( value, precision );
  }
  else
  {
    if ( infoType == OBJECT_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, whichWindow->getLevel(),
                                                          whichWindow->getTrace() );
    else if ( infoType == APPL_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, APPLICATION,
                                                          whichWindow->getTrace() );
    else if ( infoType == TASK_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, TASK,
                                                          whichWindow->getTrace() );
    else if ( infoType == THREAD_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, THREAD,
                                                          whichWindow->getTrace() );
    else if ( infoType == NODE_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, NODE,
                                                          whichWindow->getTrace() );
    else if ( infoType == CPU_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value, CPU,
                                                          whichWindow->getTrace() );
    else if ( infoType == TIME_TYPE )
      sstrSemanticLabel << LabelConstructor::timeLabel( value, whichWindow->getTimeUnit(), precision );
    else if ( infoType == STATE_TYPE )
    {
      string tmpstr;
      if ( !whichWindow->getTrace()->getStateLabels().getStateLabel( value, tmpstr ) )
        sstrSemanticLabel << tmpstr << " state " << value;
      else
        sstrSemanticLabel << tmpstr;
    }
    else if ( infoType == EVENTTYPE_TYPE )
    {
      string tmpstr;
      if ( !whichWindow->getTrace()->getEventLabels().getEventTypeLabel( value, tmpstr ) )
        sstrSemanticLabel << tmpstr << " type " << value;
      else
        sstrSemanticLabel << tmpstr;
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
          sstrSemanticLabel << tmpstr;
          break;
        }
      }
      if ( !found )
      {
        if ( types.begin() == types.end() )
        {
          if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( value, tmpstr ) )
            sstrSemanticLabel << tmpstr << " value " << value;
          else
            sstrSemanticLabel << tmpstr;
        }
        else
          sstrSemanticLabel << EventLabels::unknownLabel << " value " << value;
      }
    }
    else if ( infoType == COMMSIZE_TYPE )
      sstrSemanticLabel << value << " bytes";
    else if ( infoType == COMMTAG_TYPE )
      sstrSemanticLabel << value;
    else if ( infoType == BANDWIDTH_TYPE )
    {
      if ( whichWindow->getTimeUnit() == NS )
        sstrSemanticLabel << value << " GB/sec";
      else if ( whichWindow->getTimeUnit() == US )
        sstrSemanticLabel << value << " MB/sec";
      else if ( whichWindow->getTimeUnit() == MS )
        sstrSemanticLabel << value << " KB/sec";
      else if ( whichWindow->getTimeUnit() == SEC )
        sstrSemanticLabel << value << " bytes/sec";
      else if ( whichWindow->getTimeUnit() == MIN )
        sstrSemanticLabel << value << " bytes/min";
      else if ( whichWindow->getTimeUnit() == HOUR )
        sstrSemanticLabel << value << " bytes/hour";
      else if ( whichWindow->getTimeUnit() == DAY )
        sstrSemanticLabel << value << " bytes/day";
    }
    else
      sstrSemanticLabel << "unknown " << value;
  }

  return sstrSemanticLabel.str();
}


string LabelConstructor::eventLabel( Window *whichWindow,
                                     TEventType whichType,
                                     TEventValue whichValue,
                                     bool text )
{
  label.clear();
  label.str( "" );
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
  switch ( groupID )
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
