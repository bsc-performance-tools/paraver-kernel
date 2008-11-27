#include <math.h>
#include "histogramstatistic.h"
#include "khistogram.h"
#include "kwindow.h"
#include "ktrace.h"
#include "paraverlabels.h"

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

//-------------------------------------------------------------------------
// Histogram Statistic: #Sends
//-------------------------------------------------------------------------
string StatNumSends::name = "#Sends";

inline TObjectOrder StatNumSends::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatNumSends::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatNumSends::reset()
{}

inline bool StatNumSends::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatNumSends::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
    return 1;
  return 0;
}

inline TSemanticValue StatNumSends::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatNumSends::getName() const
{
  return StatNumSends::name;
}

inline string StatNumSends::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatNumSends::clone()
{
  return new StatNumSends( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: #Receives
//-------------------------------------------------------------------------
string StatNumReceives::name = "#Receives";

inline TObjectOrder StatNumReceives::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatNumReceives::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatNumReceives::reset()
{}

inline bool StatNumReceives::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatNumReceives::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
    return 1;
  return 0;
}

inline TSemanticValue StatNumReceives::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatNumReceives::getName() const
{
  return StatNumReceives::name;
}

inline string StatNumReceives::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatNumReceives::clone()
{
  return new StatNumReceives( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Bytes Sent
//-------------------------------------------------------------------------
string StatBytesSent::name = "Bytes sent";

inline TObjectOrder StatBytesSent::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatBytesSent::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatBytesSent::reset()
{}

inline bool StatBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
    return data->comm->getCommSize();
  return 0;
}

inline TSemanticValue StatBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatBytesSent::getName() const
{
  return StatBytesSent::name;
}

inline string StatBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatBytesSent::clone()
{
  return new StatBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Bytes Received
//-------------------------------------------------------------------------
string StatBytesReceived::name = "Bytes received";

inline TObjectOrder StatBytesReceived::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatBytesReceived::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatBytesReceived::reset()
{}

inline bool StatBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
    return data->comm->getCommSize();
  return 0;
}

inline TSemanticValue StatBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatBytesReceived::getName() const
{
  return StatBytesReceived::name;
}

inline string StatBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatBytesReceived::clone()
{
  return new StatBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average bytes sent
//-------------------------------------------------------------------------
string StatAvgBytesSent::name = "Average bytes sent";

inline TObjectOrder StatAvgBytesSent::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatAvgBytesSent::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  numComms.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numComms.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgBytesSent::reset()
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

inline bool StatAvgBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatAvgBytesSent::execute( CalculateData *data )
{
  if ( data->comm->getType() & SEND )
  {
    ( ( numComms[ data->plane ] )[ getPartner( data ) ] )++;
    return data->comm->getCommSize();
  }
  return 0;
}

inline TSemanticValue StatAvgBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

inline string StatAvgBytesSent::getName() const
{
  return StatAvgBytesSent::name;
}

inline string StatAvgBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatAvgBytesSent::clone()
{
  return new StatAvgBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average bytes received
//-------------------------------------------------------------------------
string StatAvgBytesReceived::name = "Average bytes received";

inline TObjectOrder StatAvgBytesReceived::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatAvgBytesReceived::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  numComms.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numComms.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgBytesReceived::reset()
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

inline bool StatAvgBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatAvgBytesReceived::execute( CalculateData *data )
{
  if ( data->comm->getType() & RECV )
  {
    ( ( numComms[ data->plane ] )[ getPartner( data ) ] )++;
    return data->comm->getCommSize();
  }
  return 0;
}

inline TSemanticValue StatAvgBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numComms[ plane ] )[ column ];
}

inline string StatAvgBytesReceived::getName() const
{
  return StatAvgBytesReceived::name;
}

inline string StatAvgBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatAvgBytesReceived::clone()
{
  return new StatAvgBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Minimum bytes sent
//-------------------------------------------------------------------------
string StatMinBytesSent::name = "Minimum bytes sent";

inline TObjectOrder StatMinBytesSent::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatMinBytesSent::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  min.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    min.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatMinBytesSent::reset()
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

inline bool StatMinBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatMinBytesSent::execute( CalculateData *data )
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

inline TSemanticValue StatMinBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( min[ plane ] )[ column ] );
}

inline string StatMinBytesSent::getName() const
{
  return StatMinBytesSent::name;
}

inline string StatMinBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatMinBytesSent::clone()
{
  return new StatMinBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Minimum bytes received
//-------------------------------------------------------------------------
string StatMinBytesReceived::name = "Minimum bytes received";

inline TObjectOrder StatMinBytesReceived::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatMinBytesReceived::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  min.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    min.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatMinBytesReceived::reset()
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

inline bool StatMinBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatMinBytesReceived::execute( CalculateData *data )
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

inline TSemanticValue StatMinBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( min[ plane ] )[ column ] );
}

inline string StatMinBytesReceived::getName() const
{
  return StatMinBytesReceived::name;
}

inline string StatMinBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatMinBytesReceived::clone()
{
  return new StatMinBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum bytes sent
//-------------------------------------------------------------------------
string StatMaxBytesSent::name = "Maximum bytes sent";

inline TObjectOrder StatMaxBytesSent::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getReceiverCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getReceiverThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatMaxBytesSent::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  max.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    max.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatMaxBytesSent::reset()
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

inline bool StatMaxBytesSent::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatMaxBytesSent::execute( CalculateData *data )
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

inline TSemanticValue StatMaxBytesSent::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( max[ plane ] )[ column ] );
}

inline string StatMaxBytesSent::getName() const
{
  return StatMaxBytesSent::name;
}

inline string StatMaxBytesSent::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatMaxBytesSent::clone()
{
  return new StatMaxBytesSent( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum bytes received
//-------------------------------------------------------------------------
string StatMaxBytesReceived::name = "Maximum bytes received";

inline TObjectOrder StatMaxBytesReceived::getPartner( CalculateData *data )
{
  /*  if ( controlWin->getLevel() >= SYSTEM )
      return controlWin->getTrace()->getSenderCPU( data->comm->getCommIndex() );
    else
      return controlWin->getTrace()->getSenderThread( data->comm->getCommIndex() );*/
  return data->comm->getCommPartnerObject();
}

inline void StatMaxBytesReceived::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumRows();

  max.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    max.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatMaxBytesReceived::reset()
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

inline bool StatMaxBytesReceived::filter( CalculateData *data ) const
{
  return filterCommunication( data->comm, myHistogram );
}

inline TSemanticValue StatMaxBytesReceived::execute( CalculateData *data )
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

inline TSemanticValue StatMaxBytesReceived::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( ( max[ plane ] )[ column ] );
}

inline string StatMaxBytesReceived::getName() const
{
  return StatMaxBytesReceived::name;
}

inline string StatMaxBytesReceived::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatMaxBytesReceived::clone()
{
  return new StatMaxBytesReceived( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Time
//-------------------------------------------------------------------------
string StatTime::name = "Time";

inline void StatTime::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatTime::reset()
{}

inline bool StatTime::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

inline TSemanticValue StatTime::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
          data->beginTime : controlWin->getBeginTime( data->controlRow );

  end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
        data->endTime : controlWin->getEndTime( data->controlRow );

  return end - begin;
}

inline TSemanticValue StatTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return controlWin->traceUnitsToWindowUnits( cellValue );
}

inline string StatTime::getName() const
{
  return StatTime::name;
}

inline string StatTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

inline HistogramStatistic *StatTime::clone()
{
  return new StatTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Time
//-------------------------------------------------------------------------
string StatPercTime::name = "% Time";

inline void StatPercTime::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  rowTotal.clear();
  if ( myHistogram->getThreeDimensions() )
    numPlanes = myHistogram->getNumPlanes();
  else
    numPlanes = 1;

  rowTotal.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    rowTotal.push_back( 0.0 );
}

inline void StatPercTime::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
  {
    ( *it ) = 0.0;
    it++;
  }
}

inline bool StatPercTime::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

inline TSemanticValue StatPercTime::execute( CalculateData *data )
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

inline TSemanticValue StatPercTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

inline string StatPercTime::getName() const
{
  return StatPercTime::name;
}

inline string StatPercTime::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

inline HistogramStatistic *StatPercTime::clone()
{
  return new StatPercTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Time Not Zero
//-------------------------------------------------------------------------
string StatPercTimeNotZero::name = "% Time Not Zero";

inline void StatPercTimeNotZero::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;

  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();

  rowTotal.clear();
  if ( myHistogram->getThreeDimensions() )
    numPlanes = myHistogram->getNumPlanes();
  else
    numPlanes = 1;

  rowTotal.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    rowTotal.push_back( 0.0 );
}

inline void StatPercTimeNotZero::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
  {
    ( *it ) = 0.0;
    it++;
  }
}

inline bool StatPercTimeNotZero::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

inline TSemanticValue StatPercTimeNotZero::execute( CalculateData *data )
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

inline TSemanticValue StatPercTimeNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

inline string StatPercTimeNotZero::getName() const
{
  return StatPercTimeNotZero::name;
}

inline string StatPercTimeNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

inline HistogramStatistic *StatPercTimeNotZero::clone()
{
  return new StatPercTimeNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % Window Time
//-------------------------------------------------------------------------
string StatPercTimeWindow::name = "% Window Time";

inline void StatPercTimeWindow::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  controlWin = myHistogram->getControlWindow();
}

inline void StatPercTimeWindow::reset()
{
}

inline bool StatPercTimeWindow::filter( CalculateData *data ) const
{
  return filterSemanticValue( controlWin->getValue( data->controlRow ),
                              myHistogram );
}

inline TSemanticValue StatPercTimeWindow::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > controlWin->getBeginTime( data->controlRow ) ?
          data->beginTime : controlWin->getBeginTime( data->controlRow );

  end = data->endTime < controlWin->getEndTime( data->controlRow ) ?
        data->endTime : controlWin->getEndTime( data->controlRow );

  return end - begin;
}

inline TSemanticValue StatPercTimeWindow::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) /
         ( myHistogram->getEndTime() - myHistogram->getBeginTime() );
}

inline string StatPercTimeWindow::getName() const
{
  return StatPercTimeWindow::name;
}

inline string StatPercTimeWindow::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

inline HistogramStatistic *StatPercTimeWindow::clone()
{
  return new StatPercTimeWindow( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts
//-------------------------------------------------------------------------
string StatNumBursts::name = "# Bursts";

inline void StatNumBursts::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
}

inline void StatNumBursts::reset()
{
}

inline bool StatNumBursts::filter( CalculateData *data ) const
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

inline TSemanticValue StatNumBursts::execute( CalculateData *data )
{
  return 1.0;
}

inline TSemanticValue StatNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatNumBursts::getName() const
{
  return StatNumBursts::name;
}

inline string StatNumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatNumBursts::clone()
{
  return new StatNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: % # Bursts
//-------------------------------------------------------------------------
string StatPercNumBursts::name = "% # Bursts";

inline void StatPercNumBursts::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;

  myHistogram = whichHistogram;

  rowTotal.clear();
  if ( myHistogram->getThreeDimensions() )
    numPlanes = myHistogram->getNumPlanes();
  else
    numPlanes = 1;

  rowTotal.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    rowTotal.push_back( 0.0 );
}

inline void StatPercNumBursts::reset()
{
  vector<TSemanticValue>::iterator it = rowTotal.begin();

  while ( it != rowTotal.end() )
  {
    ( *it ) = 0.0;
    it++;
  }
}

inline bool StatPercNumBursts::filter( CalculateData *data ) const
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

inline TSemanticValue StatPercNumBursts::execute( CalculateData *data )
{
  if ( myHistogram->getThreeDimensions() )
    rowTotal[ data->plane ] += 1.0;
  else
    rowTotal[ 0 ] += 1.0;

  return 1.0;
}

inline TSemanticValue StatPercNumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( cellValue * 100.0 ) / rowTotal[ plane ];
}

inline string StatPercNumBursts::getName() const
{
  return StatPercNumBursts::name;
}

inline string StatPercNumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "%";
}

inline HistogramStatistic *StatPercNumBursts::clone()
{
  return new StatPercNumBursts( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Integral
//-------------------------------------------------------------------------
string StatIntegral::name = "Integral";

inline void StatIntegral::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

inline void StatIntegral::reset()
{
}

inline bool StatIntegral::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

inline TSemanticValue StatIntegral::execute( CalculateData *data )
{
  TRecordTime begin;
  TRecordTime end;

  begin = data->beginTime > dataWin->getBeginTime( data->dataRow ) ?
          data->beginTime : dataWin->getBeginTime( data->dataRow );

  end = data->endTime < dataWin->getEndTime( data->dataRow ) ?
        data->endTime : dataWin->getEndTime( data->dataRow );

  return ( end - begin ) * dataWin->getValue( data->dataRow );
}

inline TSemanticValue StatIntegral::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatIntegral::getName() const
{
  return StatIntegral::name;
}

inline string StatIntegral::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatIntegral::clone()
{
  return new StatIntegral( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value
//-------------------------------------------------------------------------
string StatAvgValue::name = "Average value";

inline void StatAvgValue::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  numValues.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgValue::reset()
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

inline bool StatAvgValue::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

inline TSemanticValue StatAvgValue::execute( CalculateData *data )
{
  ( ( numValues[ data->plane ] )[ data->column ] )++;
  return dataWin->getValue( data->dataRow );
}

inline TSemanticValue StatAvgValue::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

inline string StatAvgValue::getName() const
{
  return StatAvgValue::name;
}

inline string StatAvgValue::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatAvgValue::clone()
{
  return new StatAvgValue( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Maximum
//-------------------------------------------------------------------------
string StatMaximum::name = "Maximum";

inline void StatMaximum::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  max.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    max.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatMaximum::reset()
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

inline bool StatMaximum::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

inline TSemanticValue StatMaximum::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) >
       ( ( max[ data->plane ] )[ data->column ] ) )
    ( ( max[ data->plane ] )[ data->column ] ) = dataWin->getValue( data->dataRow );
  return 1;
}

inline TSemanticValue StatMaximum::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return ( max[ plane ] )[ column ];
}

inline string StatMaximum::getName() const
{
  return StatMaximum::name;
}

inline string StatMaximum::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatMaximum::clone()
{
  return new StatMaximum( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average Burst Time
//-------------------------------------------------------------------------
string StatAvgBurstTime::name = "Average Burst Time";

inline void StatAvgBurstTime::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  numValues.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgBurstTime::reset()
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

inline bool StatAvgBurstTime::filter( CalculateData *data ) const
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

inline TSemanticValue StatAvgBurstTime::execute( CalculateData *data )
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

inline TSemanticValue StatAvgBurstTime::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

inline string StatAvgBurstTime::getName() const
{
  return StatAvgBurstTime::name;
}

inline string StatAvgBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

inline HistogramStatistic *StatAvgBurstTime::clone()
{
  return new StatAvgBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Stdev Burst Time
//-------------------------------------------------------------------------
string StatStdevBurstTime::name = "Stdev Burst Time";

inline void StatStdevBurstTime::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  numValues.reserve( numPlanes );
  qValues.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
  {
    numValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
    qValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
  }
}

inline void StatStdevBurstTime::reset()
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

inline bool StatStdevBurstTime::filter( CalculateData *data ) const
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

inline TSemanticValue StatStdevBurstTime::execute( CalculateData *data )
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

inline TSemanticValue StatStdevBurstTime::finishRow( TSemanticValue cellValue,
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

inline string StatStdevBurstTime::getName() const
{
  return StatStdevBurstTime::name;
}

inline string StatStdevBurstTime::getUnits( const KHistogram *whichHisto ) const
{
  TTimeUnit tu =  whichHisto->getControlWindow()->getTimeUnit();

  return LABEL_TIMEUNIT[ tu ];
}

inline HistogramStatistic *StatStdevBurstTime::clone()
{
  return new StatStdevBurstTime( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average per Burst
//-------------------------------------------------------------------------
string StatAvgPerBurst::name = "Average per Burst";

inline void StatAvgPerBurst::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  numValues.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgPerBurst::reset()
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

inline bool StatAvgPerBurst::filter( CalculateData *data ) const
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

inline TSemanticValue StatAvgPerBurst::execute( CalculateData *data )
{
  ( ( numValues[ data->plane ] )[ data->column ] )++;

  return dataWin->getValue( data->dataRow );
}

inline TSemanticValue StatAvgPerBurst::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

inline string StatAvgPerBurst::getName() const
{
  return StatAvgPerBurst::name;
}

inline string StatAvgPerBurst::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatAvgPerBurst::clone()
{
  return new StatAvgPerBurst( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Average value != 0
//-------------------------------------------------------------------------
string StatAvgValueNotZero::name = "Average value != 0";

inline void StatAvgValueNotZero::init( KHistogram *whichHistogram )
{
  THistogramColumn numPlanes;
  TObjectOrder numColumns;

  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();

  numPlanes = myHistogram->getNumPlanes();
  numColumns = myHistogram->getNumColumns();

  numValues.reserve( numPlanes );
  for ( THistogramColumn iPlane = 0; iPlane < numPlanes; iPlane++ )
    numValues.push_back( vector<TSemanticValue>( numColumns, 0.0 ) );
}

inline void StatAvgValueNotZero::reset()
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

inline bool StatAvgValueNotZero::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

inline TSemanticValue StatAvgValueNotZero::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) != 0.0 )
    ( ( numValues[ data->plane ] )[ data->column ] )++;

  return dataWin->getValue( data->dataRow );
}

inline TSemanticValue StatAvgValueNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue / ( numValues[ plane ] )[ column ];
}

inline string StatAvgValueNotZero::getName() const
{
  return StatAvgValueNotZero::name;
}

inline string StatAvgValueNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatAvgValueNotZero::clone()
{
  return new StatAvgValueNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: # Bursts != 0
//-------------------------------------------------------------------------
string StatNumBurstsNotZero::name = "# Bursts != 0";

inline void StatNumBurstsNotZero::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

inline void StatNumBurstsNotZero::reset()
{
}

inline bool StatNumBurstsNotZero::filter( CalculateData *data ) const
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

inline TSemanticValue StatNumBurstsNotZero::execute( CalculateData *data )
{
  if ( dataWin->getValue( data->dataRow ) != 0.0 )
    return 1.0;
  return 0.0;
}

inline TSemanticValue StatNumBurstsNotZero::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatNumBurstsNotZero::getName() const
{
  return StatNumBurstsNotZero::name;
}

inline string StatNumBurstsNotZero::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatNumBurstsNotZero::clone()
{
  return new StatNumBurstsNotZero( *this );
}


//-------------------------------------------------------------------------
// Histogram Statistic: Sum bursts
//-------------------------------------------------------------------------
string StatSumBursts::name = "Sum bursts";

inline void StatSumBursts::init( KHistogram *whichHistogram )
{
  myHistogram = whichHistogram;
  dataWin = myHistogram->getDataWindow();
}

inline void StatSumBursts::reset()
{
}

inline bool StatSumBursts::filter( CalculateData *data ) const
{
  return filterSemanticValue( dataWin->getValue( data->dataRow ),
                              myHistogram );
}

inline TSemanticValue StatSumBursts::execute( CalculateData *data )
{
  return dataWin->getValue( data->dataRow );
}

inline TSemanticValue StatSumBursts::finishRow( TSemanticValue cellValue,
    THistogramColumn column,
    THistogramColumn plane )
{
  return cellValue;
}

inline string StatSumBursts::getName() const
{
  return StatSumBursts::name;
}

inline string StatSumBursts::getUnits( const KHistogram *whichHisto ) const
{
  return "";
}

inline HistogramStatistic *StatSumBursts::clone()
{
  return new StatSumBursts( *this );
}

