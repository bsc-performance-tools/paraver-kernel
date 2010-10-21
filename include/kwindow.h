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

using namespace std;

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
      return THREAD;
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

    MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it )
    {
      return myTrace->copyIterator( it );
    }
    MemoryTrace::iterator *copyThreadIterator( MemoryTrace::iterator *it )
    {
      return myTrace->copyThreadIterator( it );
    }
    MemoryTrace::iterator *copyCPUIterator( MemoryTrace::iterator *it )
    {
      return myTrace->copyCPUIterator( it );
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const string& whichFunction ) = 0;
    virtual string getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel ) = 0;
    virtual string getFirstUsefulFunction( ) = 0;
    virtual TWindowLevel getFirstFreeCompose() const = 0;
    virtual SemanticFunction *getFirstSemUsefulFunction() = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
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

    TObjectOrder getWindowLevelObjects();

    TRecordTime traceUnitsToWindowUnits( TRecordTime whichTime );
    TRecordTime windowUnitsToTraceUnits( TRecordTime whichTime );

    virtual KWindow *clone();
    void getGroupLabels(  UINT32 whichGroup, vector<string>& onVector ) const;
    bool getParametersOfFunction( string whichFunction,
                                   UINT32 &numParameters,
                                   vector<string> &nameParameters,
                                   vector< vector < double > > &defaultValues  ) const;

  protected:
    KTrace *myTrace;
    TWindowLevel level;
    TTimeUnit timeUnit;

    vector<IntervalCompose> intervalTopCompose1;
    vector<IntervalCompose> intervalTopCompose2;

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
      return recordsByTime[whichOrder];
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
      return recordsByTime[whichOrder];
    }

    bool passFilter( MemoryTrace::iterator *it )
    {
      return myFilter->passFilter( it );
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const string& whichFunction );
    virtual string getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel );
    virtual string getFirstUsefulFunction( );
    virtual TWindowLevel getFirstFreeCompose() const;
    virtual SemanticFunction *getFirstSemUsefulFunction();
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const;

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

    virtual KWindow *clone();

  protected:
    vector<MemoryTrace::iterator *> recordsByTime;

    // Semantic interval structure
    vector<IntervalCompose> intervalComposeWorkload;
    vector<IntervalNotThread> intervalWorkload;
    vector<IntervalCompose> intervalComposeApplication;
    vector<IntervalNotThread> intervalApplication;
    vector<IntervalCompose> intervalComposeTask;
    vector<IntervalNotThread> intervalTask;
    vector<IntervalCompose> intervalComposeThread;
    vector<IntervalThread> intervalThread;

    vector<IntervalCompose> intervalComposeSystem;
    vector<IntervalNotThread> intervalSystem;
    vector<IntervalCompose> intervalComposeNode;
    vector<IntervalNotThread> intervalNode;
    vector<IntervalCompose> intervalComposeCPU;
    vector<IntervalCPU> intervalCPU;

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

    virtual void setParent( UINT16 whichParent, Window *whichWindow );
    virtual Window *getParent( UINT16 whichParent ) const;

    virtual void setLevel( TWindowLevel whichLevel );
    virtual TWindowLevel getMinAcceptableLevel() const;

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   const string& whichFunction );
    virtual string getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getSemanticFunction( TWindowLevel whichLevel );
    virtual string getFirstUsefulFunction( );
    virtual TWindowLevel getFirstFreeCompose() const;
    virtual SemanticFunction *getFirstSemUsefulFunction();
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual TParamIndex getFunctionNumParam( TWindowLevel whichLevel ) const;
    virtual TParamValue getFunctionParam( TWindowLevel whichLevel,
                                          TParamIndex whichParam ) const;
    virtual string getFunctionParamName( TWindowLevel whichLevel,
                                         TParamIndex whichParam ) const;

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

    void setFactor( UINT16 whichFactor, TSemanticValue newValue )
    {
      factor[ whichFactor ] = newValue;
    }

    TSemanticValue getFactor( UINT16 whichFactor ) const
    {
      return factor[ whichFactor ];
    }

    SemanticInfoType getSemanticInfoType() const;

    virtual KWindow *clone();

  protected:
    vector<KWindow *> parents;
    vector<TSemanticValue> factor;

    // Semantic interval structure
    vector<IntervalCompose> intervalComposeWorkload;
    vector<IntervalNotThread> intervalWorkload;
    vector<IntervalCompose> intervalComposeApplication;
    vector<IntervalNotThread> intervalApplication;
    vector<IntervalCompose> intervalComposeTask;
    vector<IntervalNotThread> intervalTask;
    vector<IntervalCompose> intervalComposeThread;

    vector<IntervalCompose> intervalComposeSystem;
    vector<IntervalNotThread> intervalSystem;
    vector<IntervalCompose> intervalComposeNode;
    vector<IntervalNotThread> intervalNode;
    vector<IntervalCompose> intervalComposeCPU;

    vector<IntervalDerived> intervalDerived;
    vector<IntervalControlDerived> intervalControlDerived;

    SemanticFunction *functions[ DERIVED + 1 ];
//    SemanticFunction *functions[ 3 ];
  private:
    void setup( KTrace * whichTrace );

    virtual void initSemanticFunctions();
};

#endif // KWINDOW_H_INCLUDED
