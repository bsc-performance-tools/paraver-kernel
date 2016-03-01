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
#include <boost/lexical_cast.hpp>
#include "labelconstructor.h"
#include "paraverlabels.h"
#include "histogram.h"
#include "window.h"
#include "filter.h"
#include "kfilter.h"

using namespace std;

stringstream LabelConstructor::sstrObjectLabel;
stringstream LabelConstructor::label;
stringstream LabelConstructor::columnLabel;
stringstream LabelConstructor::tmp;
stringstream LabelConstructor::sstrTimeLabel;
stringstream LabelConstructor::sstrSemanticLabel;
string       LabelConstructor::rowStr;
char         LabelConstructor::separator;
char         LabelConstructor::point;
locale       LabelConstructor::myLocaleWithoutThousands;
locale       LabelConstructor::myLocaleWithThousands;

// custom numpunct with grouping:
struct numpunct_group : std::numpunct<char> {
  std::string do_grouping() const {return "\03";}
};

// custom numpunct without grouping:
struct numpunct_nogroup : std::numpunct<char> {
  std::string do_grouping() const {return "\0";}
};

void LabelConstructor::init()
{
  myLocaleWithoutThousands = locale( locale( "" ), new numpunct_nogroup );
  myLocaleWithThousands = locale( locale( "" ), new numpunct_group );
  point = use_facet<numpunct<char> >(myLocaleWithThousands).decimal_point();
  label.imbue( myLocaleWithThousands );
  columnLabel.imbue( myLocaleWithThousands );
  tmp.imbue( myLocaleWithThousands );
  sstrTimeLabel.imbue( myLocaleWithThousands );
  sstrSemanticLabel.imbue( myLocaleWithThousands );

  if ( point == ',' )
    separator = '.';
  else
    separator = ',';
}


string LabelConstructor::objectLabel( TObjectOrder globalOrder,
                                      TWindowLevel level,
                                      Trace *whichTrace,
                                      bool showLevelTag,
                                      bool showRowText )
{
  if( showRowText )
  {
    if( level >= WORKLOAD && level <= THREAD )
      rowStr = whichTrace->getRowLabel( level, globalOrder );
    else if( level >= SYSTEM && level <= CPU && globalOrder > 0 )
      rowStr = whichTrace->getRowLabel( level, globalOrder - 1 );
  }
  else
    rowStr = "";
  sstrObjectLabel.clear();
  sstrObjectLabel.str( "" );

  if ( rowStr != "" )
    return rowStr;

  if ( level == THREAD )
  {
    TApplOrder appl;
    TTaskOrder task;
    TThreadOrder thread;
    if( globalOrder >= whichTrace->totalThreads() )
      sstrObjectLabel << "Not valid thread: " << globalOrder + 1;
    else
    {
      whichTrace->getThreadLocation( globalOrder, appl, task, thread );
      if ( showLevelTag )
        sstrObjectLabel << LEVEL_THREAD << ' ' << appl + 1 << '.' << task + 1 << '.' << thread + 1;
      else
        sstrObjectLabel << appl + 1 << '.' << task + 1 << '.' << thread + 1;
    }
  }
  else if ( level == TASK )
  {
    TApplOrder appl;
    TTaskOrder task;
    if( globalOrder >= whichTrace->totalTasks() )
      sstrObjectLabel << "Not valid task: " << globalOrder + 1;
    else
    {
      whichTrace->getTaskLocation( globalOrder, appl, task );
      if ( showLevelTag )
        sstrObjectLabel << LEVEL_TASK << ' ' << appl + 1 << '.' << task + 1;
      else
        sstrObjectLabel << appl + 1 << '.' << task + 1;
    }
  }
  else if ( level == APPLICATION )
  {
    if( globalOrder >= whichTrace->totalApplications() )
      sstrObjectLabel << "Not valid application: " << globalOrder + 1;
    else
    {
      if ( showLevelTag )
        sstrObjectLabel << LEVEL_APPLICATION << ' ' << globalOrder + 1;
      else
        sstrObjectLabel << globalOrder + 1;
    }
  }
  else if ( level == WORKLOAD )
  {
    if ( showLevelTag )
      sstrObjectLabel << LEVEL_WORKLOAD;
  }
  else if ( level == CPU )
  {
    if( globalOrder == 0 )
    {
      if ( showLevelTag )
        sstrObjectLabel << LEVEL_CPU << ' ' << "0";
      else
        sstrObjectLabel << "0";
    }
    else
    {
      TNodeOrder node;
      TCPUOrder cpu;
      if( globalOrder > whichTrace->totalCPUs() )
        sstrObjectLabel << "Not valid CPU: " << globalOrder + 1;
      else
      {
        whichTrace->getCPULocation( globalOrder, node, cpu );
        if ( showLevelTag )
          sstrObjectLabel << LEVEL_CPU << ' ' << node + 1 << '.' << cpu + 1;
        else
          sstrObjectLabel << node + 1 << '.' << cpu + 1;
      }
    }
  }
  else if ( level == NODE )
  {
    if( globalOrder > whichTrace->totalNodes() )
      sstrObjectLabel << "Not valid node: " << globalOrder;
    else
    {
      if ( showLevelTag )
        sstrObjectLabel << LEVEL_NODE << ' ' << globalOrder;
      else
        sstrObjectLabel << globalOrder;
    }
  }
  else if ( level == SYSTEM )
  {
    if ( showLevelTag )
      sstrObjectLabel << LEVEL_SYSTEM;
  }

  return sstrObjectLabel.str();
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

  if ( ParaverConfig::getInstance()->getHistogramScientificNotation() )
    columnLabel << scientific;
  else
    columnLabel << fixed;

  if ( modf( min, &tmp ) != 0.0 || delta != 1.0 )
  {
    columnLabel.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
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
    columnLabel.precision( 0 );
    // Discrete integer value
    columnLabel << LabelConstructor::semanticLabel( whichWindow,
        ( whichColumn * delta ) + min, true, ParaverConfig::getInstance()->getHistogramPrecision() );
  }

  return columnLabel.str();
}


string LabelConstructor::histoCellLabel( const Histogram *whichHisto,
    TSemanticValue value, bool showUnits )
{
  label.clear();
  label.str( "" );

  if ( value == numeric_limits<double>::infinity() )
    return "inf";

  if ( ParaverConfig::getInstance()->getHistogramScientificNotation() )
    label << scientific;
  else
    label << fixed;

  double dummyInt;
  if( modf( value, &dummyInt ) != 0.0 )
    label.precision( ParaverConfig::getInstance()->getHistogramPrecision() );
  else
    label.precision( 0 );

  if ( ParaverConfig::getInstance()->getHistogramThousandSep() &&
       !ParaverConfig::getInstance()->getHistogramScientificNotation() )
    label.imbue( myLocaleWithThousands );
  else
    label.imbue( myLocaleWithoutThousands );

  label << value;

  if ( showUnits && ParaverConfig::getInstance()->getHistogramShowUnits() &&
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


string LabelConstructor::timeLabel( TTime value, TTimeUnit unit, PRV_UINT32 precision )
{
  sstrTimeLabel.clear();
  sstrTimeLabel.str( "" );

  sstrTimeLabel << fixed;
  if( unit == NS )
    sstrTimeLabel.precision( 0 );
  else
    sstrTimeLabel.precision( precision );

  sstrTimeLabel.imbue( myLocaleWithThousands );

  sstrTimeLabel << value;

  sstrTimeLabel << " " << LABEL_TIMEUNIT[ unit ];

  return sstrTimeLabel.str();
}


bool LabelConstructor::getTimeValue( const string& timeLabel,
                                     TTimeUnit unit, PRV_UINT32 precision,
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
                                        bool text, PRV_UINT32 precision )
{
  sstrSemanticLabel.clear();
  sstrSemanticLabel.str( "" );
  SemanticInfoType infoType = whichWindow->getSemanticInfoType();

  sstrSemanticLabel << fixed;
  double dummyInt;
  if ( modf( value, &dummyInt ) != 0.0 )
    sstrSemanticLabel.precision( precision );
  else
    sstrSemanticLabel.precision( 0 );

  sstrSemanticLabel.imbue( myLocaleWithThousands );

  if ( infoType == NO_TYPE || !text )
  {
    sstrSemanticLabel << value;
  }
  else
  {
    if ( infoType == OBJECT_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value - 1, whichWindow->getLevel(),
                                                          whichWindow->getTrace() );
    else if ( infoType == APPL_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value - 1, APPLICATION,
                                                          whichWindow->getTrace() );
    else if ( infoType == TASK_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value - 1, TASK,
                                                          whichWindow->getTrace() );
    else if ( infoType == THREAD_TYPE )
      sstrSemanticLabel << LabelConstructor::objectLabel( value - 1, THREAD,
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
        {
          whichWindow->getTrace()->getEventLabels().getEventTypeLabel( *( types.begin() ), tmpstr );
          sstrSemanticLabel << tmpstr << " value " << value;
        }
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

  label << fixed;
  label.precision( 0 );
  label.imbue( myLocaleWithThousands );

  if ( !text )
    label << "Type is " << whichType;
  else
  {
    if ( !whichWindow->getTrace()->getEventLabels().getEventTypeLabel( whichType, tmpstr ) )
    {
      // Shouldn't be like this, instead?
      //label << " type " << whichType;
      label << tmpstr << " type " << whichType;
    }
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


string LabelConstructor::eventTypeLabel( Window *whichWindow,
                                         TEventType whichType,
                                         bool text )
{
  label.clear();
  label.str( "" );
  string tmpstr;

  label << fixed;
  label.precision( 0 );
  label.imbue( myLocaleWithThousands );

  if ( !text )
    label << "Type is " << whichType;
  else
  {
    if ( !whichWindow->getTrace()->getEventLabels().getEventTypeLabel( whichType, tmpstr ) )
      label << tmpstr << " type " << whichType;
    else
      label << tmpstr;
  }

  return label.str();
}


string LabelConstructor::eventValueLabel( Window *whichWindow,
                                          TEventType whichType,
                                          TEventValue whichValue,
                                          bool writeValueAsPrefix )
{
  label.clear();
  label.str( "" );
  string tmpstr;

  label << fixed;
  label.precision( 0 );
  label.imbue( myLocaleWithThousands );

  if ( !whichWindow->getTrace()->getEventLabels().getEventValueLabel( whichType, whichValue, tmpstr ) )
    label << "";
  else if ( writeValueAsPrefix )
    label << whichValue << " " << tmpstr;
  else
    label << tmpstr;

  return label.str();
}


void LabelConstructor::getGUIGroupLabels( const TGroupID groupID, vector< string > &labels )
{
  switch ( groupID )
  {
    case COLOR:
      labels.push_back( GUI_COLORS_CODE_COLOR );
      labels.push_back( GUI_COLORS_GRADIENT_COLOR );
      labels.push_back( GUI_COLORS_NOT_NULL_GRADIENT );
      labels.push_back( GUI_COLORS_FUNCTION_LINE );
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
      labels.push_back( GUI_DRAWMODE_AVERAGE_NOT_ZERO );
      labels.push_back( GUI_DRAWMODE_MODE );
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
      labels.push_back( GUI_TEXT_FORMAT_GNUPLOT );
      break;

    case OBJECT_LABELS:
      labels.push_back( GUI_OBJECT_LABELS_ALL );
      labels.push_back( GUI_OBJECT_LABELS_SPACED );
      labels.push_back( GUI_OBJECT_LABELS_POWER2 );
      break;

    case OBJECT_AXIS:
      labels.push_back( GUI_OBJECT_AXIS_CURRENT );
      labels.push_back( GUI_OBJECT_AXIS_ALL );
      labels.push_back( GUI_OBJECT_AXIS_ZERO );
      labels.push_back( GUI_OBJECT_AXIS_FIVE );
      labels.push_back( GUI_OBJECT_AXIS_TEN );
      labels.push_back( GUI_OBJECT_AXIS_TWENTYFIVE );
      break;

    default:
      break;
  }
}


string LabelConstructor::getDate( bool reverseOrder )
{
  string resultString("");

  string currentDate( __DATE__ );
  string currentDay = currentDate.substr( 4, 2 );

  if ( currentDay.compare("??") != 0 )
  {
    stringstream reverseDate;
    string currentYear  = currentDate.substr( 7, 4 );
    string currentMonth = currentDate.substr( 0, 3 );
    string months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    stringstream auxDay( currentDay );
    int numericDay;
    auxDay >> numericDay;

    if ( reverseOrder )
    {
      reverseDate << currentYear;
      reverseDate << setfill('0');
      reverseDate << setw(2) << ( months.find( currentMonth ) / 3 ) + 1;
      reverseDate << setw(2) << numericDay;
    }
    else
    {
      reverseDate << setfill('0');
      reverseDate << setw(2) << numericDay;
      reverseDate << setw(2) << ( months.find( currentMonth ) / 3 ) + 1;
      reverseDate << currentYear;
    }

    resultString = reverseDate.str();
  }

  return resultString;
}


std::string LabelConstructor::getImageFileSuffix( const ParaverConfig::TImageFormat& format )
{
  string resultString;

  switch( format )
  {
    case ParaverConfig::BMP:
      resultString = string( "bmp" );
      break;

    case ParaverConfig::JPG:
      resultString = string( "jpg" );
      break;

    case ParaverConfig::PNG:
      resultString = string( "png" );
      break;

    case ParaverConfig::XPM:
      resultString = string( "xpm" );
      break;

    default:
      resultString = string( "png" );
      break;
  }

  return resultString;
}


std::string LabelConstructor::getDataFileSuffix( const ParaverConfig::TTextFormat& format )
{
  string resultString;

  switch( format )
  {
    case ParaverConfig::CSV:
      resultString = string( "csv" );
      break;

    case ParaverConfig::GNUPLOT:
      resultString = string( "gnuplot" );
      break;

    case ParaverConfig::PLAIN:
    default:
      resultString = string( "dat" );
      break;
  }

  return resultString;
}
