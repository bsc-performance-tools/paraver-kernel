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


#include <math.h>
#include <limits>
#include "histogramstatistic.h"
#include "khistogram.h"
#include "kwindow.h"
#include "ktrace.h"
#include "paraverlabels.h"

using namespace std;

//-------------------------------------------------------------------------
// Statistics filtering functions
//-------------------------------------------------------------------------
inline bool filterCommunication( RecordList::iterator& comm, const KHistogram& histogram )
{
  TCommSize size = comm->getCommSize();
  TCommTag tag = comm->getCommTag();
  return size >= histogram.getCommSizeMin() &&
         size <= histogram.getCommSizeMax() &&
         tag >= histogram.getCommTagMin() &&
         tag <= histogram.getCommTagMax();

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

#ifndef PARALLEL_ENABLED
vector<TSemanticValue> Statistics::zeroVector;
vector<vector<TSemanticValue> > Statistics::zeroMatrix;
vector<map<TObjectOrder, TSemanticValue> > Statistics::zeroCommMatrix;
#endif

Statistics::Statistics( const KHistogram& whichHistogram )
  : myHistogram( whichHistogram )
{}

void Statistics::initAllComm()
{
#ifndef PARALLEL_ENABLED
  zeroCommMatrix.clear();
  for ( THistogramColumn iPlane = 0; iPlane < myHistogram.getNumPlanes(); ++iPlane )
    zeroCommMatrix.push_back( map<TObjectOrder, TSemanticValue>() );
#endif

  statNumSends.init( myHistogram );
  statNumReceives.init( myHistogram );
  statBytesSent.init( myHistogram );
  statBytesReceived.init( myHistogram );
  statAvgBytesSent.init( myHistogram );
  statAvgBytesReceived.init( myHistogram );
  statMinBytesSent.init( myHistogram );
  statMinBytesReceived.init( myHistogram );
  statMaxBytesSent.init( myHistogram );
  statMaxBytesReceived.init( myHistogram );
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

bool Statistics::filterAllComm( CalculateData *data )
{
  return filterCommunication( data->comm, myHistogram );
}

void Statistics::executeAllComm( CalculateData *data, array<TSemanticValue, NUM_COMM_STATS>& onValues )
{
  TTimelinesData tmpTimes = fillTimelinesData( data );

  onValues = { statNumSends.execute( data, tmpTimes ) ,
               statNumReceives.execute( data, tmpTimes ),
               statBytesSent.execute( data, tmpTimes ),
               statBytesReceived.execute( data, tmpTimes ),
               statAvgBytesSent.execute( data, tmpTimes ),
               statAvgBytesReceived.execute( data, tmpTimes ),
               statMinBytesSent.execute( data, tmpTimes ),
               statMinBytesReceived.execute( data, tmpTimes ),
               statMaxBytesSent.execute( data, tmpTimes ),
               statMaxBytesReceived.execute( data, tmpTimes ) };
}

std::array<TSemanticValue, NUM_COMM_STATS> Statistics::finishRowAllComm( const array<TSemanticValue, NUM_COMM_STATS>& cellValue,
                                                                         THistogramColumn column,
                                                                         TObjectOrder row,
                                                                         THistogramColumn plane )
{
  array<TSemanticValue, NUM_COMM_STATS> values;
  size_t i = 0;

  values[ i ] = statNumSends.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statNumReceives.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statBytesSent.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statBytesReceived.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgBytesSent.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgBytesReceived.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMinBytesSent.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMinBytesReceived.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMaxBytesSent.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMaxBytesReceived.finishRow( cellValue[ i ], column, row, plane );

  return values;
}

void Statistics::initAll()
{
#ifndef PARALLEL_ENABLED
  zeroMatrix.clear();
  zeroVector.clear();
  for ( THistogramColumn iPlane = 0; iPlane < myHistogram.getNumPlanes(); ++iPlane )
  {
    zeroMatrix.push_back( vector<TSemanticValue>( myHistogram.getNumColumns(), 0.0 ) );
    zeroVector.push_back( 0.0 );
  }
#endif

  statTime.init( myHistogram );
  statPercTime.init( myHistogram );
  statPercTimeNotZero.init( myHistogram );
  statPercTimeWindow.init( myHistogram );
  statNumBursts.init( myHistogram );
  statPercNumBursts.init( myHistogram );
  statIntegral.init( myHistogram );
  statAvgValue.init( myHistogram );
  statMaximum.init( myHistogram );
  statMinimum.init ( myHistogram );
  statAvgBurstTime.init( myHistogram );
  statStdevBurstTime.init( myHistogram );
  statAvgPerBurst.init( myHistogram );
  statAvgValueNotZero.init( myHistogram );
  statAvgPerBurstNotZero.init( myHistogram );
  statNumBurstsNotZero.init( myHistogram );
  statSumBursts.init( myHistogram );
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
  statMinimum.reset();
  statAvgBurstTime.reset();
  statStdevBurstTime.reset();
  statAvgPerBurst.reset();
  statAvgValueNotZero.reset();
  statAvgPerBurstNotZero.reset();
  statNumBurstsNotZero.reset();
  statSumBursts.reset();
}

bool Statistics::filterAll( CalculateData *data )
{
  return true;
}

void Statistics::executeAll( CalculateData *data, array<TSemanticValue, NUM_SEMANTIC_STATS>& onValues, bool& isNotZeroValue )
{
  TTimelinesData tmpTimes = fillTimelinesData( data );

  TSemanticValue tmpVal = statAvgValueNotZero.execute( data, tmpTimes );
  isNotZeroValue = tmpVal != 0.0;

  onValues = { statTime.execute( data, tmpTimes ), // no
               statPercTime.execute( data, tmpTimes ), //si
               statPercTimeNotZero.execute( data, tmpTimes ),//su
               statPercTimeWindow.execute( data, tmpTimes ),//si
               statNumBursts.execute( data, tmpTimes ),// filter
               statPercNumBursts.execute( data, tmpTimes ),//filter
               statIntegral.execute( data, tmpTimes ),// si, data
               statAvgValue.execute( data, tmpTimes ),// si, data
               statMaximum.execute( data, tmpTimes ),//si, data
               statMinimum.execute( data, tmpTimes ),//no
               statAvgBurstTime.execute( data, tmpTimes ),//no
               statStdevBurstTime.execute( data, tmpTimes ),//si, data
               statAvgPerBurst.execute( data, tmpTimes ),//si, data
               tmpVal,
               statAvgPerBurstNotZero.execute( data, tmpTimes ),//no
               statNumBurstsNotZero.execute( data, tmpTimes ),//no
               statSumBursts.execute( data, tmpTimes ) };//no
}

array<TSemanticValue, NUM_SEMANTIC_STATS> Statistics::finishRowAll( const array<TSemanticValue, NUM_SEMANTIC_STATS>& cellValue,
                                                                    THistogramColumn column,
                                                                    TObjectOrder row,
                                                                    THistogramColumn plane )
{
  array<TSemanticValue, NUM_SEMANTIC_STATS> values;
  size_t i = 0;

  values[ i ] = statTime.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statPercTime.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statPercTimeNotZero.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statPercTimeWindow.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statNumBursts.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statPercNumBursts.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statIntegral.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgValue.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMaximum.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statMinimum.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgBurstTime.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statStdevBurstTime.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgPerBurst.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgValueNotZero.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statAvgPerBurstNotZero.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statNumBurstsNotZero.finishRow( cellValue[ i ], column, row, plane );
  ++i;
  values[ i ] = statSumBursts.finishRow( cellValue[ i ], column, row, plane );

  return values;
}

TTimelinesData Statistics::fillTimelinesData( CalculateData *data ) const
{
  TTimelinesData tmpTimes;

  Timeline *controlWin = myHistogram.getControlWindow();
  tmpTimes.controlValue = controlWin->getValue( data->controlRow );
  tmpTimes.controlBeginTime = data->beginTime > myHistogram.getClonedWindow( controlWin )->getBeginTime( data->controlRow ) ?
                              data->beginTime : myHistogram.getClonedWindow( controlWin )->getBeginTime( data->controlRow );

  tmpTimes.controlEndTime = data->endTime < myHistogram.getClonedWindow( controlWin )->getEndTime( data->controlRow ) ?
                            data->endTime : myHistogram.getClonedWindow( controlWin )->getEndTime( data->controlRow );

  Timeline *dataWin = myHistogram.getDataWindow();
  tmpTimes.dataValue = dataWin->getValue( data->dataRow );
  tmpTimes.dataBeginTime = data->beginTime > myHistogram.getClonedWindow( dataWin )->getBeginTime( data->dataRow ) ?
                           data->beginTime : myHistogram.getClonedWindow( dataWin )->getBeginTime( data->dataRow );

  tmpTimes.dataEndTime = data->endTime < myHistogram.getClonedWindow( dataWin )->getEndTime( data->dataRow ) ?
                          data->endTime : myHistogram.getClonedWindow( dataWin )->getEndTime( data->dataRow );

  return tmpTimes;
}


//-------------------------------------------------------------------------
// Histogram Statistic: #Sends
//-------------------------------------------------------------------------
string StatNumSends::name = "#Sends";

TObjectOrder StatNumSends::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatNumSends::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
}

void StatNumSends::reset()
{}

TSemanticValue StatNumSends::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & SEND )
    return 1;

  return 0;
}

TSemanticValue StatNumSends::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        TObjectOrder row,
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatNumReceives::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
}

void StatNumReceives::reset()
{}

TSemanticValue StatNumReceives::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & RECV )
    return 1;
  return 0;
}

TSemanticValue StatNumReceives::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatBytesSent::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
}

void StatBytesSent::reset()
{}

TSemanticValue StatBytesSent::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & SEND )
    return data->comm->getCommSize();
  return 0;
}

TSemanticValue StatBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatBytesReceived::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
}

void StatBytesReceived::reset()
{}

TSemanticValue StatBytesReceived::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & RECV )
    return data->comm->getCommSize();
  return 0;
}

TSemanticValue StatBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatAvgBytesSent::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  numComms = Statistics::zeroCommMatrix;
#else
  numComms = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgBytesSent::reset()
{
#ifndef PARALLEL_ENABLED
  numComms = Statistics::zeroCommMatrix;
#else
  delete numComms;
#endif
}

TSemanticValue StatAvgBytesSent::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & SEND )
  {
#ifndef PARALLEL_ENABLED
//    ++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
    if( numComms[ data->plane ].count( getPartner( data ) ) == 0 )
      ( numComms[ data->plane ] )[ getPartner( data ) ] = 0.0;
    ++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
#else
    numComms->addValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ 1.0 } );
#endif
    return data->comm->getCommSize();
  }
  return 0;
}

TSemanticValue StatAvgBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return cellValue / ( numComms[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numComms->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatAvgBytesReceived::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  numComms = Statistics::zeroCommMatrix;
#else
  numComms = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgBytesReceived::reset()
{
#ifndef PARALLEL_ENABLED
  numComms = Statistics::zeroCommMatrix;
#else
  delete numComms;
#endif
}

TSemanticValue StatAvgBytesReceived::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & RECV )
  {
#ifndef PARALLEL_ENABLED
    //++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
    if( numComms[ data->plane ].count( getPartner( data ) ) == 0 )
      ( numComms[ data->plane ] )[ getPartner( data ) ] = 0.0;
    ++( ( numComms[ data->plane ] )[ getPartner( data ) ] );
#else
    numComms->addValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ 1.0 } );
#endif
    return data->comm->getCommSize();
  }
  return 0;
}

TSemanticValue StatAvgBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return cellValue / ( numComms[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numComms->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatMinBytesSent::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroCommMatrix;
#else
  min = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMinBytesSent::reset()
{
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroCommMatrix;
#else
  delete min;
#endif
}

TSemanticValue StatMinBytesSent::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & SEND )
  {
#ifndef PARALLEL_ENABLED
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
#else
    array< TSemanticValue, 1 > tmp;
    bool res = min->getCellValue( tmp, data->plane, data->row, getPartner( data ) );
    if( !res )
    {
      min->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
    else if( data->comm->getCommSize() < tmp[ 0 ] )
    {
      min->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
#endif
    return 1;
  }
  return 0;
}

TSemanticValue StatMinBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( ( min[ plane ] )[ column ] );
#else
  array< TSemanticValue, 1 > tmp;
  if( !min->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatMinBytesReceived::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroCommMatrix;
#else
  min = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMinBytesReceived::reset()
{
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroCommMatrix;
#else
  delete min;
#endif
}

TSemanticValue StatMinBytesReceived::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & RECV )
  {
#ifndef PARALLEL_ENABLED
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
#else
    array< TSemanticValue, 1 > tmp;
    bool res = min->getCellValue( tmp, data->plane, data->row, getPartner( data ) );
    if( !res )
    {
      min->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
    else if( data->comm->getCommSize() < tmp[ 0 ] )
    {
      min->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
#endif
    return 1;
  }
  return 0;
}

TSemanticValue StatMinBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( ( min[ plane ] )[ column ] );
#else
  array< TSemanticValue, 1 > tmp;
  if( !min->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatMaxBytesSent::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroCommMatrix;
#else
  max = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMaxBytesSent::reset()
{
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroCommMatrix;
#else
  delete max;
#endif
}

TSemanticValue StatMaxBytesSent::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & SEND )
  {
#ifndef PARALLEL_ENABLED
    if ( data->comm->getCommSize() >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
#else
    array< TSemanticValue, 1 > tmp;
    bool res = max->getCellValue( tmp, data->plane, data->row, getPartner( data ) );
    if( !res )
    {
      max->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
    else if( data->comm->getCommSize() > tmp[ 0 ] )
    {
      max->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }

#endif
    return 1;
  }
  return 0;
}

TSemanticValue StatMaxBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( ( max[ plane ] )[ column ] );
#else
  array< TSemanticValue, 1 > tmp;
  if( !max->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
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
  if ( controlWin->getLevel() >= TTraceLevel::WORKLOAD && controlWin->getLevel() <= TTraceLevel::THREAD )
    return controlWin->threadObjectToWindowObject( data->comm->getCommPartnerObject() );
  return controlWin->cpuObjectToWindowObject( data->comm->getCommPartnerObject() - 1 );
}

void StatMaxBytesReceived::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroCommMatrix;
#else
  max = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMaxBytesReceived::reset()
{
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroCommMatrix;
#else
  delete max;
#endif
}

TSemanticValue StatMaxBytesReceived::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( data->comm->getRecordType() & RECV )
  {
#ifndef PARALLEL_ENABLED
    if ( data->comm->getCommSize() >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        data->comm->getCommSize();
    }
#else
    array< TSemanticValue, 1 > tmp;
    bool res = max->getCellValue( tmp, data->plane, data->row, getPartner( data ) );
    if( !res )
    {
      max->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
    else if( data->comm->getCommSize() > tmp[ 0 ] )
    {
      max->setValue( data->plane, data->row, getPartner( data ), array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( data->comm->getCommSize() ) } );
    }
#endif
    return 1;
  }
  return 0;
}

TSemanticValue StatMaxBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( ( max[ plane ] )[ column ] );
#else
  array< TSemanticValue, 1 > tmp;
  if( !max->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
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

void StatTime::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
}

void StatTime::reset()
{}

TSemanticValue StatTime::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  return data->endTime - data->beginTime;
}

TSemanticValue StatTime::finishRow( TSemanticValue cellValue,
                                    THistogramColumn column,
                                    TObjectOrder row,
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
  TTimeUnit tu =  whichHisto->getTimeUnit();

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

void StatPercTime::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  rowTotal = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatPercTime::reset()
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  delete rowTotal;
#endif
}

TSemanticValue StatPercTime::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.controlBeginTime;
  TRecordTime end = timelinesData.controlEndTime;

#ifndef PARALLEL_ENABLED
  rowTotal[ data->plane ] += end - begin;
#else
  rowTotal->addValue( data->plane, data->row, 0, array< TSemanticValue, 1 >{ end - begin } );
#endif

  return end - begin;
}

TSemanticValue StatPercTime::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        TObjectOrder row,
                                        THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !rowTotal->getCellValue( tmp, plane, row, 0 ) )
    return 0.0;

  return ( cellValue * 100.0 ) / tmp[ 0 ];
#endif
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

void StatPercTimeNotZero::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  rowTotal = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatPercTimeNotZero::reset()
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  delete rowTotal;
#endif
}

TSemanticValue StatPercTimeNotZero::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( timelinesData.controlValue != 0.0 )
  {
    TRecordTime begin = timelinesData.controlBeginTime;
    TRecordTime end = timelinesData.controlEndTime;

#ifndef PARALLEL_ENABLED
    rowTotal[ data->plane ] += end - begin;
#else
    rowTotal->addValue( data->plane, data->row, 0, array< TSemanticValue, 1 >{ end - begin } );
#endif

    return end - begin;
  }

  return 0.0;
}

TSemanticValue StatPercTimeNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !rowTotal->getCellValue( tmp, plane, row, 0 ) )
    return 0.0;

  return ( cellValue * 100.0 ) / tmp[ 0 ];
#endif
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

void StatPercTimeWindow::init( const KHistogram& whichHistogram )
{
  histogramTotalDuration = whichHistogram.getEndTime() - whichHistogram.getBeginTime();
}

void StatPercTimeWindow::reset()
{
}

TSemanticValue StatPercTimeWindow::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.controlBeginTime;
  TRecordTime end = timelinesData.controlEndTime;

  return end - begin;
}

TSemanticValue StatPercTimeWindow::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / histogramTotalDuration;
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

void StatNumBursts::init( const KHistogram& whichHistogram )
{
}

void StatNumBursts::reset()
{
}

TSemanticValue StatNumBursts::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  return 1.0;
}

TSemanticValue StatNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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

void StatPercNumBursts::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  rowTotal = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatPercNumBursts::reset()
{
#ifndef PARALLEL_ENABLED
  rowTotal = Statistics::zeroVector;
#else
  delete rowTotal;
#endif
}

TSemanticValue StatPercNumBursts::execute( CalculateData *data, TTimelinesData& timelinesData )
{
#ifndef PARALLEL_ENABLED
  rowTotal[ data->plane ] += 1.0;
#else
  rowTotal->addValue( data->plane, data->row, 0, array< TSemanticValue, 1 >{ 1.0 } );
#endif

  return 1.0;
}

TSemanticValue StatPercNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !rowTotal->getCellValue( tmp, plane, row, 0 ) )
    return 0.0;

  return ( cellValue * 100.0 ) / tmp[ 0 ];
#endif
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

void StatIntegral::init( const KHistogram& whichHistogram )
{
  dataWin = whichHistogram.getDataWindow();
}

void StatIntegral::reset()
{
}

TSemanticValue StatIntegral::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.dataBeginTime;
  TRecordTime end = timelinesData.dataEndTime;
  TSemanticValue value = timelinesData.dataValue;
  value = value >= (TSemanticValue)0.0 ? value : -value;

  return dataWin->traceUnitsToWindowUnits( end - begin ) * value;
}

TSemanticValue StatIntegral::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        TObjectOrder row,
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

void StatAvgValue::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgValue::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  delete numValues;
#endif
}

TSemanticValue StatAvgValue::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.dataBeginTime;
  TRecordTime end = timelinesData.dataEndTime;

#ifndef PARALLEL_ENABLED
  ( numValues[ data->plane ] )[ data->column ] += ( end - begin );
#else
  numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ end - begin } );
#endif

  return timelinesData.dataValue * ( end -begin );
}


TSemanticValue StatAvgValue::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        TObjectOrder row,
                                        THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return cellValue / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numValues->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
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

void StatMaximum::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroMatrix;
#else
  max = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMaximum::reset()
{
#ifndef PARALLEL_ENABLED
  max = Statistics::zeroMatrix;
#else
  delete max;
#endif
}

TSemanticValue StatMaximum::execute( CalculateData *data, TTimelinesData& timelinesData )
{
#ifndef PARALLEL_ENABLED
  if ( timelinesData.dataValue >
       ( ( max[ data->plane ] )[ data->column ] ) )
    ( ( max[ data->plane ] )[ data->column ] ) = timelinesData.dataValue;
#else
  array< TSemanticValue, 1 > tmp;
  bool res = max->getCellValue( tmp, data->plane, data->row, data->column );
  if( !res )
  {
    max->setValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ timelinesData.dataValue } );
  }
  else if ( timelinesData.dataValue > tmp[ 0 ] )
  {
    max->setValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ timelinesData.dataValue } );
  }
#endif
  return 1;
}

TSemanticValue StatMaximum::finishRow( TSemanticValue cellValue,
                                       THistogramColumn column,
                                       TObjectOrder row,
                                       THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( max[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !max->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
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
// Histogram Statistic: Minimum
//-------------------------------------------------------------------------
string StatMinimum::name = "Minimum";

void StatMinimum::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroMatrix;
#else
  min = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatMinimum::reset()
{
#ifndef PARALLEL_ENABLED
  min = Statistics::zeroMatrix;
#else
  delete min;
#endif
}

TSemanticValue StatMinimum::execute( CalculateData *data, TTimelinesData& timelinesData )
{
#ifndef PARALLEL_ENABLED
  if ( ( ( min[ data->plane ] )[ data->column ] ) == 0.0 )
  {
    ( ( min[ data->plane ] )[ data->column ] ) = timelinesData.dataValue;
  }
  else if ( timelinesData.dataValue != 0.0 &&
            timelinesData.dataValue <
            ( ( min[ data->plane ] )[ data->column ] ) )
  {
     ( ( min[ data->plane ] )[ data->column ] ) = timelinesData.dataValue;
  }
#else
  array< TSemanticValue, 1 > tmp;
  bool res = min->getCellValue( tmp, data->plane, data->row, data->column );
  if( !res )
  {
    min->setValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ timelinesData.dataValue } );
  }
  else if ( timelinesData.dataValue < tmp[ 0 ] )
  {
    min->setValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ timelinesData.dataValue } );
  }
#endif
  return 1;
}

TSemanticValue StatMinimum::finishRow( TSemanticValue cellValue,
                                       THistogramColumn column,
                                       TObjectOrder row,
                                       THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return ( min[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !min->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return tmp[ 0 ];
#endif
}

string StatMinimum::getName() const
{
  return StatMinimum::name;
}

string StatMinimum::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatMinimum::clone()
{
  return new StatMinimum( *this );
}

//-------------------------------------------------------------------------
// Histogram Statistic: Average Burst Time
//-------------------------------------------------------------------------
string StatAvgBurstTime::name = "Average Burst Time";

void StatAvgBurstTime::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgBurstTime::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  delete numValues;
#endif
}

TSemanticValue StatAvgBurstTime::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.dataBeginTime;
  TRecordTime end = timelinesData.dataEndTime;

#ifndef PARALLEL_ENABLED
  ++( ( numValues[ data->plane ] )[ data->column ] );
#else
  numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ 1.0 } );
#endif

  return end - begin;
}

TSemanticValue StatAvgBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return controlWin->traceUnitsToWindowUnits( cellValue ) / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numValues->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return controlWin->traceUnitsToWindowUnits( cellValue ) / tmp[ 0 ];
#endif
}

string StatAvgBurstTime::getName() const
{
  return StatAvgBurstTime::name;
}

string StatAvgBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getTimeUnit();

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

void StatStdevBurstTime::init( const KHistogram& whichHistogram )
{
  controlWin = whichHistogram.getControlWindow();
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
  qValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
  qValues= new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatStdevBurstTime::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
  qValues = Statistics::zeroMatrix;
#else
  delete numValues;
  delete qValues;
#endif
}

TSemanticValue StatStdevBurstTime::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.dataBeginTime;
  TRecordTime end = timelinesData.dataEndTime;
  TRecordTime tmpDuration = end - begin;

  tmpDuration = controlWin->traceUnitsToWindowUnits( tmpDuration );

#ifndef PARALLEL_ENABLED
  ++( ( numValues[ data->plane ] )[ data->column ] );
  ( ( qValues[ data->plane ] )[ data->column ] ) +=
    tmpDuration * tmpDuration;
#else
  numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ 1.0 } );
  qValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ tmpDuration * tmpDuration } );
#endif

  return tmpDuration;
}

TSemanticValue StatStdevBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
  TSemanticValue tmpSemValue;

#ifndef PARALLEL_ENABLED
  TSemanticValue avgQValues = ( qValues[ plane ] )[ column ] /
                              ( numValues[ plane ] )[ column ];
  TSemanticValue avgQ = cellValue / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmpNumValues;
  if( !numValues->getCellValue( tmpNumValues, plane, row, column ) )
    return 0.0;

  array< TSemanticValue, 1 > tmpQValues;
  qValues->getCellValue( tmpQValues, plane, row, column );

  TSemanticValue avgQValues = tmpQValues[ 0 ] / tmpNumValues[ 0 ];
  TSemanticValue avgQ = cellValue / tmpNumValues[ 0 ];
#endif
  avgQ *= avgQ;

  tmpSemValue = avgQValues - avgQ;
  if ( tmpSemValue < 0.0 )
    tmpSemValue *= -1.0;
  return sqrt( tmpSemValue );
}

string StatStdevBurstTime::getName() const
{
  return StatStdevBurstTime::name;
}

string StatStdevBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getTimeUnit();

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

void StatAvgPerBurst::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgPerBurst::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  delete numValues;
#endif
}

TSemanticValue StatAvgPerBurst::execute( CalculateData *data, TTimelinesData& timelinesData )
{
#ifndef PARALLEL_ENABLED
  ++( ( numValues[ data->plane ] )[ data->column ] );
#else
  numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ 1.0 } );
#endif

  return timelinesData.dataValue;
}

TSemanticValue StatAvgPerBurst::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return cellValue / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numValues->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
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
// Histogram Statistic: Average per Burst != 0
//-------------------------------------------------------------------------
string StatAvgPerBurstNotZero::name = "Average per Burst != 0";

void StatAvgPerBurstNotZero::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgPerBurstNotZero::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  delete numValues;
#endif
}

TSemanticValue StatAvgPerBurstNotZero::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( timelinesData.dataValue != 0.0 )
#ifndef PARALLEL_ENABLED
    ++( ( numValues[ data->plane ] )[ data->column ] );
#else
    numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ 1.0 } );
#endif

  return timelinesData.dataValue;
}

TSemanticValue StatAvgPerBurstNotZero::finishRow( TSemanticValue cellValue,
                                                  THistogramColumn column,
                                                  TObjectOrder row,
                                                  THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  return cellValue / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numValues->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
}

string StatAvgPerBurstNotZero::getName() const
{
  return StatAvgPerBurstNotZero::name;
}

string StatAvgPerBurstNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

HistogramStatistic *StatAvgPerBurstNotZero::clone()
{
  return new StatAvgPerBurstNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value != 0
//-------------------------------------------------------------------------
string StatAvgValueNotZero::name = "Average value != 0";

void StatAvgValueNotZero::init( const KHistogram& whichHistogram )
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  numValues = new CubeBuffer<1>( whichHistogram->getNumPlanes(), whichHistogram->getNumRows() );
#endif
}

void StatAvgValueNotZero::reset()
{
#ifndef PARALLEL_ENABLED
  numValues = Statistics::zeroMatrix;
#else
  delete numValues;
#endif
}

TSemanticValue StatAvgValueNotZero::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  TRecordTime begin = timelinesData.dataBeginTime;
  TRecordTime end = timelinesData.dataEndTime;

  if ( timelinesData.dataValue != 0.0 )
#ifndef PARALLEL_ENABLED
    ( numValues[ data->plane ] )[ data->column ] += ( end - begin );
#else
    numValues->addValue( data->plane, data->row, data->column, array< TSemanticValue, 1 >{ static_cast<TSemanticValue>( end - begin ) } );
#endif

  return timelinesData.dataValue * ( end -begin );
}

TSemanticValue StatAvgValueNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
    THistogramColumn plane )
{
#ifndef PARALLEL_ENABLED
  if( ( numValues[ plane ] )[ column ] == 0 )
    return 0.0;
  return cellValue / ( numValues[ plane ] )[ column ];
#else
  array< TSemanticValue, 1 > tmp;
  if( !numValues->getCellValue( tmp, plane, row, column ) )
    return 0.0;

  return cellValue / tmp[ 0 ];
#endif
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

void StatNumBurstsNotZero::init( const KHistogram& whichHistogram )
{
}

void StatNumBurstsNotZero::reset()
{
}

TSemanticValue StatNumBurstsNotZero::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  if ( timelinesData.dataValue != 0.0 )
    return 1.0;
  return 0.0;
}

TSemanticValue StatNumBurstsNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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

void StatSumBursts::init( const KHistogram& whichHistogram )
{
}

void StatSumBursts::reset()
{
}

TSemanticValue StatSumBursts::execute( CalculateData *data, TTimelinesData& timelinesData )
{
  return timelinesData.dataValue;
}

TSemanticValue StatSumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    TObjectOrder row,
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
