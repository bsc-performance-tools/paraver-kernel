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

