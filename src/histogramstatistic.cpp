/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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
#include "histogramstatistic.h"
#include "khistogram.h"
#include "kwindow.h"
#include "ktrace.h"
#include "paraverlabels.h"

int Statistics::numCommStats = 10;
int Statistics::numStats = 15;

//-------------------------------------------------------------------------
// Statistics filtering functions
//-------------------------------------------------------------------------
inline bool filterCommunication( RecordList::iterator& comm, KHistogram *histogram )
{
  TCommSize size = comm->getCommSize();
  TCommTag tag = comm->getCommTag();
  return size >= histogram->getCommSizeMin() &&
         size <= histogram->getCommSizeMax() &&
         tag >= histogram->getCommTagMin() &&
         tag <= histogram->getCommTagMax();
}

inline bool filterSemanticValue( TSemanticValue value, KHistogram *histogram )
{
  return value >= histogram->getDataMin() &&
         value <= histogram->getDataMax();
}

inline bool filterBurstTime( TRecordTime burstTime, KHistogram *histogram )
{
  return burstTime >= histogram->getBurstMin() &&
         burstTime <= histogram->getBurstMax();
}

vector<TSemanticValue> Statistics::zeroVector;
vector<vector<TSemanticValue> > Statistics::zeroMatrix;
vector<vector<TSemanticValue> > Statistics::zeroCommMatrix;

int Statistics::getNumCommStats()
{
  return Statistics::numCommStats;
}

int Statistics::getNumStats()
{
  return Statistics::numStats;
}

void Statistics::initAllComm( KHistogram *whichHistogram )
{
  zeroCommMatrix.clear();
  for ( THistogramColumn iPlane = 0; iPlane < whichHistogram->getNumPlanes(); ++iPlane )
    zeroCommMatrix.push_back( vector<TSemanticValue>( whichHistogram->getControlWindow()->getWindowLevelObjects(), 0.0 ) );

  statNumSends.init( whichHistogram );
  statNumReceives.init( whichHistogram );
  statBytesSent.init( whichHistogram );
  statBytesReceived.init( whichHistogram );
  statAvgBytesSent.init( whichHistogram );
  statAvgBytesReceived.init( whichHistogram );
  statMinBytesSent.init( whichHistogram );
  statMinBytesReceived.init( whichHistogram );
  statMaxBytesSent.init( whichHistogram );
  statMaxBytesReceived.init( whichHistogram );
}

void Statistics::resetAllComm()
{
  statNumSends.reset();
  statNumReceives.reset();
  statBytesSent.reset();
  statBytesReceived.reset();
  statAvgBytesSent.reset();
  statAvgBytesReceived.reset();
  statMinBytesSent.reset();
  statMinBytesReceived.reset();
  statMaxBytesSent.reset();
  statMaxBytesReceived.reset();
}

vector<bool> Statistics::filterAllComm( CalculateData *data )
{
  vector<bool> values;

  values.push_back( statNumSends.filter( data ) );
  values.push_back( statNumReceives.filter( data ) );
  values.push_back( statBytesSent.filter( data ) );
  values.push_back( statBytesReceived.filter( data ) );
  values.push_back( statAvgBytesSent.filter( data ) );
  values.push_back( statAvgBytesReceived.filter( data ) );
  values.push_back( statMinBytesSent.filter( data ) );
  values.push_back( statMinBytesReceived.filter( data ) );
  values.push_back( statMaxBytesSent.filter( data ) );
  values.push_back( statMaxBytesReceived.filter( data ) );

  return values;
}

vector<TSemanticValue> Statistics::executeAllComm( CalculateData *data )
{
  vector<TSemanticValue> values;

  values.push_back( statNumSends.execute( data ) );
  values.push_back( statNumReceives.execute( data ) );
  values.push_back( statBytesSent.execute( data ) );
  values.push_back( statBytesReceived.execute( data ) );
  values.push_back( statAvgBytesSent.execute( data ) );
  values.push_back( statAvgBytesReceived.execute( data ) );
  values.push_back( statMinBytesSent.execute( data ) );
  values.push_back( statMinBytesReceived.execute( data ) );
  values.push_back( statMaxBytesSent.execute( data ) );
  values.push_back( statMaxBytesReceived.execute( data ) );

  return values;
}

vector<TSemanticValue> Statistics::finishRowAllComm( vector<TSemanticValue>& cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  vector<TSemanticValue> values;
  size_t i = 0;

  values.push_back( statNumSends.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statNumReceives.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statBytesSent.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statBytesReceived.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgBytesSent.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgBytesReceived.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statMinBytesSent.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statMinBytesReceived.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statMaxBytesSent.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statMaxBytesReceived.finishRow( cellValue[ i ], column, plane ) );
  ++i;

  return values;
}

void Statistics::initAll( KHistogram *whichHistogram )
{
  zeroMatrix.clear();
  zeroVector.clear();
  for ( THistogramColumn iPlane = 0; iPlane < whichHistogram->getNumPlanes(); ++iPlane )
  {
    zeroMatrix.push_back( vector<TSemanticValue>( whichHistogram->getNumColumns(), 0.0 ) );
    zeroVector.push_back( 0.0 );
  }

  statTime.init( whichHistogram );
  statPercTime.init( whichHistogram );
  statPercTimeNotZero.init( whichHistogram );
  statPercTimeWindow.init( whichHistogram );
  statNumBursts.init( whichHistogram );
  statPercNumBursts.init( whichHistogram );
  statIntegral.init( whichHistogram );
  statAvgValue.init( whichHistogram );
  statMaximum.init( whichHistogram );
  statAvgBurstTime.init( whichHistogram );
  statStdevBurstTime.init( whichHistogram );
  statAvgPerBurst.init( whichHistogram );
  statAvgValueNotZero.init( whichHistogram );
  statNumBurstsNotZero.init( whichHistogram );
  statSumBursts.init( whichHistogram );
}

void Statistics::resetAll()
{
  statTime.reset();
  statPercTime.reset();
  statPercTimeNotZero.reset();
  statPercTimeWindow.reset();
  statNumBursts.reset();
  statPercNumBursts.reset();
  statIntegral.reset();
  statAvgValue.reset();
  statMaximum.reset();
  statAvgBurstTime.reset();
  statStdevBurstTime.reset();
  statAvgPerBurst.reset();
  statAvgValueNotZero.reset();
  statNumBurstsNotZero.reset();
  statSumBursts.reset();
}

vector<bool> Statistics::filterAll( CalculateData *data )
{
  vector<bool> values;

  values.push_back( statTime.filter( data ) );
  values.push_back( statPercTime.filter( data ) );
  values.push_back( statPercTimeNotZero.filter( data ) );
  values.push_back( statPercTimeWindow.filter( data ) );
  values.push_back( statNumBursts.filter( data ) );
  values.push_back( statPercNumBursts.filter( data ) );
  values.push_back( statIntegral.filter( data ) );
  values.push_back( statAvgValue.filter( data ) );
  values.push_back( statMaximum.filter( data ) );
  values.push_back( statAvgBurstTime.filter( data ) );
  values.push_back( statStdevBurstTime.filter( data ) );
  values.push_back( statAvgPerBurst.filter( data ) );
  values.push_back( statAvgValueNotZero.filter( data ) );
  values.push_back( statNumBurstsNotZero.filter( data ) );
  values.push_back( statSumBursts.filter( data ) );

  return values;
}

vector<TSemanticValue> Statistics::executeAll( CalculateData *data )
{
  vector<TSemanticValue> values;

  values.push_back( statTime.execute( data ) );
  values.push_back( statPercTime.execute( data ) );
  values.push_back( statPercTimeNotZero.execute( data ) );
  values.push_back( statPercTimeWindow.execute( data ) );
  values.push_back( statNumBursts.execute( data ) );
  values.push_back( statPercNumBursts.execute( data ) );
  values.push_back( statIntegral.execute( data ) );
  values.push_back( statAvgValue.execute( data ) );
  values.push_back( statMaximum.execute( data ) );
  values.push_back( statAvgBurstTime.execute( data ) );
  values.push_back( statStdevBurstTime.execute( data ) );
  values.push_back( statAvgPerBurst.execute( data ) );
  values.push_back( statAvgValueNotZero.execute( data ) );
  values.push_back( statNumBurstsNotZero.execute( data ) );
  values.push_back( statSumBursts.execute( data ) );

  return values;
}

vector<TSemanticValue> Statistics::finishRowAll( vector<TSemanticValue>& cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  vector<TSemanticValue> values;
  size_t i = 0;

  values.push_back( statTime.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statPercTime.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statPercTimeNotZero.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statPercTimeWindow.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statNumBursts.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statPercNumBursts.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statIntegral.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgValue.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statMaximum.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgBurstTime.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statStdevBurstTime.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgPerBurst.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statAvgValueNotZero.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statNumBurstsNotZero.finishRow( cellValue[ i ], column, plane ) );
  ++i;
  values.push_back( statSumBursts.finishRow( cellValue[ i ], column, plane ) );
  ++i;

  return values;
}

//-------------------------------------------------------------------------
// Histogram Statistic: #Sends
//-------------------------------------------------------------------------
string StatNumSends::name = "#Sends";

TObjectOrder StatNumSends::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatNumSends::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatNumSends::reset()
{}

bool StatNumSends::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatNumSends::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
    return 1;
  return 0;
}

TSemanticValue StatNumSends::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        THistogramColumn plane )
{
  return cellValue;
}

string StatNumSends::getName() const
{
  return StatNumSends::name;
}

string StatNumSends::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatNumSends::clone()
{
  return new StatNumSends( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: #Receives
//-------------------------------------------------------------------------
string StatNumReceives::name = "#Receives";

TObjectOrder StatNumReceives::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatNumReceives::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatNumReceives::reset()
{}

bool StatNumReceives::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatNumReceives::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
    return 1;
  return 0;
}

TSemanticValue StatNumReceives::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatNumReceives::getName() const
{
  return StatNumReceives::name;
}

string StatNumReceives::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatNumReceives::clone()
{
  return new StatNumReceives( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Bytes Sent
//-------------------------------------------------------------------------
string StatBytesSent::name = "Bytes sent";

TObjectOrder StatBytesSent::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatBytesSent::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatBytesSent::reset()
{}

bool StatBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
    return data->comm->getCommSize();
  return 0;
}

TSemanticValue StatBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatBytesSent::getName() const
{
  return StatBytesSent::name;
}

string StatBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatBytesSent::clone()
{
  return new StatBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Bytes Received
//-------------------------------------------------------------------------
string StatBytesReceived::name = "Bytes received";

TObjectOrder StatBytesReceived::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatBytesReceived::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatBytesReceived::reset()
{}

bool StatBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
    return data->comm->getCommSize();
  return 0;
}

TSemanticValue StatBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatBytesReceived::getName() const
{
  return StatBytesReceived::name;
}

string StatBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatBytesReceived::clone()
{
  return new StatBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average bytes sent
//-------------------------------------------------------------------------
string StatAvgBytesSent::name = "Average bytes sent";

TObjectOrder StatAvgBytesSent::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatAvgBytesSent::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  numComms = Statistics::zeroCommMatrix;
}

void StatAvgBytesSent::reset()
{
  numComms = Statistics::zeroCommMatrix;
}

bool StatAvgBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatAvgBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    ++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
    return data->comm->getCommSize();
  }
  return 0;
}

TSemanticValue StatAvgBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

string StatAvgBytesSent::getName() const
{
  return StatAvgBytesSent::name;
}

string StatAvgBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgBytesSent::clone()
{
  return new StatAvgBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average bytes received
//-------------------------------------------------------------------------
string StatAvgBytesReceived::name = "Average bytes received";

TObjectOrder StatAvgBytesReceived::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatAvgBytesReceived::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  numComms = Statistics::zeroCommMatrix;
}

void StatAvgBytesReceived::reset()
{
  numComms = Statistics::zeroCommMatrix;
}

bool StatAvgBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatAvgBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    ++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
    return data->comm->getCommSize();
  }
  return 0;
}

TSemanticValue StatAvgBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

string StatAvgBytesReceived::getName() const
{
  return StatAvgBytesReceived::name;
}

string StatAvgBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgBytesReceived::clone()
{
  return new StatAvgBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Minimum bytes sent
//-------------------------------------------------------------------------
string StatMinBytesSent::name = "Minimum bytes sent";

TObjectOrder StatMinBytesSent::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatMinBytesSent::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  min = Statistics::zeroCommMatrix;
}

void StatMinBytesSent::reset()
{
  min = Statistics::zeroCommMatrix;
}

bool StatMinBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatMinBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    if ( ( ( min[ data->plane ] )[ getPartner( data ) ] ) == 0.0 )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    else if ( data->comm->getCommSize() <
              ( ( min[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    return 1;
  }
  return 0;
}

TSemanticValue StatMinBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( min[ plane ] )[ column ] );
}

string StatMinBytesSent::getName() const
{
  return StatMinBytesSent::name;
}

string StatMinBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMinBytesSent::clone()
{
  return new StatMinBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Minimum bytes received
//-------------------------------------------------------------------------
string StatMinBytesReceived::name = "Minimum bytes received";

TObjectOrder StatMinBytesReceived::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatMinBytesReceived::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  min = Statistics::zeroCommMatrix;
}

void StatMinBytesReceived::reset()
{
  min = Statistics::zeroCommMatrix;
}

bool StatMinBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatMinBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    if ( ( ( min[ data->plane ] )[ getPartner( data ) ] ) == 0.0 )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    else if ( data->comm->getCommSize() <
              ( ( min[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    return 1;
  }
  return 0;
}

TSemanticValue StatMinBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( min[ plane ] )[ column ] );
}

string StatMinBytesReceived::getName() const
{
  return StatMinBytesReceived::name;
}

string StatMinBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMinBytesReceived::clone()
{
  return new StatMinBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum bytes sent
//-------------------------------------------------------------------------
string StatMaxBytesSent::name = "Maximum bytes sent";

TObjectOrder StatMaxBytesSent::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatMaxBytesSent::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  max = Statistics::zeroCommMatrix;
}

void StatMaxBytesSent::reset()
{
  max = Statistics::zeroCommMatrix;
}

bool StatMaxBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatMaxBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    if ( data->comm->getCommSize() >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    return 1;
  }
  return 0;
}

TSemanticValue StatMaxBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( max[ plane ] )[ column ] );
}

string StatMaxBytesSent::getName() const
{
  return StatMaxBytesSent::name;
}

string StatMaxBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMaxBytesSent::clone()
{
  return new StatMaxBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum bytes received
//-------------------------------------------------------------------------
string StatMaxBytesReceived::name = "Maximum bytes received";

TObjectOrder StatMaxBytesReceived::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= WORKLOAD && controlWin->getLevel() <= THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() );
}

void StatMaxBytesReceived::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  max = Statistics::zeroCommMatrix;
}

void StatMaxBytesReceived::reset()
{
  max = Statistics::zeroCommMatrix;
}

bool StatMaxBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

TSemanticValue StatMaxBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    if ( data->comm->getCommSize() >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
    return 1;
  }
  return 0;
}

TSemanticValue StatMaxBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( max[ plane ] )[ column ] );
}

string StatMaxBytesReceived::getName() const
{
  return StatMaxBytesReceived::name;
}

string StatMaxBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMaxBytesReceived::clone()
{
  return new StatMaxBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Time
//-------------------------------------------------------------------------
string StatTime::name = "Time";

void StatTime::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatTime::reset()
{}

bool StatTime::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

TSemanticValue StatTime::execute( CalculateData *data )
{
  /*  TRecordTime begin;
    TRecordTime end;

    begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
            data->beginTime : controlWin->getBeginTime( data->controlRow );

    end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
          data->endTime : controlWin->getEndTime( data->controlRow );

    return end - begin;
  */

  return data->endTime - data->beginTime;
}

TSemanticValue StatTime::finishRow( TSemanticValue cellValue,
                                    THistogramColumn column,
                                    THistogramColumn plane )
{
  return controlWin->traceUnitsToWindowUnits( cellValue );
}

string StatTime::getName() const
{
  return StatTime::name;
}

string StatTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

HistogramStatistic *StatTime::clone()
{
  return new StatTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Time
//-------------------------------------------------------------------------
string StatPercTime::name = "% Time";

void StatPercTime::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  rowTotal = Statistics::zeroVector;
}

void StatPercTime::reset()
{
  rowTotal = Statistics::zeroVector;
}

bool StatPercTime::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

TSemanticValue StatPercTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
          data->beginTime : controlWin->getBeginTime( data->controlRow );

  end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
        data->endTime : controlWin->getEndTime( data->controlRow );

  if ( myHistogram->getThreeDimensions() )
    rowTotal[ data->plane ] += end - begin;
  else
    rowTotal[ 0 ] += end - begin;

  return end - begin;
}

TSemanticValue StatPercTime::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

string StatPercTime::getName() const
{
  return StatPercTime::name;
}

string StatPercTime::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

HistogramStatistic *StatPercTime::clone()
{
  return new StatPercTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Time Not Zero
//-------------------------------------------------------------------------
string StatPercTimeNotZero::name = "% Time Not Zero";

void StatPercTimeNotZero::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  rowTotal = Statistics::zeroVector;
}

void StatPercTimeNotZero::reset()
{
  rowTotal = Statistics::zeroVector;
}

bool StatPercTimeNotZero::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

TSemanticValue StatPercTimeNotZero::execute( CalculateData *data )
{
  if ( controlWin->getValue( data->controlRow ) != 0.0 )
  {
    TRecordTime begin;
    TRecordTime end;

    begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
            data->beginTime : controlWin->getBeginTime( data->controlRow );

    end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
          data->endTime : controlWin->getEndTime( data->controlRow );

    if ( myHistogram->getThreeDimensions() )
      rowTotal[ data->plane ] += end - begin;
    else
      rowTotal[ 0 ] += end - begin;

    return end - begin;
  }

  return 0.0;
}

TSemanticValue StatPercTimeNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

string StatPercTimeNotZero::getName() const
{
  return StatPercTimeNotZero::name;
}

string StatPercTimeNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

HistogramStatistic *StatPercTimeNotZero::clone()
{
  return new StatPercTimeNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Window Time
//-------------------------------------------------------------------------
string StatPercTimeWindow::name = "% Window Time";

void StatPercTimeWindow::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatPercTimeWindow::reset()
{
}

bool StatPercTimeWindow::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

TSemanticValue StatPercTimeWindow::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
          data->beginTime : controlWin->getBeginTime( data->controlRow );

  end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
        data->endTime : controlWin->getEndTime( data->controlRow );

  return end - begin;
}

TSemanticValue StatPercTimeWindow::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) /
         ( myHistogram->getEndTime() - myHistogram->getBeginTime() );
}

string StatPercTimeWindow::getName() const
{
  return StatPercTimeWindow::name;
}

string StatPercTimeWindow::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

HistogramStatistic *StatPercTimeWindow::clone()
{
  return new StatPercTimeWindow( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts
//-------------------------------------------------------------------------
string StatNumBursts::name = "# Bursts";

void StatNumBursts::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
}

void StatNumBursts::reset()
{
}

bool StatNumBursts::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > myHistogram->getDataWindow()->getBeginTime( data->dataRow ) ?
          data->beginTime : myHistogram->getDataWindow()->getBeginTime( data->dataRow );

  end = data->endTime < myHistogram->getDataWindow()->getEndTime( data->dataRow ) ?
        data->endTime : myHistogram->getDataWindow()->getEndTime( data->dataRow );

  return filterSemanticValue( myHistogram->getDataWindow()->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatNumBursts::execute( CalculateData *data )
{
  return 1.0;
}

TSemanticValue StatNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatNumBursts::getName() const
{
  return StatNumBursts::name;
}

string StatNumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatNumBursts::clone()
{
  return new StatNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % # Bursts
//-------------------------------------------------------------------------
string StatPercNumBursts::name = "% # Bursts";

void StatPercNumBursts::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  rowTotal = Statistics::zeroVector;
}

void StatPercNumBursts::reset()
{
  rowTotal = Statistics::zeroVector;
}

bool StatPercNumBursts::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > myHistogram->getDataWindow()->getBeginTime( data->dataRow ) ?
          data->beginTime : myHistogram->getDataWindow()->getBeginTime( data->dataRow );

  end = data->endTime < myHistogram->getDataWindow()->getEndTime( data->dataRow ) ?
        data->endTime : myHistogram->getDataWindow()->getEndTime( data->dataRow );

  return filterSemanticValue( myHistogram->getDataWindow()->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatPercNumBursts::execute( CalculateData *data )
{
  if ( myHistogram->getThreeDimensions() )
    rowTotal[ data->plane ] += 1.0;
  else
    rowTotal[ 0 ] += 1.0;

  return 1.0;
}

TSemanticValue StatPercNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

string StatPercNumBursts::getName() const
{
  return StatPercNumBursts::name;
}

string StatPercNumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

HistogramStatistic *StatPercNumBursts::clone()
{
  return new StatPercNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Integral
//-------------------------------------------------------------------------
string StatIntegral::name = "Integral";

void StatIntegral::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatIntegral::reset()
{
}

bool StatIntegral::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

TSemanticValue StatIntegral::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return ( end - begin ) * dataWin->getValue( data->dataRow );
}

TSemanticValue StatIntegral::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        THistogramColumn plane )
{
  return cellValue;
}

string StatIntegral::getName() const
{
  return StatIntegral::name;
}

string StatIntegral::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatIntegral::clone()
{
  return new StatIntegral( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value
//-------------------------------------------------------------------------
string StatAvgValue::name = "Average value";

void StatAvgValue::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
  numValues = Statistics::zeroMatrix;
}

void StatAvgValue::reset()
{
  numValues = Statistics::zeroMatrix;
}

bool StatAvgValue::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}


TSemanticValue StatAvgValue::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  ( numValues[ data->plane ] )[ data->column ] += ( end - begin );

  return dataWin->getValue( data->dataRow ) * ( end -begin );
}


TSemanticValue StatAvgValue::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}


string StatAvgValue::getName() const
{
  return StatAvgValue::name;
}


string StatAvgValue::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgValue::clone()
{
  return new StatAvgValue( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum
//-------------------------------------------------------------------------
string StatMaximum::name = "Maximum";

void StatMaximum::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
  max = Statistics::zeroMatrix;
}

void StatMaximum::reset()
{
  max = Statistics::zeroMatrix;
}

bool StatMaximum::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

TSemanticValue StatMaximum::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) >
       ( ( max[ data->plane ] )[ data->column ] ) )
    ( ( max[ data->plane ] )[ data->column ] ) = dataWin->getValue( data->dataRow );
  return 1;
}

TSemanticValue StatMaximum::finishRow( TSemanticValue cellValue,
                                       THistogramColumn column,
                                       THistogramColumn plane )
{
  return ( max[ plane ] )[ column ];
}

string StatMaximum::getName() const
{
  return StatMaximum::name;
}

string StatMaximum::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMaximum::clone()
{
  return new StatMaximum( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average Burst Time
//-------------------------------------------------------------------------
string StatAvgBurstTime::name = "Average Burst Time";

void StatAvgBurstTime::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
  dataWin = myHistogram->getDataWindow();
  numValues = Statistics::zeroMatrix;
}

void StatAvgBurstTime::reset()
{
  numValues = Statistics::zeroMatrix;
}

bool StatAvgBurstTime::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatAvgBurstTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  ++( ( numValues[ data->plane ] )[ data->column ] );

  return end - begin;
}

TSemanticValue StatAvgBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return controlWin->traceUnitsToWindowUnits( cellValue ) / ( numValues[ plane ] )[ column ];
}

string StatAvgBurstTime::getName() const
{
  return StatAvgBurstTime::name;
}

string StatAvgBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

HistogramStatistic *StatAvgBurstTime::clone()
{
  return new StatAvgBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Stdev Burst Time
//-------------------------------------------------------------------------
string StatStdevBurstTime::name = "Stdev Burst Time";

void StatStdevBurstTime::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
  numValues = Statistics::zeroMatrix;
  qValues = Statistics::zeroMatrix;
}

void StatStdevBurstTime::reset()
{
  numValues = Statistics::zeroMatrix;
  qValues = Statistics::zeroMatrix;
}

bool StatStdevBurstTime::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatStdevBurstTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  ++( ( numValues[ data->plane ] )[ data->column ] );
  ( ( qValues[ data->plane ] )[ data->column ] ) +=
    ( end - begin ) * ( end - begin );

  return end - begin;
}

TSemanticValue StatStdevBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  TSemanticValue tmp;
  TSemanticValue avgQValues = ( qValues[ plane ] )[ column ] /
                              ( numValues[ plane ] )[ column ];
  TSemanticValue avgQ = cellValue / ( numValues[ plane ] )[ column ];
  avgQ *= avgQ;

  tmp = avgQValues - avgQ;
  if ( tmp < 0.0 )
    tmp *= -1.0;
  return sqrt( tmp );
}

string StatStdevBurstTime::getName() const
{
  return StatStdevBurstTime::name;
}

string StatStdevBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

HistogramStatistic *StatStdevBurstTime::clone()
{
  return new StatStdevBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average per Burst
//-------------------------------------------------------------------------
string StatAvgPerBurst::name = "Average per Burst";

void StatAvgPerBurst::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
  numValues = Statistics::zeroMatrix;
}

void StatAvgPerBurst::reset()
{
  numValues = Statistics::zeroMatrix;
}

bool StatAvgPerBurst::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatAvgPerBurst::execute( CalculateData *data )
{
  ++( ( numValues[ data->plane ] )[ data->column ] );

  return dataWin->getValue( data->dataRow );
}

TSemanticValue StatAvgPerBurst::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgPerBurst::getName() const
{
  return StatAvgPerBurst::name;
}

string StatAvgPerBurst::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgPerBurst::clone()
{
  return new StatAvgPerBurst( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value != 0
//-------------------------------------------------------------------------
string StatAvgValueNotZero::name = "Average value != 0";

void StatAvgValueNotZero::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
  numValues = Statistics::zeroMatrix;
}

void StatAvgValueNotZero::reset()
{
  numValues = Statistics::zeroMatrix;
}

bool StatAvgValueNotZero::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

TSemanticValue StatAvgValueNotZero::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  if ( dataWin->getValue( data->dataRow ) != 0.0 )
    ( numValues[ data->plane ] )[ data->column ] += ( end - begin );

  return dataWin->getValue( data->dataRow ) * ( end -begin );
}

TSemanticValue StatAvgValueNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  if( ( numValues[ plane ] )[ column ] == 0 )
    return 0.0;
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgValueNotZero::getName() const
{
  return StatAvgValueNotZero::name;
}

string StatAvgValueNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgValueNotZero::clone()
{
  return new StatAvgValueNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts != 0
//-------------------------------------------------------------------------
string StatNumBurstsNotZero::name = "# Bursts != 0";

void StatNumBurstsNotZero::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatNumBurstsNotZero::reset()
{
}

bool StatNumBurstsNotZero::filter( CalculateData *data ) const
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram )
         &&
         filterBurstTime( end - begin, myHistogram );
}

TSemanticValue StatNumBurstsNotZero::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) != 0.0 )
    return 1.0;
  return 0.0;
}

TSemanticValue StatNumBurstsNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatNumBurstsNotZero::getName() const
{
  return StatNumBurstsNotZero::name;
}

string StatNumBurstsNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatNumBurstsNotZero::clone()
{
  return new StatNumBurstsNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Sum bursts
//-------------------------------------------------------------------------
string StatSumBursts::name = "Sum bursts";

void StatSumBursts::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatSumBursts::reset()
{
}

bool StatSumBursts::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

TSemanticValue StatSumBursts::execute( CalculateData *data )
{
  return dataWin->getValue( data->dataRow );
}

TSemanticValue StatSumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatSumBursts::getName() const
{
  return StatSumBursts::name;
}

string StatSumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatSumBursts::clone()
{
  return new StatSumBursts( *this );
}

