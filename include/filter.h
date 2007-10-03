#ifndef _FILTER_H
#define _FILTER_H

#include <iostream>
#include "memorytrace.h"

class KSingleWindow;


class FilterFunction
{
  public:
    FilterFunction()
    {}
    virtual ~FilterFunction()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data ) = 0;
  protected:

  private:

};

class FilterAll:public FilterFunction
{
  public:
    FilterAll()
    {}
    virtual ~FilterAll()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};

class FilterNotEqual:public FilterFunction
{
  public:
    FilterNotEqual()
    {}
    virtual ~FilterNotEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};

class FilterEqual:public FilterFunction
{
  public:
    FilterEqual()
    {}
    virtual ~FilterEqual()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};

class FilterGreater:public FilterFunction
{
  public:
    FilterGreater()
    {}
    virtual ~FilterGreater()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};

class FilterFewer:public FilterFunction
{
  public:
    FilterFewer()
    {}
    virtual ~FilterFewer()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};


class FilterNone:public FilterFunction
{
  public:
    FilterNone()
    {}
    virtual ~FilterNone()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:

};

class FilterRange:public FilterFunction
{
  public:
    FilterRange()
    {
      position = MINOR;
    }
    virtual ~FilterRange()
    {}

    virtual bool execute( TSemanticValue param, TSemanticValue data );
  protected:

  private:
    static const bool MINOR = true;
    static const bool MAJOR = false;
    bool position;
};

class Filter
{
  public:
    static const bool AND = true;
    static const bool OR = false;

    Filter()
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

    Filter( KSingleWindow *whichWindow ) : window( whichWindow )
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
    void setCommFromFunction( FilterFunction *newFunction );

    void clearCommTo();
    void insertCommTo( TObjectOrder value );
    void setCommToFunction( FilterFunction *newFunction );

    void clearCommTags();
    void insertCommTag( TCommTag value );
    void setCommTagFunction( FilterFunction *newFunction );

    void clearCommSizes();
    void insertCommSize( TCommSize value );
    void setCommSizeFunction( FilterFunction *newFunction );

    void clearBandWidth();
    void insertBandWitdh( TSemanticValue value );
    void setBandWidthFunction( FilterFunction *newFunction );

    void clearEventTypes();
    void insertEventType( TEventType value );
    void setEventTypeFunction( FilterFunction *newFunction );

    void clearEventValues();
    void insertEventValue( TEventValue value );
    void setEventValueFunction( FilterFunction *newFunction );


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
