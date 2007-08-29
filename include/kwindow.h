#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

#include <vector>
#include "kwindowexception.h"
#include "trace.h"
#include "intervalnotthread.h"
#include "intervalthread.h"
#include "intervalcpu.h"
#include "intervalcompose.h"
#include "intervalderived.h"
#include "intervalcontrolderived.h"
#include "semanticthread.h"
#include "semanticcompose.h"

class IntervalNotThread;
class IntervalThread;
class IntervalCPU;
class IntervalCompose;
class IntervalDerived;
class IntervalControlDerived;

class KWindow
{
  public:
    KWindow()
    {}
    KWindow( Trace *whichTrace ): myTrace( whichTrace )
    {}
    virtual ~KWindow()
    {}

    Trace *getTrace() const
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

    virtual void setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction ) = 0;
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel ) = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;
    virtual bool initFromBegin() const = 0;

    virtual RecordList *init( TRecordTime initialTime, TCreateList create ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject ) = 0;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;

    virtual Interval *getLevelInterval( TWindowLevel whichLevel,
                                        TObjectOrder whichOrder ) = 0;

    virtual bool isDerivedWindow() const = 0;
  protected:
    Trace *myTrace;
    TWindowLevel level;

    vector<IntervalCompose> intervalTopCompose1;
    vector<IntervalCompose> intervalTopCompose2;

  private:

};


class KSingleWindow: public KWindow
{
  public:
    KSingleWindow()
    {}

    KSingleWindow( Trace *whichTrace );

    virtual ~KSingleWindow()
    {
      for ( TObjectOrder i = 0; i < recordsByTime.size(); i++ )
      {
        if ( recordsByTime[ i ] != NULL )
          delete recordsByTime[ i ];
      }
      recordsByTime.clear();
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
      return true;
    }

    virtual void setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction );
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual bool initFromBegin() const;

    virtual RecordList *init( TRecordTime initialTime, TCreateList create );
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
    SemanticFunction *functions[ COMPOSECPU ];
};


class KDerivedWindow: public KWindow
{
  public:
    KDerivedWindow()
    {}

    KDerivedWindow( KWindow *window1, KWindow *window2 );

    virtual ~KDerivedWindow()
    {}

    void setParent( UINT8 whichParent, KWindow *whichWindow )
    {
      parents[ whichParent ] = whichWindow;
    }

    KWindow *getParent( UINT8 whichParent ) const
    {
      return parents[whichParent];
    }

    virtual TWindowLevel getLevel() const
    {
      TWindowLevel tmp = NONE;

      for( UINT8 i = 0; i < parents.size(); i++ )
      {
        if( parents[ i ]->getLevel() > tmp )
          tmp = parents[ i ]->getLevel();
      }

      return tmp;
    }

    virtual void setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction );
    virtual SemanticFunction *getLevelFunction( TWindowLevel whichLevel );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );
    virtual bool initFromBegin() const;

    virtual RecordList *init( TRecordTime initialTime, TCreateList create );
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

    void setFactor( UINT8 whichFactor, TSemanticValue newValue )
    {
      factor[ whichFactor ] = newValue;
    }

    TSemanticValue getFactor( UINT8 whichFactor )
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

};

#endif // KWINDOW_H_INCLUDED
