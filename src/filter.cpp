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
  bool tmp = true;

  if ( logical && !( it->getType() & LOG ) )
    return false;
  if ( physical && !( it->getType() & PHY ) )
    return false;

  if ( existCommTags )
  {
    for ( UINT32 i = 0; i < commTags.size(); i++ )
    {
      tmp = functionCommTags->execute( ( TSemanticValue ) commTags[ i ],
                                       ( TSemanticValue ) window->getTrace()->getCommTag( it->getCommIndex() ) );
      if ( tmp == false )
        break;
    }
  }

  if ( opTagSize == AND && tmp == false )
    return false;
  tmp = true;

  if ( existCommSize )
  {
    for ( UINT32 i = 0; i < commSizes.size(); i++ )
    {
      tmp = functionCommSizes->execute( ( TSemanticValue ) commSizes[ i ],
                                        ( TSemanticValue ) window->getTrace()->getCommSize( it->getCommIndex() ) );
      if ( tmp == false )
        break;
    }
  }

  if ( tmp == false )
    return false;

  if ( existBandWidth )
  {
    TSemanticValue tmpBW;

    for ( UINT32 i = 0; i < bandWidth.size(); i++ )
    {
      tmpBW = ( TSemanticValue ) window->getTrace()->getCommSize( it->getCommIndex() )
              / ( TSemanticValue )
              ( window->getTrace()->getPhysicalReceive( it->getCommIndex() ) -
                window->getTrace()->getPhysicalSend( it->getCommIndex() ) );
      tmp = functionBandWidth->execute( ( TSemanticValue ) bandWidth[ i ],
                                        tmpBW );

      if ( tmp == false )
        break;
    }
  }

  return tmp;
}

bool Filter::filterEvents( MemoryTrace::iterator *it )
{
  bool tmp = true;

  if ( existEventTypes )
  {
    for ( UINT32 i = 0; i < eventTypes.size(); i++ )
    {
      tmp = functionEventTypes->execute( ( TSemanticValue ) eventTypes[ i ],
                                       ( TSemanticValue ) it->getEventType() );
      if ( tmp == false )
        break;
    }
  }

  if ( opTypeValue == AND && tmp == false )
    return false;
  tmp = true;

  if ( existEventValues )
  {
    for ( UINT32 i = 0; i < eventValues.size(); i++ )
    {
      tmp = functionEventValues->execute( ( TSemanticValue ) eventValues[ i ],
                                        ( TSemanticValue ) it->getEventValue() );
      if ( tmp == false )
        break;
    }
  }

  return tmp;
}


bool FilterAll::execute( TSemanticValue param, TSemanticValue data )
{
  return true;
}

bool FilterNone::execute( TSemanticValue param, TSemanticValue data )
{
  return false;
}

bool FilterEqual::execute( TSemanticValue param, TSemanticValue data )
{
  return data == param;
}

bool FilterNotEqual::execute( TSemanticValue param, TSemanticValue data )
{
  return data != param;
}

bool FilterGreater::execute( TSemanticValue param, TSemanticValue data )
{
  return data > param;
}

bool FilterFewer::execute( TSemanticValue param, TSemanticValue data )
{
  return data < param;
}

bool FilterRange::execute( TSemanticValue param, TSemanticValue data )
{
  bool tmp = true;

  if( position == MINOR )
  {
    tmp = data >= param;
    position = MAJOR;
  }
  else if( position == MAJOR )
  {
    tmp = data <= param;
    position = MINOR;
  }
  return tmp;
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
