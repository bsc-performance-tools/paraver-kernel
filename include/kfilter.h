#ifndef _FILTER_H
#define _FILTER_H

#include <iostream>
#include "memorytrace.h"
#include "filter.h"

class KSingleWindow;


class FilterFunction
{
  public:
    FilterFunction()
    {}
    virtual ~FilterFunction()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data ) = 0;

    virtual string getName() = 0;
    virtual FilterFunction *clone() = 0;

    bool getResult()
    {
      return result;
    }

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

    virtual string getName()
    {
      return FilterAll::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterAll( *this );
    }

  protected:

  private:
    static string name;

};

class FilterNotEqual: public FilterFunction
{
  public:
    FilterNotEqual()
    {}
    virtual ~FilterNotEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual string getName()
    {
      return FilterNotEqual::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterNotEqual( *this );
    }

  protected:

  private:
    static string name;


};

class FilterEqual: public FilterFunction
{
  public:
    FilterEqual()
    {}
    virtual ~FilterEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual string getName()
    {
      return FilterEqual::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterEqual( *this );
    }

  protected:

  private:
    static string name;


};

class FilterGreater: public FilterFunction
{
  public:
    FilterGreater()
    {}
    virtual ~FilterGreater()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual string getName()
    {
      return FilterGreater::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterGreater( *this );
    }

  protected:

  private:
    static string name;

};

class FilterFewer: public FilterFunction
{
  public:
    FilterFewer()
    {}
    virtual ~FilterFewer()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual string getName()
    {
      return FilterFewer::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterFewer( *this );
    }

  protected:

  private:
    static string name;

};


class FilterNone: public FilterFunction
{
  public:
    FilterNone()
    {}
    virtual ~FilterNone()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );

    virtual string getName()
    {
      return FilterNone::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterNone( *this );
    }

  protected:

  private:
    static string name;

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

    virtual string getName()
    {
      return FilterRange::name;
    }

    virtual FilterFunction *clone()
    {
      return new FilterRange( *this );
    }

  protected:

  private:
    static const bool MINOR = true;
    static const bool MAJOR = false;
    bool position;
    static string name;

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
    void getCommFrom( vector<TObjectOrder>& onVector ) const;
    void setCommFromFunction( string newFunction );

    void clearCommTo();
    void insertCommTo( TObjectOrder value );
    void getCommTo( vector<TObjectOrder>& onVector ) const;
    void setCommToFunction( string newFunction );

    void clearCommTags();
    void insertCommTag( TCommTag value );
    void getCommTag( vector<TCommTag>& onVector ) const;
    void setCommTagFunction( string newFunction );

    void clearCommSizes();
    void insertCommSize( TCommSize value );
    void getCommSize( vector<TCommSize>& onVector ) const;
    void setCommSizeFunction( string newFunction );

    void clearBandWidth();
    void insertBandWidth( TSemanticValue value );
    void getBandWidth( vector<TSemanticValue>& onVector ) const;
    void setBandWidthFunction( string newFunction );

    void clearEventTypes();
    void insertEventType( TEventType value );
    void getEventType( vector<TEventType>& onVector ) const;
    void setEventTypeFunction( string newFunction );

    void clearEventValues();
    void insertEventValue( TEventValue value );
    void getEventValue( vector<TEventValue>& onVector ) const;
    void setEventValueFunction( string newFunction );


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

  private:
    KSingleWindow *window;

    bool logical;
    bool physical;

    bool existCommFrom;
    vector<TObjectOrder> commFrom;
    FilterFunction *functionCommFrom;

    bool opFromTo;

    bool existCommTo;
    vector<TObjectOrder> commTo;
    FilterFunction *functionCommTo;

    bool existCommTags;
    vector<TCommTag> commTags;
    FilterFunction *functionCommTags;

    bool opTagSize;

    bool existCommSize;
    vector<TCommSize> commSizes;
    FilterFunction *functionCommSizes;

    bool existBandWidth;
    vector<TSemanticValue> bandWidth;
    FilterFunction *functionBandWidth;

    bool existEventTypes;
    vector<TEventType> eventTypes;
    FilterFunction *functionEventTypes;

    bool opTypeValue;

    bool existEventValues;
    vector<TEventValue> eventValues;
    FilterFunction *functionEventValues;

    bool filterComms( MemoryTrace::iterator *it );
    bool filterEvents( MemoryTrace::iterator *it );
};


#endif // _FILTER_H_INCLUDED
