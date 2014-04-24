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

#ifndef _FILTER_H
#define _FILTER_H

#include <limits>
#include <iostream>
#include <set>
#include "memorytrace.h"
#include "filter.h"

class KSingleWindow;
class KWindow;

class FilterFunction
{
  public:
    FilterFunction()
    {}
    virtual ~FilterFunction()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data ) = 0;

    virtual std::string getName() = 0;
    virtual FilterFunction *clone() = 0;

    virtual bool getDefaultValue() const
    {
      return false;
    }

    virtual bool getResult() const
    {
      return result;
    }

    virtual TFilterNumParam getNumParameters() const = 0;
    virtual bool allowedNumParameters( TFilterNumParam numParams ) const = 0;

  protected:
    bool result;
  private:

};

class FilterAll: public FilterFunction
{
  public:
    FilterAll()
    {}
    virtual ~FilterAll()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterAll::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterAll( *this );
    }

    virtual bool getDefaultValue() const
    {
      return true;
    }

    virtual bool getResult() const
    {
      return true;
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return 0;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return true;
    }

  protected:

  private:
    static std::string name;

};

class FilterNotEqual: public FilterFunction
{
  public:
    FilterNotEqual()
    {}
    virtual ~FilterNotEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterNotEqual::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterNotEqual( *this );
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return std::numeric_limits<TFilterNumParam>::max();
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return numParams <= getNumParameters();
    }

  protected:

  private:
    static std::string name;


};

class FilterEqual: public FilterFunction
{
  public:
    FilterEqual()
    {}
    virtual ~FilterEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterEqual::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterEqual( *this );
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return std::numeric_limits<TFilterNumParam>::max();
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return numParams <= getNumParameters();
    }

  protected:

  private:
    static std::string name;


};

class FilterGreater: public FilterFunction
{
  public:
    FilterGreater()
    {}
    virtual ~FilterGreater()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterGreater::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterGreater( *this );
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return 1;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return numParams == getNumParameters();
    }

  protected:

  private:
    static std::string name;

};

class FilterFewer: public FilterFunction
{
  public:
    FilterFewer()
    {}
    virtual ~FilterFewer()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterFewer::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterFewer( *this );
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return 1;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return numParams == getNumParameters();
    }

  protected:

  private:
    static std::string name;

};


class FilterNone: public FilterFunction
{
  public:
    FilterNone()
    {}
    virtual ~FilterNone()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterNone::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterNone( *this );
    }

    virtual bool getResult() const
    {
      return false;
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return 0;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return true;
    }

  protected:

  private:
    static std::string name;

};

class FilterRange: public FilterFunction
{
  public:
    FilterRange()
    {
      position = MINOR;
    }
    virtual ~FilterRange()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual std::string getName()
    {
      return FilterRange::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterRange( *this );
    }

    virtual TFilterNumParam getNumParameters() const
    {
      return 2;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const
    {
      return numParams == getNumParameters();
    }

  protected:

  private:
    static const bool MINOR = true;
    static const bool MAJOR = false;
    bool position;
    static std::string name;

};

class KFilter : public Filter
{
  public:
    static const bool AND = true;
    static const bool OR = false;

    KFilter()
    {
      logical = true;
      physical = false;
      existCommFrom = false;
      existCommTo = false;
      existCommTags = false;
      existCommSize = false;
      existBandWidth = false;
      existEventTypes = false;
      existEventValues = false;
      opFromTo = AND;
      opTagSize = AND;
      opTypeValue = AND;
      functionCommFrom = new FilterAll();
      functionCommTo = new FilterAll();
      functionCommTags = new FilterAll();
      functionCommSizes = new FilterAll();
      functionBandWidth = new FilterAll();
      functionEventTypes = new FilterAll();
      functionEventValues = new FilterAll();
    }

    KFilter( Window *whichWindow ) : window( (KSingleWindow *)whichWindow )
    {
      logical = true;
      physical = false;
      existCommFrom = false;
      existCommTo = false;
      existCommTags = false;
      existCommSize = false;
      existBandWidth = false;
      existEventTypes = false;
      existEventValues = false;
      opFromTo = AND;
      opTagSize = AND;
      opTypeValue = AND;
      functionCommFrom = new FilterAll();
      functionCommTo = new FilterAll();
      functionCommTags = new FilterAll();
      functionCommSizes = new FilterAll();
      functionBandWidth = new FilterAll();
      functionEventTypes = new FilterAll();
      functionEventValues = new FilterAll();
    }

    ~KFilter()
    {
      delete functionCommFrom;
      delete functionCommTo;
      delete functionCommTags;
      delete functionCommSizes;
      delete functionBandWidth;
      delete functionEventTypes;
      delete functionEventValues;
    }

    bool passFilter( MemoryTrace::iterator *it );

    void setLogical( bool newValue )
    {
      logical = newValue;
    }

    void setPhysical( bool newValue )
    {
      physical = newValue;
    }

    bool getLogical()
    {
      return logical;
    }

    bool getPhysical()
    {
      return physical;
    }

    void clearCommFrom();
    void insertCommFrom( TObjectOrder value );
    void getCommFrom( std::vector<TObjectOrder>& onVector ) const;
    void setCommFromFunction( std::string newFunction );
    std::string getCommFromFunction() const;
    TFilterNumParam getCommFromFunctionNumParams() const;
    bool allowedCommFromFunctionNumParams( TFilterNumParam numParams ) const;

    void clearCommTo();
    void insertCommTo( TObjectOrder value );
    void getCommTo( std::vector<TObjectOrder>& onVector ) const;
    void setCommToFunction( std::string newFunction );
    std::string getCommToFunction() const;
    TFilterNumParam getCommToFunctionNumParams() const;
    bool allowedCommToFunctionNumParams( TFilterNumParam numParams ) const;

    void clearCommTags();
    void insertCommTag( TCommTag value );
    void getCommTag( std::vector<TCommTag>& onVector ) const;
    void setCommTagFunction( std::string newFunction );
    std::string getCommTagFunction() const;
    TFilterNumParam getCommTagFunctionNumParams() const;
    bool allowedCommTagFunctionNumParams( TFilterNumParam numParams ) const;

    void clearCommSizes();
    void insertCommSize( TCommSize value );
    void getCommSize( std::vector<TCommSize>& onVector ) const;
    void setCommSizeFunction( std::string newFunction );
    std::string getCommSizeFunction() const;
    TFilterNumParam getCommSizeFunctionNumParams() const;
    bool allowedCommSizeFunctionNumParams( TFilterNumParam numParams ) const;

    void clearBandWidth();
    void insertBandWidth( TSemanticValue value );
    void getBandWidth( std::vector<TSemanticValue>& onVector ) const;
    void setBandWidthFunction( std::string newFunction );
    std::string getBandWidthFunction() const;
    TFilterNumParam getBandWidthFunctionNumParams() const;
    bool allowedBandWidthFunctionNumParams( TFilterNumParam numParams ) const;

    void clearEventTypes();
    void insertEventType( TEventType value );
    void getEventType( std::vector<TEventType>& onVector ) const;
    void setEventTypeFunction( std::string newFunction );
    std::string getEventTypeFunction() const;
    TFilterNumParam getEventTypeFunctionNumParams() const;
    bool allowedEventTypeFunctionNumParams( TFilterNumParam numParams ) const;
    void getValidEvents( std::vector<TEventType>& onVector,
                         const std::set<TEventType>& eventsLoaded ) const;

    void clearEventValues();
    void insertEventValue( TEventValue value );
    void getEventValue( std::vector<TEventValue>& onVector ) const;
    void setEventValueFunction( std::string newFunction );
    std::string getEventValueFunction() const;
    TFilterNumParam getEventValueFunctionNumParams() const;
    bool allowedEventValueFunctionNumParams( TFilterNumParam numParams ) const;


    void setOpFromToAnd()
    {
      opFromTo = AND;
    }
    void setOpFromToOr()
    {
      opFromTo = OR;
    }
    void setOpTagSizeAnd()
    {
      opTagSize = AND;
    }
    void setOpTagSizeOr()
    {
      opTagSize = OR;
    }
    void setOpTypeValueAnd()
    {
      opTypeValue = AND;
    }
    void setOpTypeValueOr()
    {
      opTypeValue = OR;
    }

    bool getOpFromTo() const
    {
      return opFromTo;
    }
    bool getOpTagSize() const
    {
      return opTagSize;
    }
    bool getOpTypeValue() const
    {
      return opTypeValue;
    }

    KFilter *clone( KWindow *clonedWindow );

  private:
    KSingleWindow *window;

    bool logical;
    bool physical;

    bool existCommFrom;
    std::vector<TObjectOrder> commFrom;
    FilterFunction *functionCommFrom;

    bool opFromTo;

    bool existCommTo;
    std::vector<TObjectOrder> commTo;
    FilterFunction *functionCommTo;

    bool existCommTags;
    std::vector<TCommTag> commTags;
    FilterFunction *functionCommTags;

    bool opTagSize;

    bool existCommSize;
    std::vector<TCommSize> commSizes;
    FilterFunction *functionCommSizes;

    bool existBandWidth;
    std::vector<TSemanticValue> bandWidth;
    FilterFunction *functionBandWidth;

    bool existEventTypes;
    std::vector<TEventType> eventTypes;
    FilterFunction *functionEventTypes;

    bool opTypeValue;

    bool existEventValues;
    std::vector<TEventValue> eventValues;
    FilterFunction *functionEventValues;

    bool filterComms( MemoryTrace::iterator *it );
    bool filterEvents( MemoryTrace::iterator *it );
};


#endif // _FILTER_H_INCLUDED
