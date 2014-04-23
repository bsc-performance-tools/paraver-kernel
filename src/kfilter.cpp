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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "kfilter.h"
#include "kwindow.h"
#include "functionmanagement.h"

using namespace std;

bool KFilter::passFilter( MemoryTrace::iterator *it )
{
  if ( it->getType() & EVENT )
    return filterEvents( it );
  else if ( it->getType() & COMM ||
            it->getType() & RSEND ||
            it->getType() & RRECV )
    return filterComms( it );
  return true;
}

bool KFilter::filterComms( MemoryTrace::iterator *it )
{
  bool stop = true;
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
        if ( !( logical && ( ( it->getType() & RECV || it->getType() & RRECV ) &&
                             ( window->getTrace()->getLogicalReceive( it->getCommIndex() ) <=
                               window->getTrace()->getPhysicalReceive( it->getCommIndex() ) ) ) )
           )
          return false;
      }
    }
  }

  bool tmpResult = functionCommFrom->getDefaultValue();
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
    for ( PRV_UINT32 i = 0; i < commFrom.size(); i++ )
    {
      stop = functionCommFrom->execute( ( TSemanticValue ) commFrom[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionCommFrom->getResult();
  }

  if ( opFromTo == AND && !tmpResult )
    return false;
  else if( opFromTo == OR && tmpResult )
    return true;

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
    for ( PRV_UINT32 i = 0; i < commTo.size(); i++ )
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
    for ( PRV_UINT32 i = 0; i < commTags.size(); i++ )
    {
      stop = functionCommTags->execute( ( TSemanticValue ) commTags[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionCommTags->getResult();
  }

  if ( opTagSize == AND && !tmpResult )
    return false;
  else if( opTagSize == OR && tmpResult )
    return true;

  tmpResult = functionCommSizes->getDefaultValue();
  if ( existCommSize )
  {
    info = ( TSemanticValue ) window->getTrace()->getCommSize( it->getCommIndex() );
    for ( PRV_UINT32 i = 0; i < commSizes.size(); i++ )
    {
      stop = functionCommSizes->execute( ( TSemanticValue ) commSizes[ i ], info );
      if ( stop ) // == false?
        break;
    }
    tmpResult = functionCommSizes->getResult();
  }

  if ( !tmpResult )
    return false;

  tmpResult = functionBandWidth->getDefaultValue();
  if ( existBandWidth )
  {
    TRecordTime time;
    TCommID id = it->getCommIndex();

    if ( window->getFilter()->getLogical() &&
         window->getTrace()->getLogicalReceive( id ) > window->getTrace()->getPhysicalReceive( id ) )
      time = window->getTrace()->getLogicalReceive( id ) - window->getTrace()->getLogicalSend( id );
    else
    {
      if ( window->getFilter()->getPhysical() )
        time = window->getTrace()->getPhysicalReceive( id ) - window->getTrace()->getPhysicalSend( id );
      else
        time = window->getTrace()->getPhysicalReceive( id ) - window->getTrace()->getLogicalSend( id );
    }

    info = ( TSemanticValue ) window->getTrace()->getCommSize( id )
           / ( TSemanticValue ) window->traceUnitsToWindowUnits( time );

    for ( PRV_UINT32 i = 0; i < bandWidth.size(); i++ )
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
    for ( PRV_UINT32 i = 0; i < eventTypes.size(); i++ )
    {
      stop = functionEventTypes->execute( ( TSemanticValue ) eventTypes[ i ], info );
      if ( stop )
        break;
    }
    tmpResult = functionEventTypes->getResult();
  }

  if ( opTypeValue == AND && !tmpResult )
    return false;
  else if( opTypeValue == OR && tmpResult )
    return true;

  if ( existEventValues )
  {
    tmpResult = functionEventValues->getDefaultValue();

    info = ( TSemanticValue ) it->getEventValue();
    for ( PRV_UINT32 i = 0; i < eventValues.size(); i++ )
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

TFilterNumParam KFilter::getCommFromFunctionNumParam() const
{
  return functionCommFrom->getNumParameters();
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

TFilterNumParam KFilter::getCommToFunctionNumParam() const
{
  return functionCommTo->getNumParameters();
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

TFilterNumParam KFilter::getCommTagFunctionNumParam() const
{
  return functionCommTags->getNumParameters();
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

TFilterNumParam KFilter::getCommSizeFunctionNumParam() const
{
  return functionCommSizes->getNumParameters();
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

TFilterNumParam KFilter::getBandWidthFunctionNumParam() const
{
  return functionBandWidth->getNumParameters();
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

TFilterNumParam KFilter::getEventTypeFunctionNumParam() const
{
  return functionEventTypes->getNumParameters();
}

void KFilter::getValidEvents( vector<TEventType>& onVector,
                              const set<TEventType>& eventsLoaded ) const
{
  bool stop = true;

  for ( set<TEventType>::const_iterator itEvt = eventsLoaded.begin();
        itEvt != eventsLoaded.end(); ++itEvt )
  {
    for ( PRV_UINT32 i = 0; i < eventTypes.size(); i++ )
    {
      stop = functionEventTypes->execute( ( TSemanticValue ) eventTypes[ i ], ( *itEvt ) );
      if ( stop )
        break;
    }
    if( functionEventTypes->getResult() )
      onVector.push_back( ( *itEvt ) );
  }

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

TFilterNumParam KFilter::getEventValueFunctionNumParam() const
{
  return functionEventValues->getNumParameters();
}

KFilter *KFilter::clone( KWindow *clonedWindow )
{
  KFilter *clonedKFilter = new KFilter( clonedWindow );

  // Constructor allocates FilterFunctions
  delete clonedKFilter->functionCommFrom;
  delete clonedKFilter->functionCommTo;
  delete clonedKFilter->functionCommTags;
  delete clonedKFilter->functionCommSizes;
  delete clonedKFilter->functionBandWidth;
  delete clonedKFilter->functionEventTypes;
  delete clonedKFilter->functionEventValues;

  // Copy values and clone FilterFunctions
  clonedKFilter->logical = logical;
  clonedKFilter->physical = physical;

  clonedKFilter->existCommFrom = existCommFrom;
  clonedKFilter->commFrom = vector<TObjectOrder>( commFrom );
  clonedKFilter->functionCommFrom = functionCommFrom->clone();

  clonedKFilter->opFromTo = opFromTo;

  clonedKFilter->existCommTo = existCommTo;
  clonedKFilter->commTo = vector<TObjectOrder>( commTo );
  clonedKFilter->functionCommTo = functionCommTo->clone();

  clonedKFilter->existCommTags = existCommTags;
  clonedKFilter->commTags = vector<TCommTag>( commTags );
  clonedKFilter->functionCommTags = functionCommTags->clone();

  clonedKFilter->opTagSize = opTagSize;

  clonedKFilter->existCommSize = existCommSize;
  clonedKFilter->commSizes = vector<TCommSize>( commSizes );
  clonedKFilter->functionCommSizes = functionCommSizes->clone();

  clonedKFilter->existBandWidth = existBandWidth;
  clonedKFilter->bandWidth = vector<TSemanticValue>( bandWidth );
  clonedKFilter->functionBandWidth = functionBandWidth->clone();

  clonedKFilter->existEventTypes = existEventTypes;
  clonedKFilter->eventTypes = vector<TEventType>( eventTypes );
  clonedKFilter->functionEventTypes = functionEventTypes->clone();

  clonedKFilter->opTypeValue = opTypeValue;

  clonedKFilter->existEventValues = existEventValues;
  clonedKFilter->eventValues = vector<TEventValue>( eventValues );
  clonedKFilter->functionEventValues = functionEventValues->clone();

  return clonedKFilter;
}
