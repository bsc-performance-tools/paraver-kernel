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
#include "filter.h"
#include "window.h"

using namespace std;

class KWindow:public Window
{
  public:
    KWindow()
    {}
    KWindow( KTrace *whichTrace ): myTrace( whichTrace )
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

    virtual void setFactor( UINT16 whichFactor, TSemanticValue newValue )
    {}

    virtual void setParent( UINT16 whichParent, Window *whichWindow )
    {}

    KTrace *getTrace() const
    {
      return myTrace;
    }

    virtual TWindowLevel getLevel() const
    {
      return level;
    }

    void setLevel( TWindowLevel whichLevel )
    {
      if ( whichLevel >= TOPCOMPOSE1 )
        throw KWindowException( KWindowException::invalidLevel );
      level = whichLevel;
    }

    void setTimeUnit( TTimeUnit whichUnit )
    {
      timeUnit = whichUnit;
    }

    TTimeUnit getTimeUnit()
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
                                   SemanticFunction *whichFunction ) = 0;
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual SemanticFunction *getFirstUsefulFunction( ) = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;

    virtual bool initFromBegin() const = 0;

    RecordList *getRecordList( TObjectOrder whichObject );
    virtual void init( TRecordTime initialTime, TCreateList create ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject ) = 0;

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
  protected:
    KTrace *myTrace;
    TWindowLevel level;
    TTimeUnit timeUnit;

    vector<IntervalCompose> intervalTopCompose1;
    vector<IntervalCompose> intervalTopCompose2;

  private:

};


class KSingleWindow: public KWindow
{
  public:
    KSingleWindow()
    {
      timeUnit = NS;
    }

    KSingleWindow( KTrace *whichTrace );

    virtual ~KSingleWindow();

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
                                   SemanticFunction *whichFunction );
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getFirstUsefulFunction( );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );

    virtual bool initFromBegin() const;

    virtual void init( TRecordTime initialTime, TCreateList create );
    virtual RecordList *calcNext( TObjectOrder whichObject );
    virtual RecordList *calcPrev( TObjectOrder whichObject );

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
      return myFilter;
    }

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
    SemanticFunction *functions[ COMPOSECPU + 1 ];
    Filter *myFilter;
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

      functions[ 0 ] = NULL;
      functions[ 1 ] = NULL;
      functions[ 2 ] = NULL;

      parents.push_back( NULL );
      parents.push_back( NULL );
    }

    KDerivedWindow( KWindow *window1, KWindow *window2 )
    {
      timeUnit = NS;

      factor.clear();
      factor.push_back( 1.0 );
      factor.push_back( 1.0 );

      functions[ 0 ] = NULL;
      functions[ 1 ] = NULL;
      functions[ 2 ] = NULL;

      parents.push_back( window1 );
      parents.push_back( window2 );
      setup();
    }

    virtual ~KDerivedWindow()
    {
      if ( functions[ 0 ] != NULL )
        delete functions[ 0 ];
      if ( functions[ 1 ] != NULL )
        delete functions[ 1 ];
      if ( functions[ 2 ] != NULL )
        delete functions[ 2 ];
    }

    void setParent( UINT16 whichParent, KWindow *whichWindow )
    {
      parents[ whichParent ] = whichWindow;
      if ( parents[ 0 ] != NULL && parents[ 1 ] != NULL )
        setup();
    }

    KWindow *getParent( UINT16 whichParent ) const
    {
      return parents[whichParent];
    }

    virtual TWindowLevel getLevel() const
    {
      TWindowLevel tmp = NONE;

      for ( UINT16 i = 0; i < parents.size(); i++ )
      {
        if ( parents[ i ]->getLevel() > tmp )
          tmp = parents[ i ]->getLevel();
      }

      return tmp;
    }

    virtual bool setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction );
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel );
    virtual SemanticFunction *getFirstUsefulFunction( );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );

    virtual bool initFromBegin() const;

    virtual void init( TRecordTime initialTime, TCreateList create );
    virtual RecordList *calcNext( TObjectOrder whichObject );
    virtual RecordList *calcPrev( TObjectOrder whichObject );

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

    TSemanticValue getFactor( UINT16 whichFactor )
    {
      return factor[ whichFactor ];
    }

  protected:
    vector<KWindow *> parents;
    vector<TSemanticValue> factor;

    vector<IntervalDerived> intervalDerived;
    vector<IntervalControlDerived> intervalControlDerived;

    SemanticFunction *functions[ 3 ];

  private:
    void setup();
};

#endif // KWINDOW_H_INCLUDED
