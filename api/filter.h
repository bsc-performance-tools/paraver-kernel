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

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <string>
#include <set>
#include "paraverkerneltypes.h"
#include "localkernel.h"

using std::set;

class Window;

class Filter
{
  public:
    static const bool AND = true;
    static const bool OR = false;

    Filter()
    {}

    Filter( const KernelConnection *whichKernel ) : myKernel( whichKernel )
    {}

    virtual ~Filter()
    {}

    virtual void setLogical( bool newValue ) = 0;

    virtual void setPhysical( bool newValue ) = 0;

    virtual bool getLogical() = 0;

    virtual bool getPhysical() = 0;

    virtual void clearCommFrom() = 0;
    virtual void insertCommFrom( TObjectOrder value ) = 0;
    virtual void getCommFrom( vector<TObjectOrder>& onVector ) const = 0;
    virtual void setCommFromFunction( string newFunction ) = 0;
    virtual string getCommFromFunction() const = 0;

    virtual void clearCommTo() = 0;
    virtual void insertCommTo( TObjectOrder value ) = 0;
    virtual void getCommTo( vector<TObjectOrder>& onVector ) const = 0;
    virtual void setCommToFunction( string newFunction ) = 0;
    virtual string getCommToFunction() const = 0;

    virtual void clearCommTags() = 0;
    virtual void insertCommTag( TCommTag value ) = 0;
    virtual void getCommTag( vector<TCommTag>& onVector ) const = 0;
    virtual void setCommTagFunction( string newFunction ) = 0;
    virtual string getCommTagFunction() const = 0;

    virtual void clearCommSizes() = 0;
    virtual void insertCommSize( TCommSize value ) = 0;
    virtual void getCommSize( vector<TCommSize>& onVector ) const = 0;
    virtual void setCommSizeFunction( string newFunction ) = 0;
    virtual string getCommSizeFunction() const = 0;

    virtual void clearBandWidth() = 0;
    virtual void insertBandWidth( TSemanticValue value ) = 0;
    virtual void getBandWidth( vector<TSemanticValue>& onVector ) const = 0;
    virtual void setBandWidthFunction( string newFunction ) = 0;
    virtual string getBandWidthFunction() const = 0;

    virtual void clearEventTypes() = 0;
    virtual void insertEventType( TEventType value ) = 0;
    virtual void getEventType( vector<TEventType>& onVector ) const = 0;
    virtual void setEventTypeFunction( string newFunction ) = 0;
    virtual string getEventTypeFunction() const = 0;
    virtual void getValidEvents( vector<TEventType>& onVector,
                                 const set<TEventType>& eventsLoaded ) const = 0;

    virtual void clearEventValues() = 0;
    virtual void insertEventValue( TEventValue value ) = 0;
    virtual void getEventValue( vector<TEventValue>& onVector ) const = 0;
    virtual void setEventValueFunction( string newFunction ) = 0;
    virtual string getEventValueFunction() const = 0;


    virtual void setOpFromToAnd() = 0;
    virtual void setOpFromToOr() = 0;
    virtual void setOpTagSizeAnd() = 0;
    virtual void setOpTagSizeOr() = 0;
    virtual void setOpTypeValueAnd() = 0;
    virtual void setOpTypeValueOr() = 0;

    virtual bool getOpFromTo() const = 0;
    virtual bool getOpTagSize() const = 0;
    virtual bool getOpTypeValue() const = 0;

    virtual void copyEventsSection( Filter *filter ) {};
    virtual void copyCommunicationsSection( Filter *filter ) {};

    virtual void getAllFilterFunctions( vector<string>& onVector ) const;

  private:
    const KernelConnection *myKernel;
};

class FilterProxy : public Filter
{
  public:
    virtual ~FilterProxy()
    {}

    virtual void setLogical( bool newValue );

    virtual void setPhysical( bool newValue );

    virtual bool getLogical();

    virtual bool getPhysical();

    virtual void clearCommFrom();
    virtual void insertCommFrom( TObjectOrder value );
    virtual void getCommFrom( vector<TObjectOrder>& onVector ) const;
    virtual void setCommFromFunction( string newFunction );
    virtual string getCommFromFunction() const;

    virtual void clearCommTo();
    virtual void insertCommTo( TObjectOrder value );
    virtual void getCommTo( vector<TObjectOrder>& onVector ) const;
    virtual void setCommToFunction( string newFunction );
    virtual string getCommToFunction() const;

    virtual void clearCommTags();
    virtual void insertCommTag( TCommTag value );
    virtual void getCommTag( vector<TCommTag>& onVector ) const;
    virtual void setCommTagFunction( string newFunction );
    virtual string getCommTagFunction() const;

    virtual void clearCommSizes();
    virtual void insertCommSize( TCommSize value );
    virtual void getCommSize( vector<TCommSize>& onVector ) const;
    virtual void setCommSizeFunction( string newFunction );
    virtual string getCommSizeFunction() const;

    virtual void clearBandWidth();
    virtual void insertBandWidth( TSemanticValue value );
    virtual void getBandWidth( vector<TSemanticValue>& onVector ) const;
    virtual void setBandWidthFunction( string newFunction );
    virtual string getBandWidthFunction() const;

    virtual void clearEventTypes();
    virtual void insertEventType( TEventType value );
    virtual void getEventType( vector<TEventType>& onVector ) const;
    virtual void setEventTypeFunction( string newFunction );
    virtual string getEventTypeFunction() const;
    virtual void getValidEvents( vector<TEventType>& onVector,
                                 const set<TEventType>& eventsLoaded ) const;

    virtual void clearEventValues();
    virtual void insertEventValue( TEventValue value );
    virtual void getEventValue( vector<TEventValue>& onVector ) const;
    virtual void setEventValueFunction( string newFunction );
    virtual string getEventValueFunction() const;


    virtual void setOpFromToAnd();
    virtual void setOpFromToOr();
    virtual void setOpTagSizeAnd();
    virtual void setOpTagSizeOr();
    virtual void setOpTypeValueAnd();
    virtual void setOpTypeValueOr();

    virtual bool getOpFromTo() const;
    virtual bool getOpTagSize() const;
    virtual bool getOpTypeValue() const;

    virtual void copyEventsSection( Filter *filter );
    virtual void copyCommunicationsSection( Filter *filter );

  private:
    Filter *myFilter;

    FilterProxy()
    {}

    FilterProxy( const KernelConnection *whichKernel ) : Filter( whichKernel )
    {}

    friend Filter *LocalKernel::newFilter( Filter * ) const;
};

#endif // FILTER_H_INCLUDED
