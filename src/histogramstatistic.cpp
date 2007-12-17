#include <math.h>
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
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
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
// Histogram Statistic: Average bytes received
//-------------------------------------------------------------------------
string StatAvgBytesReceived::name = "Average bytes received";

TObjectOrder StatAvgBytesReceived::getPartner( CalculateData *data )
{
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );
  return 0;
}

void StatAvgBytesReceived::init( Histogram *whichHistogram )
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

void StatAvgBytesReceived::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numComms.begin();

  while ( itPlane != numComms.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatAvgBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    ( ( numComms[ data->plane ] )[ getPartner( data ) ] )++;
    return controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
  }
  return 0;
}

TSemanticValue StatAvgBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

string StatAvgBytesReceived::getName()
{
  return StatAvgBytesReceived::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );
  return 0;
}

void StatMinBytesSent::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    min.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      min[ iPlane ].push_back( 0.0 );
  }
}

void StatMinBytesSent::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = min.begin();

  while ( itPlane != min.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatMinBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    if ( ( ( min[ data->plane ] )[ getPartner( data ) ] ) == 0.0 )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
    }
    else if ( controlWin->getTrace()->getCommSize( data->comm->getCommIndex() ) <
              ( ( min[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
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

string StatMinBytesSent::getName()
{
  return StatMinBytesSent::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );
  return 0;
}

void StatMinBytesReceived::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    min.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      min[ iPlane ].push_back( 0.0 );
  }
}

void StatMinBytesReceived::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = min.begin();

  while ( itPlane != min.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatMinBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    if ( ( ( min[ data->plane ] )[ getPartner( data ) ] ) == 0.0 )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
    }
    else if ( controlWin->getTrace()->getCommSize( data->comm->getCommIndex() ) <
              ( ( min[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( min[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
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

string StatMinBytesReceived::getName()
{
  return StatMinBytesReceived::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );
  return 0;
}

void StatMaxBytesSent::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    max.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      max[ iPlane ].push_back( 0.0 );
  }
}

void StatMaxBytesSent::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = max.begin();

  while ( itPlane != max.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatMaxBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    if ( controlWin->getTrace()->getCommSize( data->comm->getCommIndex() ) >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
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

string StatMaxBytesSent::getName()
{
  return StatMaxBytesSent::name;
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
  if ( controlWin->getLevel() >= SYSTEM )
    return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
  else
    return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );
  return 0;
}

void StatMaxBytesReceived::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    max.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      max[ iPlane ].push_back( 0.0 );
  }
}

void StatMaxBytesReceived::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = max.begin();

  while ( itPlane != max.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatMaxBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    if ( controlWin->getTrace()->getCommSize( data->comm->getCommIndex() ) >
         ( ( max[ data->plane ] )[ getPartner( data ) ] ) )
    {
      ( ( max[ data->plane ] )[ getPartner( data ) ] ) =
        controlWin->getTrace()->getCommSize( data->comm->getCommIndex() );
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

string StatMaxBytesReceived::getName()
{
  return StatMaxBytesReceived::name;
}

HistogramStatistic *StatMaxBytesReceived::clone()
{
  return new StatMaxBytesReceived( *this );
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
  {
    ( *it ) = 0.0;
    it++;
  }
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


//-------------------------------------------------------------------------
// Histogram Statistic: % Time Not Zero
//-------------------------------------------------------------------------
string StatPercTimeNotZero::name = "% Time Not Zero";

void StatPercTimeNotZero::init( Histogram *whichHistogram )
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

void StatPercTimeNotZero::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
  {
    ( *it ) = 0.0;
    it++;
  }
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

string StatPercTimeNotZero::getName()
{
  return StatPercTimeNotZero::name;
}

HistogramStatistic *StatPercTimeNotZero::clone()
{
  return new StatPercTimeNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Window Time
//-------------------------------------------------------------------------
string StatPercTimeWindow::name = "% Window Time";

void StatPercTimeWindow::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

void StatPercTimeWindow::reset()
{
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

string StatPercTimeWindow::getName()
{
  return StatPercTimeWindow::name;
}

HistogramStatistic *StatPercTimeWindow::clone()
{
  return new StatPercTimeWindow( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts
//-------------------------------------------------------------------------
string StatNumBursts::name = "# Bursts";

void StatNumBursts::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
}

void StatNumBursts::reset()
{
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

string StatNumBursts::getName()
{
  return StatNumBursts::name;
}

HistogramStatistic *StatNumBursts::clone()
{
  return new StatNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % # Bursts
//-------------------------------------------------------------------------
string StatPercNumBursts::name = "% # Bursts";

void StatPercNumBursts::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;

  myHistogram = whichHistogram;

  rowTotal.clear();
  if ( myHistogram->getThreeDimensions() )
    numPlanes = myHistogram->getNumPlanes();
  else
    numPlanes = 1;

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    rowTotal.push_back( 0.0 );
}

void StatPercNumBursts::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
  {
    ( *it ) = 0.0;
    it++;
  }
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

string StatPercNumBursts::getName()
{
  return StatPercNumBursts::name;
}

HistogramStatistic *StatPercNumBursts::clone()
{
  return new StatPercNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Integral
//-------------------------------------------------------------------------
string StatIntegral::name = "Integral";

void StatIntegral::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatIntegral::reset()
{
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

string StatIntegral::getName()
{
  return StatIntegral::name;
}

HistogramStatistic *StatIntegral::clone()
{
  return new StatIntegral( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value
//-------------------------------------------------------------------------
string StatAvgValue::name = "Average value";

void StatAvgValue::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      numValues[ iPlane ].push_back( 0.0 );
  }
}

void StatAvgValue::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numValues.begin();

  while ( itPlane != numValues.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatAvgValue::execute( CalculateData *data )
{
  ( ( numValues[ data->plane ] )[ data->column ] )++;
  return dataWin->getValue( data->dataRow );
}

TSemanticValue StatAvgValue::finishRow( TSemanticValue cellValue,
                                        THistogramColumn column,
                                        THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgValue::getName()
{
  return StatAvgValue::name;
}

HistogramStatistic *StatAvgValue::clone()
{
  return new StatAvgValue( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum
//-------------------------------------------------------------------------
string StatMaximum::name = "Maximum";

void StatMaximum::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    max.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      max[ iPlane ].push_back( 0.0 );
  }
}

void StatMaximum::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = max.begin();

  while ( itPlane != max.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
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

string StatMaximum::getName()
{
  return StatMaximum::name;
}

HistogramStatistic *StatMaximum::clone()
{
  return new StatMaximum( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average Burst Time
//-------------------------------------------------------------------------
string StatAvgBurstTime::name = "Average Burst Time";

void StatAvgBurstTime::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      numValues[ iPlane ].push_back( 0.0 );
  }
}

void StatAvgBurstTime::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numValues.begin();

  while ( itPlane != numValues.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatAvgBurstTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  ( ( numValues[ data->plane ] )[ data->column ] )++;

  return end - begin;
}

TSemanticValue StatAvgBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgBurstTime::getName()
{
  return StatAvgBurstTime::name;
}

HistogramStatistic *StatAvgBurstTime::clone()
{
  return new StatAvgBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Stdev Burst Time
//-------------------------------------------------------------------------
string StatStdevBurstTime::name = "Stdev Burst Time";

void StatStdevBurstTime::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>() );
    qValues.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
    {
      numValues[ iPlane ].push_back( 0.0 );
      qValues[ iPlane ].push_back( 0.0 );
    }
  }
}

void StatStdevBurstTime::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlaneN = numValues.begin();
  vector<vector<TSemanticValue> >::iterator itPlaneQ = qValues.begin();

  while ( itPlaneN != numValues.end() )
  {
    vector<TSemanticValue>::iterator itColumnN = ( *itPlaneN ).begin();
    vector<TSemanticValue>::iterator itColumnQ = ( *itPlaneQ ).begin();
    while ( itColumnN != ( *itPlaneN ).end() )
    {
      ( *itColumnN ) = 0.0;
      itColumnN++;
      ( *itColumnQ ) = 0.0;
      itColumnQ++;
    }
    itPlaneN++;
    itPlaneQ++;
  }
}

TSemanticValue StatStdevBurstTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  ( ( numValues[ data->plane ] )[ data->column ] )++;
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

string StatStdevBurstTime::getName()
{
  return StatStdevBurstTime::name;
}

HistogramStatistic *StatStdevBurstTime::clone()
{
  return new StatStdevBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average per Burst
//-------------------------------------------------------------------------
string StatAvgPerBurst::name = "Average per Burst";

void StatAvgPerBurst::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      numValues[ iPlane ].push_back( 0.0 );
  }
}

void StatAvgPerBurst::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numValues.begin();

  while ( itPlane != numValues.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatAvgPerBurst::execute( CalculateData *data )
{
  ( ( numValues[ data->plane ] )[ data->column ] )++;

  return dataWin->getValue( data->dataRow );
}

TSemanticValue StatAvgPerBurst::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgPerBurst::getName()
{
  return StatAvgPerBurst::name;
}

HistogramStatistic *StatAvgPerBurst::clone()
{
  return new StatAvgPerBurst( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value != 0
//-------------------------------------------------------------------------
string StatAvgValueNotZero::name = "Average value != 0";

void StatAvgValueNotZero::init( Histogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>() );
    for ( TObjectOrder iColumn = 0; iColumn < numColumns; iColumn++ )
      numValues[ iPlane ].push_back( 0.0 );
  }
}

void StatAvgValueNotZero::reset()
{
  vector<vector<TSemanticValue> >::iterator itPlane = numValues.begin();

  while ( itPlane != numValues.end() )
  {
    vector<TSemanticValue>::iterator itColumn = ( *itPlane ).begin();
    while ( itColumn != ( *itPlane ).end() )
    {
      ( *itColumn ) = 0.0;
      itColumn++;
    }
    itPlane++;
  }
}

TSemanticValue StatAvgValueNotZero::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) != 0.0 )
    ( ( numValues[ data->plane ] )[ data->column ] )++;

  return dataWin->getValue( data->dataRow );
}

TSemanticValue StatAvgValueNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

string StatAvgValueNotZero::getName()
{
  return StatAvgValueNotZero::name;
}

HistogramStatistic *StatAvgValueNotZero::clone()
{
  return new StatAvgValueNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts != 0
//-------------------------------------------------------------------------
string StatNumBurstsNotZero::name = "# Bursts != 0";

void StatNumBurstsNotZero::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatNumBurstsNotZero::reset()
{
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

string StatNumBurstsNotZero::getName()
{
  return StatNumBurstsNotZero::name;
}

HistogramStatistic *StatNumBurstsNotZero::clone()
{
  return new StatNumBurstsNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Sum bursts
//-------------------------------------------------------------------------
string StatSumBursts::name = "Sum bursts";

void StatSumBursts::init( Histogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

void StatSumBursts::reset()
{
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

string StatSumBursts::getName()
{
  return StatSumBursts::name;
}

HistogramStatistic *StatSumBursts::clone()
{
  return new StatSumBursts( *this );
}

