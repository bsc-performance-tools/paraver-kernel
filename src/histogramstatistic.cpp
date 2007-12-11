#include "histogramstatistic.h"
#include "histogram.h"

//-------------------------------------------------------------------------
// Histogram Statistic: #Sends
//-------------------------------------------------------------------------
string StatNumSends::name = "#Sends";

TObjectOrder StatNumSends::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );
  return 0;
}

void StatNumSends::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatNumSends::reset()
{}

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

string StatNumSends::getName()
{
  return StatNumSends::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );
  return 0;
}

void StatNumReceives::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatNumReceives::reset()
{}

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

string StatNumReceives::getName()
{
  return StatNumReceives::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );
  return 0;
}

void StatBytesSent::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatBytesSent::reset()
{}

TSemanticValue StatBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
    return controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
  return 0;
}

TSemanticValue StatBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatBytesSent::getName()
{
  return StatBytesSent::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );
  return 0;
}

void StatBytesReceived::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatBytesReceived::reset()
{}

TSemanticValue StatBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
    return controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
  return 0;
}

TSemanticValue StatBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

string StatBytesReceived::getName()
{
  return StatBytesReceived::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );
  return 0;
}

void StatAvgBytesSent::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numComms.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      numComms[ iPlane ].push_back( 0.0 );
  }
}

void StatAvgBytesSent::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numComms.begin();

  while ( itPlane != numComms.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
      ( *itColumn ) = 0.0;
  }
}

TSemanticValue StatAvgBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    ( ( numComms[ data->plane ] )[ getPartner( data ) ] )++;
    return controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
  }
  return 0;
}

TSemanticValue StatAvgBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

string StatAvgBytesSent::getName()
{
  return StatAvgBytesSent::name;
}

HistogramStatistic *StatAvgBytesSent::clone()
{
  return new StatAvgBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Time
//-------------------------------------------------------------------------
string StatTime::name = "Time";

void StatTime::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatTime::reset()
{}

TSemanticValue StatTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
          data->beginTime : controlWin->getBeginTime( data->controlRow );

  end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
        data->endTime : controlWin->getEndTime( data->controlRow );

  return end - begin;
}

TSemanticValue StatTime::finishRow( TSemanticValue cellValue,
                                    THistogramColumn column,
                                    THistogramColumn plane )
{
  return controlWin->traceUnitsToWindowUnits( cellValue );
}

string StatTime::getName()
{
  return StatTime::name;
}

HistogramStatistic *StatTime::clone()
{
  return new StatTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Time
//-------------------------------------------------------------------------
string StatPercTime::name = "% Time";

void StatPercTime::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  rowTotal.clear();
  if ( myHistogram->getThreeDimensions() )
    numPlanes = myHistogram->getNumPlanes();
  else
    numPlanes = 1;

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    rowTotal.push_back( 0.0 );
}

void StatPercTime::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
    ( *it ) = 0.0;
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

string StatPercTime::getName()
{
  return StatPercTime::name;
}

HistogramStatistic *StatPercTime::clone()
{
  return new StatPercTime( *this );
}

