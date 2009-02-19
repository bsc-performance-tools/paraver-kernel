#include "filter.h"

void Filter::getAllFilterFunctions( vector<string>& onVector ) const
{
  myKernel->getAllFilterFunctions( onVector );
}

void FilterProxy::setLogical( bool newValue )
{
  myFilter->setLogical( newValue );
}

void FilterProxy::setPhysical( bool newValue )
{
  myFilter->setPhysical( newValue );
}

bool FilterProxy::getLogical()
{
  return myFilter->getLogical();
}

bool FilterProxy::getPhysical()
{
  return myFilter->getPhysical();
}

void FilterProxy::clearCommFrom()
{
  myFilter->clearCommFrom();
}

void FilterProxy::insertCommFrom( TObjectOrder value )
{
  myFilter->insertCommFrom( value );
}

void FilterProxy::getCommFrom( vector<TObjectOrder>& onVector ) const
{
  myFilter->getCommFrom( onVector );
}

void FilterProxy::setCommFromFunction( string newFunction )
{
  myFilter->setCommFromFunction( newFunction );
}

string FilterProxy::getCommFromFunction() const
{
  return myFilter->getCommFromFunction();
}

void FilterProxy::clearCommTo()
{
  myFilter->clearCommTo();
}

void FilterProxy::insertCommTo( TObjectOrder value )
{
  myFilter->insertCommTo( value );
}

void FilterProxy::getCommTo( vector<TObjectOrder>& onVector ) const
{
  myFilter->getCommTo( onVector );
}

void FilterProxy::setCommToFunction( string newFunction )
{
  myFilter->setCommToFunction( newFunction );
}

string FilterProxy::getCommToFunction() const
{
  return myFilter->getCommToFunction();
}

void FilterProxy::clearCommTags()
{
  myFilter->clearCommTags();
}

void FilterProxy::insertCommTag( TCommTag value )
{
  myFilter->insertCommTag( value );
}

void FilterProxy::getCommTag( vector<TCommTag>& onVector ) const
{
  myFilter->getCommTag( onVector );
}

void FilterProxy::setCommTagFunction( string newFunction )
{
  myFilter->setCommTagFunction( newFunction );
}

string FilterProxy::getCommTagFunction() const
{
  return myFilter->getCommTagFunction();
}

void FilterProxy::clearCommSizes()
{
  myFilter->clearCommSizes();
}

void FilterProxy::insertCommSize( TCommSize value )
{
  myFilter->insertCommSize( value );
}

void FilterProxy::getCommSize( vector<TCommSize>& onVector ) const
{
  myFilter->getCommSize( onVector );
}

void FilterProxy::setCommSizeFunction( string newFunction )
{
  myFilter->setCommSizeFunction( newFunction );
}

string FilterProxy::getCommSizeFunction() const
{
  return myFilter->getCommSizeFunction();
}

void FilterProxy::clearBandWidth()
{
  myFilter->clearBandWidth();
}

void FilterProxy::insertBandWidth( TSemanticValue value )
{
  myFilter->insertBandWidth( value );
}

void FilterProxy::getBandWidth( vector<TSemanticValue>& onVector ) const
{
  myFilter->getBandWidth( onVector );
}

void FilterProxy::setBandWidthFunction( string newFunction )
{
  myFilter->setBandWidthFunction( newFunction );
}

string FilterProxy::getBandWidthFunction() const
{
  return myFilter->getBandWidthFunction();
}

void FilterProxy::clearEventTypes()
{
  myFilter->clearEventTypes();
}

void FilterProxy::insertEventType( TEventType value )
{
  myFilter->insertEventType( value );
}

void FilterProxy::getEventType( vector<TEventType>& onVector ) const
{
  myFilter->getEventType( onVector );
}

void FilterProxy::setEventTypeFunction( string newFunction )
{
  myFilter->setEventTypeFunction( newFunction );
}

string FilterProxy::getEventTypeFunction() const
{
  return myFilter->getEventTypeFunction();
}

void FilterProxy::clearEventValues()
{
  myFilter->clearEventValues();
}

void FilterProxy::insertEventValue( TEventValue value )
{
  myFilter->insertEventValue( value );
}

void FilterProxy::getEventValue( vector<TEventValue>& onVector ) const
{
  myFilter->getEventValue( onVector );
}

void FilterProxy::setEventValueFunction( string newFunction )
{
  myFilter->setEventValueFunction( newFunction );
}

string FilterProxy::getEventValueFunction() const
{
  return myFilter->getEventValueFunction();
}

void FilterProxy::setOpFromToAnd()
{
  myFilter->setOpFromToAnd();
}

void FilterProxy::setOpFromToOr()
{
  myFilter->setOpFromToOr();
}

void FilterProxy::setOpTagSizeAnd()
{
  myFilter->setOpTagSizeAnd();
}

void FilterProxy::setOpTagSizeOr()
{
  myFilter->setOpTagSizeOr();
}

void FilterProxy::setOpTypeValueAnd()
{
  myFilter->setOpTypeValueAnd();
}

void FilterProxy::setOpTypeValueOr()
{
  myFilter->setOpTypeValueOr();
}

bool FilterProxy::getOpFromTo() const
{
  return myFilter->getOpFromTo();
}

bool FilterProxy::getOpTagSize() const
{
  return myFilter->getOpTagSize();
}

bool FilterProxy::getOpTypeValue() const
{
  return myFilter->getOpTypeValue();
}

void FilterProxy::copyEventsSection( Filter *whichFilter )
{
  if ( whichFilter != NULL )
  {
    vector< TEventType > etype;
    whichFilter->getEventType( etype );
    for ( vector< TEventType >::iterator it = etype.begin(); it < etype.end(); it++ )
      myFilter->insertEventType( *it );

    myFilter->setEventTypeFunction( whichFilter->getEventTypeFunction() );

    vector< TEventValue > evalue;
    whichFilter->getEventValue( evalue );
    for ( vector< TEventValue >::iterator it = evalue.begin(); it < evalue.end(); it++ )
      myFilter->insertEventValue( *it );

    myFilter->setEventValueFunction( whichFilter->getEventValueFunction() );

    if ( whichFilter->getOpFromTo() )
      myFilter->setOpFromToAnd();
    else
      myFilter->setOpFromToOr();

    if ( whichFilter->getOpTagSize() )
      myFilter->setOpTagSizeAnd();
    else
      myFilter->setOpTagSizeOr();

    if ( whichFilter->getOpTypeValue() )
      myFilter->setOpTypeValueAnd();
    else
      myFilter->setOpTypeValueOr();
  }
}

void FilterProxy::copyCommunicationsSection( Filter *whichFilter )
{
  if ( whichFilter != NULL )
  {
    myFilter->setLogical( whichFilter->getLogical() );
    myFilter->setPhysical( whichFilter->getPhysical() );

    vector< TObjectOrder > from;
    whichFilter->getCommFrom( from );
    for ( vector< TObjectOrder >::iterator it = from.begin(); it < from.end(); it++ )
      myFilter->insertCommFrom( *it );

    myFilter->setCommFromFunction( whichFilter->getCommFromFunction() );

    vector< TObjectOrder > to;
    whichFilter->getCommTo( to );
    for ( vector< TObjectOrder >::iterator it = to.begin(); it < to.end(); it++ )
      myFilter->insertCommTo( *it );

    myFilter->setCommToFunction( whichFilter->getCommToFunction() );

    vector< TCommTag > tag;
    whichFilter->getCommTag( tag );
    for ( vector< TCommTag >::iterator it = tag.begin(); it < tag.end(); it++ )
      myFilter->insertCommTag( *it );

    myFilter->setCommTagFunction( whichFilter->getCommTagFunction() );

    vector< TCommSize > size;
    whichFilter->getCommSize( size );
    for ( vector< TCommSize >::iterator it = size.begin(); it < size.end(); it++ )
      myFilter->insertCommSize( *it );

    myFilter->setCommSizeFunction( whichFilter->getCommSizeFunction() );

    vector< TSemanticValue > bw;
    whichFilter->getBandWidth( bw );
    for ( vector< TSemanticValue >::iterator it = bw.begin(); it < bw.end(); it++ )
      myFilter->insertBandWidth( *it );

    myFilter->setBandWidthFunction( whichFilter->getBandWidthFunction() );
  }
}
