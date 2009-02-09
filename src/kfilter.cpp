#include "kfilter.h"
#include "kwindow.h"
#include "functionmanagement.h"

bool KFilter::passFilter( MemoryTrace::iterator *it )
{
  if ( it->getType() & EVENT )
    return filterEvents( it );
  else if ( it->getType() & COMM )
    return filterComms( it );
  return true;
}

bool KFilter::filterComms( MemoryTrace::iterator *it )
{
  bool stop = true;
  bool tmpResult = functionCommFrom->getDefaultValue();
  TSemanticValue info;

  if ( !( logical && physical ) )
  {
    if ( !logical && !physical )
      return false;
    if ( it->getType() & LOG )
    {
      if ( !logical )
        return false;
    }
    else if ( it->getType() & PHY )
    {
      if ( !physical )
      {
        if ( !( logical && ( ( it->getType() & RECV ) &&
            ( window->getTrace()->getLogicalReceive( it->getCommIndex() ) <
              window->getTrace()->getPhysicalReceive( it->getCommIndex() ) ) ) )
           )
          return false;
      }
    }
  }

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
  tmpResult = functionCommTo->getDefaultValue();

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

  tmpResult = functionCommTags->getDefaultValue();
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
  tmpResult = functionCommSizes->getDefaultValue();

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

  tmpResult = functionBandWidth->getDefaultValue();
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

bool KFilter::filterEvents( MemoryTrace::iterator *it )
{
  bool stop = true;
  bool tmpResult = functionEventTypes->getDefaultValue();
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
  tmpResult = functionEventValues->getDefaultValue();

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


void KFilter::clearCommFrom()
{
  commFrom.clear();
  existCommFrom = false;
}

void KFilter::insertCommFrom( TObjectOrder value )
{
  commFrom.push_back( value );
  existCommFrom = true;
}

void KFilter::getCommFrom( vector<TObjectOrder>& onVector ) const
{
  onVector = commFrom;
}

void KFilter::setCommFromFunction( string newFunction )
{
  delete functionCommFrom;
  functionCommFrom = FunctionManagement<FilterFunction>::getInstance()
                     ->getFunction( newFunction );
}

string KFilter::getCommFromFunction() const
{
  return functionCommFrom->getName();
}

void KFilter::clearCommTo()
{
  commTo.clear();
  existCommTo = false;
}

void KFilter::insertCommTo( TObjectOrder value )
{
  commTo.push_back( value );
  existCommTo = true;
}

void KFilter::getCommTo( vector<TObjectOrder>& onVector ) const
{
  onVector = commTo;
}

void KFilter::setCommToFunction( string newFunction )
{
  delete functionCommTo;
  functionCommTo = FunctionManagement<FilterFunction>::getInstance()
                   ->getFunction( newFunction );
}

string KFilter::getCommToFunction() const
{
  return functionCommTo->getName();
}

void KFilter::clearCommTags()
{
  commTags.clear();
  existCommTags = false;
}

void KFilter::insertCommTag( TCommTag value )
{
  commTags.push_back( value );
  existCommTags = true;
}

void KFilter::getCommTag( vector<TCommTag>& onVector ) const
{
  onVector = commTags;
}

void KFilter::setCommTagFunction( string newFunction )
{
  delete functionCommTags;
  functionCommTags = FunctionManagement<FilterFunction>::getInstance()
                     ->getFunction( newFunction );
}

string KFilter::getCommTagFunction() const
{
  return functionCommTags->getName();
}

void KFilter::clearCommSizes()
{
  commSizes.clear();
  existCommSize = false;
}

void KFilter::insertCommSize( TCommSize value )
{
  commSizes.push_back( value );
  existCommSize = true;
}

void KFilter::getCommSize( vector<TCommSize>& onVector ) const
{
  onVector = commSizes;
}

void KFilter::setCommSizeFunction( string newFunction )
{
  delete functionCommSizes;
  functionCommSizes = FunctionManagement<FilterFunction>::getInstance()
                      ->getFunction( newFunction );
}

string KFilter::getCommSizeFunction() const
{
  return functionCommSizes->getName();
}

void KFilter::clearBandWidth()
{
  bandWidth.clear();
  existBandWidth = false;
}

void KFilter::insertBandWidth( TSemanticValue value )
{
  bandWidth.push_back( value );
  existBandWidth = true;
}

void KFilter::getBandWidth( vector<TSemanticValue>& onVector ) const
{
  onVector = bandWidth;
}

void KFilter::setBandWidthFunction( string newFunction )
{
  delete functionBandWidth;
  functionBandWidth = FunctionManagement<FilterFunction>::getInstance()
                      ->getFunction( newFunction );
}

string KFilter::getBandWidthFunction() const
{
  return functionBandWidth->getName();
}

void KFilter::clearEventTypes()
{
  eventTypes.clear();
  existEventTypes = false;
}

void KFilter::insertEventType( TEventType value )
{
  eventTypes.push_back( value );
  existEventTypes = true;
}

void KFilter::getEventType( vector<TEventType>& onVector ) const
{
  onVector = eventTypes;
}

void KFilter::setEventTypeFunction( string newFunction )
{
  delete functionEventTypes;
  functionEventTypes = FunctionManagement<FilterFunction>::getInstance()
                       ->getFunction( newFunction );
}

string KFilter::getEventTypeFunction() const
{
  return functionEventTypes->getName();
}

void KFilter::clearEventValues()
{
  eventValues.clear();
  existEventValues = false;
}

void KFilter::insertEventValue( TEventValue value )
{
  eventValues.push_back( value );
  existEventValues = true;
}

void KFilter::getEventValue( vector<TEventValue>& onVector ) const
{
  onVector = eventValues;
}

void KFilter::setEventValueFunction( string newFunction )
{
  delete functionEventValues;
  functionEventValues = FunctionManagement<FilterFunction>::getInstance()
                        ->getFunction( newFunction );
}

string KFilter::getEventValueFunction() const
{
  return functionEventValues->getName();
}
