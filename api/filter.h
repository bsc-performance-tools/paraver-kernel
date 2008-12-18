#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"
#include "localkernel.h"

class Window;

class Filter
{
  public:
    static const bool AND = true;
    static const bool OR = false;

    Filter()
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

    virtual void clearCommTo() = 0;
    virtual void insertCommTo( TObjectOrder value ) = 0;
    virtual void getCommTo( vector<TObjectOrder>& onVector ) const = 0;
    virtual void setCommToFunction( string newFunction ) = 0;

    virtual void clearCommTags() = 0;
    virtual void insertCommTag( TCommTag value ) = 0;
    virtual void getCommTag( vector<TCommTag>& onVector ) const = 0;
    virtual void setCommTagFunction( string newFunction ) = 0;

    virtual void clearCommSizes() = 0;
    virtual void insertCommSize( TCommSize value ) = 0;
    virtual void getCommSize( vector<TCommSize>& onVector ) const = 0;
    virtual void setCommSizeFunction( string newFunction ) = 0;

    virtual void clearBandWidth() = 0;
    virtual void insertBandWidth( TSemanticValue value ) = 0;
    virtual void getBandWidth( vector<TSemanticValue>& onVector ) const = 0;
    virtual void setBandWidthFunction( string newFunction ) = 0;

    virtual void clearEventTypes() = 0;
    virtual void insertEventType( TEventType value ) = 0;
    virtual void getEventType( vector<TEventType>& onVector ) const = 0;
    virtual void setEventTypeFunction( string newFunction ) = 0;

    virtual void clearEventValues() = 0;
    virtual void insertEventValue( TEventValue value ) = 0;
    virtual void getEventValue( vector<TEventValue>& onVector ) const = 0;
    virtual void setEventValueFunction( string newFunction ) = 0;


    virtual void setOpFromToAnd() = 0;
    virtual void setOpFromToOr() = 0;
    virtual void setOpTagSizeAnd() = 0;
    virtual void setOpTagSizeOr() = 0;
    virtual void setOpTypeValueAnd() = 0;
    virtual void setOpTypeValueOr() = 0;

  private:
};

class FilterProxy : public Filter
{
  public:
    FilterProxy()
    {}

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

    virtual void clearCommTo();
    virtual void insertCommTo( TObjectOrder value );
    virtual void getCommTo( vector<TObjectOrder>& onVector ) const;
    virtual void setCommToFunction( string newFunction );

    virtual void clearCommTags();
    virtual void insertCommTag( TCommTag value );
    virtual void getCommTag( vector<TCommTag>& onVector ) const;
    virtual void setCommTagFunction( string newFunction );

    virtual void clearCommSizes();
    virtual void insertCommSize( TCommSize value );
    virtual void getCommSize( vector<TCommSize>& onVector ) const;
    virtual void setCommSizeFunction( string newFunction );

    virtual void clearBandWidth();
    virtual void insertBandWidth( TSemanticValue value );
    virtual void getBandWidth( vector<TSemanticValue>& onVector ) const;
    virtual void setBandWidthFunction( string newFunction );

    virtual void clearEventTypes();
    virtual void insertEventType( TEventType value );
    virtual void getEventType( vector<TEventType>& onVector ) const;
    virtual void setEventTypeFunction( string newFunction );

    virtual void clearEventValues();
    virtual void insertEventValue( TEventValue value );
    virtual void getEventValue( vector<TEventValue>& onVector ) const;
    virtual void setEventValueFunction( string newFunction );


    virtual void setOpFromToAnd();
    virtual void setOpFromToOr();
    virtual void setOpTagSizeAnd();
    virtual void setOpTagSizeOr();
    virtual void setOpTypeValueAnd();
    virtual void setOpTypeValueOr();

  private:
    Filter *myFilter;

    friend Filter *LocalKernel::newFilter( Filter * ) const;
};

#endif // FILTER_H_INCLUDED
