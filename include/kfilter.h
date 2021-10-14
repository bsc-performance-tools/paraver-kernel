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

#pragma once


#include <limits>
#include <iostream>
#include <set>
#include "memorytrace.h"
#include "filter.h"

#ifdef WIN32
#undef max
#undef min
#endif

class KSingleWindow;
class KWindow;

class FilterFunction
{
  public:
    FilterFunction()
    {}
    virtual ~FilterFunction()
    {}

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) = 0;

    virtual std::string getName() = 0;
    virtual FilterFunction *clone() = 0;

    virtual bool getDefaultValue() const
    {
      return false;
    }

    virtual TFilterNumParam getNumParameters() const = 0;
    virtual bool allowedNumParameters( TFilterNumParam numParams ) const = 0;

};

class FilterAll: public FilterFunction
{
  public:
    FilterAll()
    {}
    virtual ~FilterAll()
    {}

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterAll::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterAll( *this );
    }

    virtual bool getDefaultValue() const override
    {
      return true;
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return 0;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
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

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterNotEqual::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterNotEqual( *this );
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return std::numeric_limits<TFilterNumParam>::max();
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
    {
      return 0 < numParams && numParams <= getNumParameters();
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

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterEqual::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterEqual( *this );
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return std::numeric_limits<TFilterNumParam>::max();
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
    {
      return 0 < numParams && numParams <= getNumParameters();
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

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterGreater::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterGreater( *this );
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return 1;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
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

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterFewer::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterFewer( *this );
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return 1;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
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

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterNone::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterNone( *this );
    }

    virtual bool getResult() const
    {
      return false;
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return 0;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
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
    {}
    virtual ~FilterRange()
    {}

    virtual bool execute( TSemanticValue param, TFilterNumParam numParam, TSemanticValue data, bool& result ) override;

    virtual std::string getName() override
    {
      return FilterRange::name;
    }

    virtual FilterFunction *clone() override
    {
      return new FilterRange( *this );
    }

    virtual TFilterNumParam getNumParameters() const override
    {
      return 2;
    }

    virtual bool allowedNumParameters( TFilterNumParam numParams ) const override
    {
      return numParams == getNumParameters();
    }

  protected:

  private:
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

    void setLogical( bool newValue ) override
    {
      logical = newValue;
    }

    void setPhysical( bool newValue ) override
    {
      physical = newValue;
    }

    bool getLogical() override
    {
      return logical;
    }

    bool getPhysical() override
    {
      return physical;
    }

    void clearCommFrom() override;
    void insertCommFrom( TObjectOrder value ) override;
    void getCommFrom( std::vector<TObjectOrder>& onVector ) const override;
    void setCommFromFunction( std::string newFunction ) override;
    std::string getCommFromFunction() const override;
    TFilterNumParam getCommFromFunctionNumParams() const override;
    bool allowedCommFromFunctionNumParams( TFilterNumParam numParams ) const override;

    void clearCommTo() override;
    void insertCommTo( TObjectOrder value ) override;
    void getCommTo( std::vector<TObjectOrder>& onVector ) const override;
    void setCommToFunction( std::string newFunction ) override;
    std::string getCommToFunction() const override;
    TFilterNumParam getCommToFunctionNumParams() const override;
    bool allowedCommToFunctionNumParams( TFilterNumParam numParams ) const override;

    void clearCommTags() override;
    void insertCommTag( TCommTag value ) override;
    void getCommTag( std::vector<TCommTag>& onVector ) const override;
    void setCommTagFunction( std::string newFunction ) override;
    std::string getCommTagFunction() const override;
    TFilterNumParam getCommTagFunctionNumParams() const override;
    bool allowedCommTagFunctionNumParams( TFilterNumParam numParams ) const override;

    void clearCommSizes() override;
    void insertCommSize( TCommSize value ) override;
    void getCommSize( std::vector<TCommSize>& onVector ) const override;
    void setCommSizeFunction( std::string newFunction ) override;
    std::string getCommSizeFunction() const override;
    TFilterNumParam getCommSizeFunctionNumParams() const override;
    bool allowedCommSizeFunctionNumParams( TFilterNumParam numParams ) const override;

    void clearBandWidth() override;
    void insertBandWidth( TSemanticValue value ) override;
    void getBandWidth( std::vector<TSemanticValue>& onVector ) const override;
    void setBandWidthFunction( std::string newFunction ) override;
    std::string getBandWidthFunction() const override;
    TFilterNumParam getBandWidthFunctionNumParams() const override;
    bool allowedBandWidthFunctionNumParams( TFilterNumParam numParams ) const override;

    void clearEventTypes() override;
    void insertEventType( TEventType value ) override;
    void getEventType( std::vector<TEventType>& onVector ) const override;
    void setEventTypeFunction( std::string newFunction ) override;
    std::string getEventTypeFunction() const override;
    TFilterNumParam getEventTypeFunctionNumParams() const override;
    bool allowedEventTypeFunctionNumParams( TFilterNumParam numParams ) const override;
    void getValidEvents( std::vector<TEventType>& onVector,
                         const std::set<TEventType>& eventsLoaded ) const override;

    void clearEventValues() override;
    void insertEventValue( TSemanticValue value ) override;
    void getEventValue( std::vector<TSemanticValue>& onVector ) const override;
    void setEventValueFunction( std::string newFunction ) override;
    std::string getEventValueFunction() const override;
    TFilterNumParam getEventValueFunctionNumParams() const override;
    bool allowedEventValueFunctionNumParams( TFilterNumParam numParams ) const override;


    void setOpFromToAnd() override
    {
      opFromTo = AND;
    }
    void setOpFromToOr() override
    {
      opFromTo = OR;
    }
    void setOpTagSizeAnd() override
    {
      opTagSize = AND;
    }
    void setOpTagSizeOr() override
    {
      opTagSize = OR;
    }
    void setOpTypeValueAnd() override
    {
      opTypeValue = AND;
    }
    void setOpTypeValueOr() override
    {
      opTypeValue = OR;
    }

    bool getOpFromTo() const override
    {
      return opFromTo;
    }
    bool getOpTagSize() const override
    {
      return opTagSize;
    }
    bool getOpTypeValue() const override
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
    std::vector<TSemanticValue> eventValues;
    FilterFunction *functionEventValues;

    bool filterComms( MemoryTrace::iterator *it );
    bool filterEvents( MemoryTrace::iterator *it );
};



