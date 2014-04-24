/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include "filter.h"

using namespace std;

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

TFilterNumParam FilterProxy::getCommFromFunctionNumParams() const
{
  return myFilter->getCommFromFunctionNumParams();
}

bool FilterProxy::allowedCommFromFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedCommFromFunctionNumParams( numParams );
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

TFilterNumParam FilterProxy::getCommToFunctionNumParams() const
{
  return myFilter->getCommToFunctionNumParams();
}

bool FilterProxy::allowedCommToFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedCommToFunctionNumParams( numParams );
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

TFilterNumParam FilterProxy::getCommTagFunctionNumParams() const
{
  return myFilter->getCommTagFunctionNumParams();
}

bool FilterProxy::allowedCommTagFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedCommTagFunctionNumParams( numParams );
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

TFilterNumParam FilterProxy::getCommSizeFunctionNumParams() const
{
  return myFilter->getCommSizeFunctionNumParams();
}

bool FilterProxy::allowedCommSizeFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedCommSizeFunctionNumParams( numParams );
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

TFilterNumParam FilterProxy::getBandWidthFunctionNumParams() const
{
  return myFilter->getBandWidthFunctionNumParams();
}

bool FilterProxy::allowedBandWidthFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedBandWidthFunctionNumParams( numParams );
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

TFilterNumParam FilterProxy::getEventTypeFunctionNumParams() const
{
  return myFilter->getEventTypeFunctionNumParams();
}

bool FilterProxy::allowedEventTypeFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedEventTypeFunctionNumParams( numParams );
}

void FilterProxy::getValidEvents( vector<TEventType>& onVector,
                                  const set<TEventType>& eventsLoaded ) const
{
  myFilter->getValidEvents( onVector, eventsLoaded );
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

TFilterNumParam FilterProxy::getEventValueFunctionNumParams() const
{
  return myFilter->getEventValueFunctionNumParams();
}

bool FilterProxy::allowedEventValueFunctionNumParams( TFilterNumParam numParams ) const
{
  return myFilter->allowedEventValueFunctionNumParams( numParams );
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
    clearEventTypes();
    for ( vector< TEventType >::iterator it = etype.begin(); it < etype.end(); it++ )
      insertEventType( *it );

    setEventTypeFunction( whichFilter->getEventTypeFunction() );

    vector< TEventValue > evalue;
    whichFilter->getEventValue( evalue );
    clearEventValues();
    for ( vector< TEventValue >::iterator it = evalue.begin(); it < evalue.end(); it++ )
      insertEventValue( *it );

    setEventValueFunction( whichFilter->getEventValueFunction() );

    if ( whichFilter->getOpTypeValue() )
      setOpTypeValueAnd();
    else
      setOpTypeValueOr();
  }
}

void FilterProxy::copyCommunicationsSection( Filter *whichFilter )
{
  if ( whichFilter != NULL )
  {
    setLogical( whichFilter->getLogical() );
    setPhysical( whichFilter->getPhysical() );

    vector< TObjectOrder > from;
    whichFilter->getCommFrom( from );
    clearCommFrom();
    for ( vector< TObjectOrder >::iterator it = from.begin(); it < from.end(); it++ )
      insertCommFrom( *it );

    setCommFromFunction( whichFilter->getCommFromFunction() );

    vector< TObjectOrder > to;
    whichFilter->getCommTo( to );
    clearCommTo();
    for ( vector< TObjectOrder >::iterator it = to.begin(); it < to.end(); it++ )
      insertCommTo( *it );

    setCommToFunction( whichFilter->getCommToFunction() );

    vector< TCommTag > tag;
    whichFilter->getCommTag( tag );
    clearCommTags();
    for ( vector< TCommTag >::iterator it = tag.begin(); it < tag.end(); it++ )
      insertCommTag( *it );

    setCommTagFunction( whichFilter->getCommTagFunction() );

    vector< TCommSize > size;
    whichFilter->getCommSize( size );
    clearCommSizes();
    for ( vector< TCommSize >::iterator it = size.begin(); it < size.end(); it++ )
      insertCommSize( *it );

    setCommSizeFunction( whichFilter->getCommSizeFunction() );

    vector< TSemanticValue > bw;
    whichFilter->getBandWidth( bw );
    clearBandWidth();
    for ( vector< TSemanticValue >::iterator it = bw.begin(); it < bw.end(); it++ )
      insertBandWidth( *it );

    setBandWidthFunction( whichFilter->getBandWidthFunction() );

    if ( whichFilter->getOpFromTo() )
      setOpFromToAnd();
    else
      setOpFromToOr();

    if ( whichFilter->getOpTagSize() )
      setOpTagSizeAnd();
    else
      setOpTagSizeOr();
  }
}

