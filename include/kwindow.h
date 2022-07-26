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


#include <vector>
#include "kwindowexception.h"
#include "ktrace.h"
#include "intervalnotthread.h"
#include "intervalthread.h"
#include "intervalcpu.h"
#include "intervalcompose.h"
#include "intervalderived.h"
#include "intervalcontrolderived.h"
#include "semanticthread.h"
#include "semanticcompose.h"
#include "kfilter.h"
#include "window.h"

class KTimeline: public Timeline
{
  public:
    KTimeline()
    {}
    KTimeline( Trace *whichTrace ): myTrace( (KTrace*)whichTrace )
    {
      timeUnit = NS;
      level = TTraceLevel::THREAD;
    }
    virtual ~KTimeline();

    virtual Filter *getFilter() const override
    {
      return nullptr;
    }

    Trace *getTrace() const override
    {
      return myTrace;
    }

    virtual TTraceLevel getLevel() const override
    {
      return level;
    }

    virtual void setLevel( TTraceLevel whichLevel ) override
    {
      level = whichLevel;
    }

    virtual TTraceLevel getMinAcceptableLevel() const override
    {
      TTraceLevel minAcceptableLevel = TTraceLevel::THREAD;

      if ( level >= TTraceLevel::SYSTEM && level <= TTraceLevel::CPU )
        minAcceptableLevel = TTraceLevel::CPU;

      return minAcceptableLevel;
    }

    void setTimeUnit( TTimeUnit whichUnit ) override
    {
      timeUnit = whichUnit;
    }

    TTimeUnit getTimeUnit() const override
    {
      return timeUnit;
    }

    TWindowLevel getComposeLevel( TTraceLevel whichLevel ) const override
    {
      if ( whichLevel == TTraceLevel::WORKLOAD )
        return COMPOSEWORKLOAD;
      else if ( whichLevel == TTraceLevel::APPLICATION )
        return COMPOSEAPPLICATION;
      else if ( whichLevel == TTraceLevel::TASK )
        return COMPOSETASK;
      else if ( whichLevel == TTraceLevel::THREAD )
        return COMPOSETHREAD;
      else if ( whichLevel == TTraceLevel::SYSTEM )
        return COMPOSESYSTEM;
      else if ( whichLevel == TTraceLevel::NODE )
        return COMPOSENODE;
      else if ( whichLevel == TTraceLevel::CPU )
        return COMPOSECPU;

      return NONE;
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) override = 0;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) const override = 0;
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel ) = 0;
    virtual std::string getFirstUsefulFunction( ) override = 0;
    virtual TWindowLevel getFirstFreeCompose() const override = 0;
    virtual SemanticFunction *getFirstSemUsefulFunction() = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) override = 0;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) override = 0;
    virtual void removeExtraCompose( TWindowLevel whichLevel ) override = 0;
    virtual size_t getExtraNumPositions( TWindowLevel whichLevel ) const override;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) override = 0;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) override = 0;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) override = 0;
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const override = 0;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const override = 0;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const override = 0;

    virtual bool initFromBegin() const = 0;

    RecordList *getRecordList( TObjectOrder whichObject ) override;
    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override = 0;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) override = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) override = 0;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const override = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const override = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const override = 0;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder,
                                        bool includeExtraCompose = false ) = 0;

    virtual bool isDerivedWindow() const override = 0;

    TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU ) override;
    TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread )override;

    TObjectOrder getWindowLevelObjects() const override;

    TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;
    TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const override;
    TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) const override;
    TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime ) const override;

    virtual KTimeline *clone( bool recursiveClone = false ) override;
    void getGroupLabels(  PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const override;
    bool getParametersOfFunction( std::string whichFunction,
                                   PRV_UINT32 &numParameters,
                                   std::vector<std::string> &nameParameters,
                                   std::vector< std::vector < double > > &defaultValues  ) const override;

  protected:
    KTrace *myTrace;
    TTraceLevel level;
    TTimeUnit timeUnit;

    std::vector<IntervalCompose> intervalTopCompose1;
    std::vector<IntervalCompose> intervalTopCompose2;

    std::map< TWindowLevel, std::vector< std::vector< IntervalCompose * > > >  extraCompose;
    std::map< TWindowLevel, std::vector< SemanticFunction * > >                extraComposeFunctions;

    SemanticInfoType getTopComposeSemanticInfoType() const;
    virtual void initSemanticFunctions() = 0;
  private:

};


class KSingleWindow: public KTimeline
{
  public:
    KSingleWindow()
    {
      timeUnit = NS;
    }

    KSingleWindow( Trace *whichTrace );

    virtual ~KSingleWindow();

    MemoryTrace::iterator *getEndRecord()
    {
      return myTrace->end();
    }

    MemoryTrace::iterator *getBeginRecord()
    {
      return myTrace->begin();
    }

    MemoryTrace::iterator *getThreadRecordByTime( TThreadOrder whichOrder )
    {
      return recordsByTimeThread[whichOrder];
    }

    MemoryTrace::iterator *getThreadEndRecord( TThreadOrder whichOrder )
    {
      return myTrace->threadEnd( whichOrder );
    }

    MemoryTrace::iterator *getThreadBeginRecord( TThreadOrder whichOrder )
    {
      return myTrace->threadBegin( whichOrder );
    }

    MemoryTrace::iterator *getCPUEndRecord( TCPUOrder whichOrder )
    {
      return myTrace->CPUEnd( whichOrder );
    }

    MemoryTrace::iterator *getCPUBeginRecord( TCPUOrder whichOrder )
    {
      return myTrace->CPUBegin( whichOrder );
    }

    MemoryTrace::iterator *getCPURecordByTime( TCPUOrder whichOrder )
    {
      return recordsByTimeCPU[whichOrder];
    }

    bool passFilter( MemoryTrace::iterator *it )
    {
      return myFilter->passFilter( it );
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) override;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) const override;
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel ) override;
    virtual std::string getFirstUsefulFunction( ) override;
    virtual TWindowLevel getFirstFreeCompose() const override;
    virtual SemanticFunction *getFirstSemUsefulFunction() override;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) override;
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const override;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const override;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const override;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) override;
    virtual void removeExtraCompose( TWindowLevel whichLevel ) override;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) override;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) override;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) override;
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const override;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const override;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const override;

    virtual bool initFromBegin() const override;

    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) override;
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) override;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const override;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const override;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const override;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder,
                                        bool includeExtraCompose = false ) override;

    virtual bool isDerivedWindow() const override
    {
      return false;
    }

    virtual Filter *getFilter() const override
    {
      return (Filter *)myFilter;
    }

    SemanticInfoType getSemanticInfoType() const override;

    virtual KTimeline *clone( bool recursiveClone = false ) override;

  protected:
    std::vector<MemoryTrace::iterator *> recordsByTimeThread;
    std::vector<MemoryTrace::iterator *> recordsByTimeCPU;

    // Semantic interval structure
    std::vector<IntervalCompose>   intervalComposeWorkload;
    std::vector<IntervalNotThread> intervalWorkload;

    std::vector<IntervalCompose>   intervalComposeApplication;
    std::vector<IntervalNotThread> intervalApplication;

    std::vector<IntervalCompose>   intervalComposeTask;
    std::vector<IntervalNotThread> intervalTask;

    std::vector<IntervalCompose>   intervalComposeThread;
    std::vector<IntervalThread>    intervalThread;

    std::vector<IntervalCompose>   intervalComposeSystem;
    std::vector<IntervalNotThread> intervalSystem;

    std::vector<IntervalCompose>   intervalComposeNode;
    std::vector<IntervalNotThread> intervalNode;

    std::vector<IntervalCompose>   intervalComposeCPU;
    std::vector<IntervalCPU>       intervalCPU;

  private:
    virtual void initSemanticFunctions() override;

    SemanticFunction *functions[ COMPOSECPU + 1 ];
    KFilter *myFilter;
};


class KDerivedWindow: public KTimeline
{
  public:
    KDerivedWindow()
    {
      timeUnit = NS;

      factor.clear();
      factor.push_back( 1.0 );
      factor.push_back( 1.0 );

      shift.clear();
      shift.push_back( 0 );
      shift.push_back( 0 );

      initSemanticFunctions();

      parents.push_back( nullptr );
      parents.push_back( nullptr );
    }

    KDerivedWindow( Timeline *window1, Timeline *window2 )
    {
      timeUnit = NS;

      factor.clear();
      factor.push_back( 1.0 );
      factor.push_back( 1.0 );

      shift.clear();
      shift.push_back( 0 );
      shift.push_back( 0 );

      initSemanticFunctions();

      parents.push_back( (KTimeline *)window1 );
      parents.push_back( (KTimeline *)window2 );
      setup( nullptr );
    }

    virtual ~KDerivedWindow();

    virtual void setParent( PRV_UINT16 whichParent, Timeline *whichWindow ) override;
    virtual Timeline *getParent( PRV_UINT16 whichParent ) const override;

    virtual void setLevel( TTraceLevel whichLevel ) override;
    virtual TTraceLevel getMinAcceptableLevel() const override;

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) override;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) const override;
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel ) override;
    virtual std::string getFirstUsefulFunction( ) override;
    virtual TWindowLevel getFirstFreeCompose() const override;
    virtual SemanticFunction *getFirstSemUsefulFunction() override;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) override;
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const override;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const override;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                              TParamIndex whichParam ) const override;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) override;
    virtual void removeExtraCompose( TWindowLevel whichLevel ) override;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) override;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) override;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) override;
    virtual TParamIndex getExtraFunctionNumParam( TWindowLevel whichLevel, size_t whichPosition ) const override;
    virtual TParamValue getExtraFunctionParam( TWindowLevel whichLevel,
                                               size_t whichPosition,
                                               TParamIndex whichParam ) const override;
    virtual std::string getExtraFunctionParamName( TWindowLevel whichLevel,
                                                   size_t whichPosition,
                                                   TParamIndex whichParam ) const override;

    virtual bool initFromBegin() const override;

    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) override;
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) override;
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) override;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const override;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const override;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const override;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder,
                                        bool includeExtraCompose = false ) override;

    virtual bool isDerivedWindow() const override
    {
      return true;
    }

    void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue ) override
    {
      factor[ whichFactor ] = newValue;
    }

    TSemanticValue getFactor( PRV_UINT16 whichFactor ) const override
    {
      return factor[ whichFactor ];
    }

    void setShift( PRV_UINT16 whichShift, TSemanticValue newValue ) override
    {
      shift[ whichShift ] = newValue;
    }

    PRV_INT16 getShift( PRV_UINT16 whichShift ) const override
    {
      return shift[ whichShift ];
    }

    SemanticInfoType getSemanticInfoType() const override;

    virtual KTimeline *clone( bool recursiveClone = false ) override;

  protected:
    std::vector<KTimeline *> parents;
    std::vector<TSemanticValue> factor;
    std::vector<PRV_INT16> shift;

    // Semantic interval structure
    std::vector<IntervalCompose>        intervalComposeWorkload;
    std::vector<IntervalNotThread>      intervalWorkload;

    std::vector<IntervalCompose>        intervalComposeApplication;
    std::vector<IntervalNotThread>      intervalApplication;

    std::vector<IntervalCompose>        intervalComposeTask;
    std::vector<IntervalNotThread>      intervalTask;

    std::vector<IntervalCompose>        intervalComposeThread;

    std::vector<IntervalCompose>        intervalComposeSystem;
    std::vector<IntervalNotThread>      intervalSystem;

    std::vector<IntervalCompose>        intervalComposeNode;
    std::vector<IntervalNotThread>      intervalNode;

    std::vector<IntervalCompose>        intervalComposeCPU;

    std::vector<IntervalDerived>        intervalDerived;
    std::vector<IntervalControlDerived> intervalControlDerived;

    SemanticFunction *functions[ DERIVED + 1 ];
//    SemanticFunction *functions[ 3 ];
  private:
    void setup( KTrace * whichTrace );

    virtual void initSemanticFunctions() override;
};


