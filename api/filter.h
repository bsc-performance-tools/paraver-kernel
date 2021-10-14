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

#pragma once


#include <string>
#include <set>
#include <vector>
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
    virtual void getCommFrom( std::vector<TObjectOrder>& onVector ) const = 0;
    virtual void setCommFromFunction( std::string newFunction ) = 0;
    virtual std::string getCommFromFunction() const = 0;
    virtual TFilterNumParam getCommFromFunctionNumParams() const = 0;
    virtual bool allowedCommFromFunctionNumParams( TFilterNumParam numParams ) const = 0;

    virtual void clearCommTo() = 0;
    virtual void insertCommTo( TObjectOrder value ) = 0;
    virtual void getCommTo( std::vector<TObjectOrder>& onVector ) const = 0;
    virtual void setCommToFunction( std::string newFunction ) = 0;
    virtual std::string getCommToFunction() const = 0;
    virtual TFilterNumParam getCommToFunctionNumParams() const = 0;
    virtual bool allowedCommToFunctionNumParams( TFilterNumParam numParams ) const = 0;

    virtual void clearCommTags() = 0;
    virtual void insertCommTag( TCommTag value ) = 0;
    virtual void getCommTag( std::vector<TCommTag>& onVector ) const = 0;
    virtual void setCommTagFunction( std::string newFunction ) = 0;
    virtual std::string getCommTagFunction() const = 0;
    virtual TFilterNumParam getCommTagFunctionNumParams() const = 0;
    virtual bool allowedCommTagFunctionNumParams( TFilterNumParam numParams ) const = 0;

    virtual void clearCommSizes() = 0;
    virtual void insertCommSize( TCommSize value ) = 0;
    virtual void getCommSize( std::vector<TCommSize>& onVector ) const = 0;
    virtual void setCommSizeFunction( std::string newFunction ) = 0;
    virtual std::string getCommSizeFunction() const = 0;
    virtual TFilterNumParam getCommSizeFunctionNumParams() const = 0;
    virtual bool allowedCommSizeFunctionNumParams( TFilterNumParam numParams ) const = 0;

    virtual void clearBandWidth() = 0;
    virtual void insertBandWidth( TSemanticValue value ) = 0;
    virtual void getBandWidth( std::vector<TSemanticValue>& onVector ) const = 0;
    virtual void setBandWidthFunction( std::string newFunction ) = 0;
    virtual std::string getBandWidthFunction() const = 0;
    virtual TFilterNumParam getBandWidthFunctionNumParams() const = 0;
    virtual bool allowedBandWidthFunctionNumParams( TFilterNumParam numParams ) const = 0;

    virtual void clearEventTypes() = 0;
    virtual void insertEventType( TEventType value ) = 0;
    virtual void getEventType( std::vector<TEventType>& onVector ) const = 0;
    virtual void setEventTypeFunction( std::string newFunction ) = 0;
    virtual std::string getEventTypeFunction() const = 0;
    virtual TFilterNumParam getEventTypeFunctionNumParams() const = 0;
    virtual bool allowedEventTypeFunctionNumParams( TFilterNumParam numParams ) const = 0;
    virtual void getValidEvents( std::vector<TEventType>& onVector,
                                 const std::set<TEventType>& eventsLoaded ) const = 0;

    virtual void clearEventValues() = 0;
    virtual void insertEventValue( TSemanticValue value ) = 0;
    virtual void getEventValue( std::vector<TSemanticValue>& onVector ) const = 0;
    virtual void setEventValueFunction( std::string newFunction ) = 0;
    virtual std::string getEventValueFunction() const = 0;
    virtual TFilterNumParam getEventValueFunctionNumParams() const = 0;
    virtual bool allowedEventValueFunctionNumParams( TFilterNumParam numParams ) const = 0;


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

    virtual void getAllFilterFunctions( std::vector<std::string>& onVector ) const;

  private:
    const KernelConnection *myKernel;
};

class FilterProxy : public Filter
{
  public:
    virtual ~FilterProxy()
    {}

    virtual void setLogical( bool newValue ) override;

    virtual void setPhysical( bool newValue ) override;

    virtual bool getLogical() override;

    virtual bool getPhysical() override;

    virtual void clearCommFrom() override;
    virtual void insertCommFrom( TObjectOrder value ) override;
    virtual void getCommFrom( std::vector<TObjectOrder>& onVector ) const override;
    virtual void setCommFromFunction( std::string newFunction ) override;
    virtual std::string getCommFromFunction() const override;
    virtual TFilterNumParam getCommFromFunctionNumParams() const override;
    virtual bool allowedCommFromFunctionNumParams( TFilterNumParam numParams ) const override;

    virtual void clearCommTo() override;
    virtual void insertCommTo( TObjectOrder value ) override;
    virtual void getCommTo( std::vector<TObjectOrder>& onVector ) const override;
    virtual void setCommToFunction( std::string newFunction ) override;
    virtual std::string getCommToFunction() const override;
    virtual TFilterNumParam getCommToFunctionNumParams() const override;
    virtual bool allowedCommToFunctionNumParams( TFilterNumParam numParams ) const override;

    virtual void clearCommTags() override;
    virtual void insertCommTag( TCommTag value ) override;
    virtual void getCommTag( std::vector<TCommTag>& onVector ) const override;
    virtual void setCommTagFunction( std::string newFunction ) override;
    virtual std::string getCommTagFunction() const override;
    virtual TFilterNumParam getCommTagFunctionNumParams() const override;
    virtual bool allowedCommTagFunctionNumParams( TFilterNumParam numParams ) const override;

    virtual void clearCommSizes() override;
    virtual void insertCommSize( TCommSize value ) override;
    virtual void getCommSize( std::vector<TCommSize>& onVector ) const override;
    virtual void setCommSizeFunction( std::string newFunction ) override;
    virtual std::string getCommSizeFunction() const override;
    virtual TFilterNumParam getCommSizeFunctionNumParams() const override;
    virtual bool allowedCommSizeFunctionNumParams( TFilterNumParam numParams ) const override;

    virtual void clearBandWidth() override;
    virtual void insertBandWidth( TSemanticValue value ) override;
    virtual void getBandWidth( std::vector<TSemanticValue>& onVector ) const override;
    virtual void setBandWidthFunction( std::string newFunction ) override;
    virtual std::string getBandWidthFunction() const override;
    virtual TFilterNumParam getBandWidthFunctionNumParams() const override;
    virtual bool allowedBandWidthFunctionNumParams( TFilterNumParam numParams ) const override;

    virtual void clearEventTypes() override;
    virtual void insertEventType( TEventType value ) override;
    virtual void getEventType( std::vector<TEventType>& onVector ) const override;
    virtual void setEventTypeFunction( std::string newFunction ) override;
    virtual std::string getEventTypeFunction() const override;
    virtual TFilterNumParam getEventTypeFunctionNumParams() const override;
    virtual bool allowedEventTypeFunctionNumParams( TFilterNumParam numParams ) const override;
    virtual void getValidEvents( std::vector<TEventType>& onVector,
                                 const std::set<TEventType>& eventsLoaded ) const override;

    virtual void clearEventValues() override;
    virtual void insertEventValue( TSemanticValue value ) override;
    virtual void getEventValue( std::vector<TSemanticValue>& onVector ) const override;
    virtual void setEventValueFunction( std::string newFunction ) override;
    virtual std::string getEventValueFunction() const override;
    virtual TFilterNumParam getEventValueFunctionNumParams() const override;
    virtual bool allowedEventValueFunctionNumParams( TFilterNumParam numParams ) const override;


    virtual void setOpFromToAnd() override;
    virtual void setOpFromToOr() override;
    virtual void setOpTagSizeAnd() override;
    virtual void setOpTagSizeOr() override;
    virtual void setOpTypeValueAnd() override;
    virtual void setOpTypeValueOr() override;

    virtual bool getOpFromTo() const override;
    virtual bool getOpTagSize() const override;
    virtual bool getOpTypeValue() const override;

    virtual void copyEventsSection( Filter *filter ) override;
    virtual void copyCommunicationsSection( Filter *filter ) override;

  private:
    Filter *myFilter;

    FilterProxy()
    {}

    FilterProxy( const KernelConnection *whichKernel ) : Filter( whichKernel )
    {}

    friend Filter *LocalKernel::newFilter( Filter * ) const;
};


