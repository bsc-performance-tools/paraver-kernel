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
