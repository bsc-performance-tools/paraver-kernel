#include "filter.h"
#include "kwindow.h"

bool Filter::passFilter( MemoryTrace::iterator *it )
{
  if ( it->getType() & EVENT )
    return filterEvents( it );
  else if ( it->getType() & COMM )
    return filterComms( it );
  return true;
}

bool Filter::filterComms( MemoryTrace::iterator *it )
{
  bool stop = true;
  bool tmpResult = true;
  TSemanticValue info;

  if ( logical && !( it->getType() & LOG ) )
    return false;
  if ( physical && !( it->getType() & PHY ) )
    return false;

  if ( existCommFrom )
  {
    if ( window->getLevel() >= SYSTEM )
    {
      TCPUOrder tmpCPU = window->getTrace()->getSenderCPU( it->getCommIndex() );
      info = ( TSemanticValue ) window->cpuObjectToWindowObject( tmpCPU );
    }
    else
    {
      TThreadOrder tmpThread = window->getTrace()->getSenderThread( it->getCommIndex() );
      info = ( TSemanticValue ) window->threadObjectToWindowObject( tmpThread );
    }
    for ( UINT32 i = 0; i < commFrom.size(); i++ )
    {
      stop = functionCommFrom->execute( ( TSemanticValue ) commFrom[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionCommFrom->getResult();
  }

  if ( opFromTo == AND && !tmpResult )
    return false;
  tmpResult = true;

  if ( existCommTo )
  {
    if ( window->getLevel() >= SYSTEM )
    {
      TCPUOrder tmpCPU = window->getTrace()->getReceiverCPU( it->getCommIndex() );
      info = ( TSemanticValue ) window->cpuObjectToWindowObject( tmpCPU );
    }
    else
    {
      TThreadOrder tmpThread = window->getTrace()->getReceiverThread( it->getCommIndex() );
      info = ( TSemanticValue ) window->threadObjectToWindowObject( tmpThread );
    }
    for ( UINT32 i = 0; i < commTo.size(); i++ )
    {
      stop = functionCommTo->execute( ( TSemanticValue ) commTo[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionCommTo->getResult();
  }

  if ( !tmpResult )
    return false;

  if ( existCommTags )
  {
    info = ( TSemanticValue ) window->getTrace()->getCommTag( it->getCommIndex() );
    for ( UINT32 i = 0; i < commTags.size(); i++ )
    {
      stop = functionCommTags->execute( ( TSemanticValue ) commTags[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionCommTags->getResult();
  }

  if ( opTagSize == AND && !tmpResult )
    return false;
  tmpResult = true;

  if ( existCommSize )
  {
    info = ( TSemanticValue ) window->getTrace()->getCommSize( it->getCommIndex() );
    for ( UINT32 i = 0; i < commSizes.size(); i++ )
    {
      stop = functionCommSizes->execute( ( TSemanticValue ) commSizes[ i ], info );
      if ( stop == false )
        break;
    }
    tmpResult = functionCommSizes->getResult();
  }

  if ( !tmpResult )
    return false;

  if ( existBandWidth )
  {
    info = ( TSemanticValue ) window->getTrace()->getCommSize( it->getCommIndex() )
           / ( TSemanticValue )
           ( window->getTrace()->getPhysicalReceive( it->getCommIndex() ) -
             window->getTrace()->getPhysicalSend( it->getCommIndex() ) );
    for ( UINT32 i = 0; i < bandWidth.size(); i++ )
    {
      stop = functionBandWidth->execute( ( TSemanticValue ) bandWidth[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionBandWidth->getResult();
  }

  return tmpResult;
}

bool Filter::filterEvents( MemoryTrace::iterator *it )
{
  bool stop = true;
  bool tmpResult = true;
  TSemanticValue info;

  if ( existEventTypes )
  {
    info = ( TSemanticValue ) it->getEventType();
    for ( UINT32 i = 0; i < eventTypes.size(); i++ )
    {
      stop = functionEventTypes->execute( ( TSemanticValue ) eventTypes[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionEventTypes->getResult();
  }

  if ( opTypeValue == AND && !tmpResult )
    return false;
  tmpResult = true;

  if ( existEventValues )
  {
    info = ( TSemanticValue ) it->getEventValue();
    for ( UINT32 i = 0; i < eventValues.size(); i++ )
    {
      stop = functionEventValues->execute( ( TSemanticValue ) eventValues[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionEventValues->getResult();
  }

  return tmpResult;
}


string FilterAll::name = "All";
bool FilterAll::execute( TSemanticValue param, TSemanticValue data )
{
  result = true;
  return true;
}

string FilterNone::name = "None";
bool FilterNone::execute( TSemanticValue param, TSemanticValue data )
{
  result = false;
  return true;
}

string FilterEqual::name = "=";
bool FilterEqual::execute( TSemanticValue param, TSemanticValue data )
{
  result = data == param;
  if ( result )
    return true;
  return false;
}

string FilterNotEqual::name = "!=";
bool FilterNotEqual::execute( TSemanticValue param, TSemanticValue data )
{
  result = data != param;
  if ( result )
    return false;
  return true;
}

string FilterGreater::name = ">";
bool FilterGreater::execute( TSemanticValue param, TSemanticValue data )
{
  result = data > param;
  if ( result )
    return false;
  return true;
}

string FilterFewer::name = "<";
bool FilterFewer::execute( TSemanticValue param, TSemanticValue data )
{
  result = data < param;
  if ( result )
    return false;
  return true;
}

string FilterRange::name = "[x,y]";
bool FilterRange::execute( TSemanticValue param, TSemanticValue data )
{
  bool tmp = true;

  if ( position == MINOR )
  {
    result = data >= param;
    if ( result )
    {
      tmp = false;
      position = MAJOR;
    }
    else
      tmp = true;
  }
  else if ( position == MAJOR )
  {
    result = data <= param;
    position = MINOR;
    tmp = true;
  }
  return tmp;
}


void Filter::clearCommFrom()
{
  commFrom.clear();
  existCommFrom = false;
}

void Filter::insertCommFrom( TObjectOrder value )
{
  commFrom.push_back( value );
  existCommFrom = true;
}

void Filter::setCommFromFunction( FilterFunction *newFunction )
{
  delete functionCommFrom;
  functionCommFrom = newFunction;
}


void Filter::clearCommTo()
{
  commTo.clear();
  existCommTo = false;
}

void Filter::insertCommTo( TObjectOrder value )
{
  commTo.push_back( value );
  existCommTo = true;
}

void Filter::setCommToFunction( FilterFunction *newFunction )
{
  delete functionCommTo;
  functionCommTo = newFunction;
}


void Filter::clearCommTags()
{
  commTags.clear();
  existCommTags = false;
}

void Filter::insertCommTag( TCommTag value )
{
  commTags.push_back( value );
  existCommTags = true;
}

void Filter::setCommTagFunction( FilterFunction *newFunction )
{
  delete functionCommTags;
  functionCommTags = newFunction;
}


void Filter::clearCommSizes()
{
  commSizes.clear();
  existCommSize = false;
}

void Filter::insertCommSize( TCommSize value )
{
  commSizes.push_back( value );
  existCommSize = true;
}

void Filter::setCommSizeFunction( FilterFunction *newFunction )
{
  delete functionCommSizes;
  functionCommSizes = newFunction;
}


void Filter::clearBandWidth()
{
  bandWidth.clear();
  existBandWidth = false;
}

void Filter::insertBandWitdh( TSemanticValue value )
{
  bandWidth.push_back( value );
  existBandWidth = true;
}

void Filter::setBandWidthFunction( FilterFunction *newFunction )
{
  delete functionBandWidth;
  functionBandWidth = newFunction;
}


void Filter::clearEventTypes()
{
  eventTypes.clear();
  existEventTypes = false;
}

void Filter::insertEventType( TEventType value )
{
  eventTypes.push_back( value );
  existEventTypes = true;
}

void Filter::setEventTypeFunction( FilterFunction *newFunction )
{
  delete functionEventTypes;
  functionEventTypes = newFunction;
}


void Filter::clearEventValues()
{
  eventValues.clear();
  existEventValues = false;
}

void Filter::insertEventValue( TEventValue value )
{
  eventValues.push_back( value );
  existEventValues = true;
}

void Filter::setEventValueFunction( FilterFunction *newFunction )
{
  delete functionEventValues;
  functionEventValues = newFunction;
}
