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

#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

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

class KWindow: public Window
{
  public:
    KWindow()
    {}
    KWindow( Trace *whichTrace ): myTrace( (KTrace*)whichTrace )
    {
      timeUnit = NS;
      level = THREAD;
    }
    virtual ~KWindow()
    {}

    virtual Filter *getFilter() const
    {
      return NULL;
    }

    Trace *getTrace() const
    {
      return myTrace;
    }

    virtual TWindowLevel getLevel() const
    {
      return level;
    }

    virtual void setLevel( TWindowLevel whichLevel )
    {
      if ( whichLevel >= TOPCOMPOSE1 )
        throw KWindowException( KWindowException::invalidLevel );
      level = whichLevel;
    }

    virtual TWindowLevel getMinAcceptableLevel() const
    {
      TWindowLevel minAcceptableLevel = THREAD;

      if ( level >= SYSTEM && level <= CPU )
        minAcceptableLevel = CPU;

      return minAcceptableLevel;
    }

    void setTimeUnit( TTimeUnit whichUnit )
    {
      timeUnit = whichUnit;
    }

    TTimeUnit getTimeUnit() const
    {
      return timeUnit;
    }

    TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const
    {
      if ( whichLevel == WORKLOAD )
        return COMPOSEWORKLOAD;
      else if ( whichLevel == APPLICATION )
        return COMPOSEAPPLICATION;
      else if ( whichLevel == TASK )
        return COMPOSETASK;
      else if ( whichLevel == THREAD )
        return COMPOSETHREAD;
      else if ( whichLevel == SYSTEM )
        return COMPOSESYSTEM;
      else if ( whichLevel == NODE )
        return COMPOSENODE;
      else if ( whichLevel == CPU )
        return COMPOSECPU;

      return NONE;
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction ) = 0;
    virtual std::string getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel ) = 0;
    virtual std::string getFirstUsefulFunction( ) = 0;
    virtual TWindowLevel getFirstFreeCompose() const = 0;
    virtual SemanticFunction *getFirstSemUsefulFunction() = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel ) = 0;
    virtual void removeExtraCompose( TWindowLevel whichLevel ) = 0;
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction ) = 0;
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition ) = 0;
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue ) = 0;

    virtual bool initFromBegin() const = 0;

    RecordList *getRecordList( TObjectOrder whichObject );
    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true ) = 0;
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true ) = 0;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder ) = 0;

    virtual bool isDerivedWindow() const = 0;

    TObjectOrder cpuObjectToWindowObject( TCPUOrder whichCPU );
    TObjectOrder threadObjectToWindowObject( TThreadOrder whichThread );

    TObjectOrder getWindowLevelObjects() const;

    TRecordTime customUnitsToTraceUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const;
    TRecordTime traceUnitsToCustomUnits( TRecordTime whichTime, TTimeUnit whichUnits ) const;
    TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime ) const;
    TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime ) const;

    virtual KWindow *clone( bool recursiveClone = false );
    void getGroupLabels(  PRV_UINT32 whichGroup, std::vector<std::string>& onVector ) const;
    bool getParametersOfFunction( std::string whichFunction,
                                   PRV_UINT32 &numParameters,
                                   std::vector<std::string> &nameParameters,
                                   std::vector< std::vector < double > > &defaultValues  ) const;

  protected:
    KTrace *myTrace;
    TWindowLevel level;
    TTimeUnit timeUnit;

    std::vector<IntervalCompose> intervalTopCompose1;
    std::vector<IntervalCompose> intervalTopCompose2;

    std::map< TWindowLevel, std::vector< IntervalCompose * > >  extraCompose;
    std::map< TWindowLevel, std::vector< SemanticFunction * > > extraComposeFunctions;

    SemanticInfoType getTopComposeSemanticInfoType() const;
    virtual void initSemanticFunctions() = 0;
  private:

};


class KSingleWindow: public KWindow
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
                                   const std::string& whichFunction );
    virtual std::string getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel );
    virtual std::string getFirstUsefulFunction( );
    virtual TWindowLevel getFirstFreeCompose() const;
    virtual SemanticFunction *getFirstSemUsefulFunction();
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel );
    virtual void removeExtraCompose( TWindowLevel whichLevel );
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction );
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition );
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue );

    virtual bool initFromBegin() const;

    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true );
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true );

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder );

    virtual bool isDerivedWindow() const
    {
      return false;
    }

    virtual Filter *getFilter() const
    {
      return (Filter *)myFilter;
    }

    SemanticInfoType getSemanticInfoType() const;

    virtual KWindow *clone( bool recursiveClone = false );

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
    virtual void initSemanticFunctions();

    SemanticFunction *functions[ COMPOSECPU + 1 ];
    KFilter *myFilter;
};


class KDerivedWindow: public KWindow
{
  public:
    KDerivedWindow()
    {
      timeUnit = NS;

      factor.clear();
      factor.push_back( 1.0 );
      factor.push_back( 1.0 );

      initSemanticFunctions();

      parents.push_back( NULL );
      parents.push_back( NULL );
    }

    KDerivedWindow( Window *window1, Window *window2 )
    {
      timeUnit = NS;

      factor.clear();
      factor.push_back( 1.0 );
      factor.push_back( 1.0 );

      initSemanticFunctions();

      parents.push_back( (KWindow*)window1 );
      parents.push_back( (KWindow*)window2 );
      setup( NULL );
    }

    virtual ~KDerivedWindow();

    virtual void setParent( PRV_UINT16 whichParent, Window *whichWindow );
    virtual Window *getParent( PRV_UINT16 whichParent ) const;

    virtual void setLevel( TWindowLevel whichLevel );
    virtual TWindowLevel getMinAcceptableLevel() const;

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const std::string& whichFunction );
    virtual std::string getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel );
    virtual std::string getFirstUsefulFunction( );
    virtual TWindowLevel getFirstFreeCompose() const;
    virtual SemanticFunction *getFirstSemUsefulFunction();
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual std::string getFunctionParamName( TWindowLevel whichLevel,
                                              TParamIndex whichParam ) const;

    // Extra composes
    virtual void addExtraCompose( TWindowLevel whichLevel );
    virtual void removeExtraCompose( TWindowLevel whichLevel );
    virtual bool setExtraLevelFunction( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        const std::string& whichFunction );
    virtual std::string getExtraLevelFunction( TWindowLevel whichLevel,
                                               size_t whichPosition );
    virtual void setExtraFunctionParam( TWindowLevel whichLevel,
                                        size_t whichPosition,
                                        TParamIndex whichParam,
                                        const TParamValue& newValue );

    virtual bool initFromBegin() const;

    virtual void init( TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual void initRow( TObjectOrder whichRow, TRecordTime initialTime, TCreateList create, bool updateLimits = true );
    virtual RecordList *calcNext( TObjectOrder whichObject, bool updateLimits = true );
    virtual RecordList *calcPrev( TObjectOrder whichObject, bool updateLimits = true );

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder );

    virtual bool isDerivedWindow() const
    {
      return true;
    }

    void setFactor( PRV_UINT16 whichFactor, TSemanticValue newValue )
    {
      factor[ whichFactor ] = newValue;
    }

    TSemanticValue getFactor( PRV_UINT16 whichFactor ) const
    {
      return factor[ whichFactor ];
    }

    SemanticInfoType getSemanticInfoType() const;

    virtual KWindow *clone( bool recursiveClone = false );

  protected:
    std::vector<KWindow *> parents;
    std::vector<TSemanticValue> factor;

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

    virtual void initSemanticFunctions();
};

#endif // KWINDOW_H_INCLUDED
