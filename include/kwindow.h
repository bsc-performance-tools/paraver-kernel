#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

#include <vector>
#include "kwindowexception.h"
#include "trace.h"
#include "intervalthread.h"
#include "intervalcompose.h"
#include "semanticthread.h"
#include "semanticcompose.h"

class IntervalThread;
class IntervalCompose;

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

    virtual void setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction ) = 0;
    //virtual void setComposeFunction( composelevel?, SemanticFunction *function ) = 0;
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue ) = 0;

    virtual RecordList *init( TRecordTime initialTime, TCreateList create ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject ) = 0;

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;

    virtual vector<IntervalCompose> *getLevelInterval( TWindowLevel whichLevel ) = 0;
  protected:
    Trace *myTrace;

    vector<IntervalCompose> intervalTopCompose1;
    vector<IntervalCompose> intervalTopCompose2;

    SemanticCompose *topCompose1;
    SemanticCompose *topCompose2;

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

    TWindowLevel getLevel() const
    {
      return level;
    }

    void setLevel( TWindowLevel whichLevel )
    {
      if( whichLevel >= TOPCOMPOSE1 )
        throw KWindowException( KWindowException::invalidLevel );
      level = whichLevel;
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

    bool passFilter( MemoryTrace::iterator *it )
    {
      return true;
    }

    virtual void setLevelFunction( TWindowLevel whichLevel,
                                   SemanticFunction *whichFunction );
    virtual void setFunctionParam( TWindowLevel whichLevel,
                                   TParamIndex whichParam,
                                   const TParamValue& newValue );

    virtual RecordList *init( TRecordTime initialTime, TCreateList create );
    virtual RecordList *calcNext( TObjectOrder whichObject );
    virtual RecordList *calcPrev( TObjectOrder whichObject );

    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;

    virtual vector<IntervalCompose> *getLevelInterval( TWindowLevel whichLevel );

  protected:
    vector<MemoryTrace::iterator *> recordsByTime;
    TWindowLevel level;

    // Semantic interval structure
    vector<IntervalCompose> intervalComposeThread;
    vector<IntervalThread> intervalThread;

  private:
    SemanticCompose *composeThread;
    SemanticThread *functionThread;
};


class KDerivedWindow: public KWindow
{
  public:
    KDerivedWindow()
    {}
    virtual ~KDerivedWindow()
    {}

    KWindow *getParent( UINT8 whichParent ) const
    {
      return parents[whichParent];
    }

    virtual RecordList *init( TRecordTime initialTime, TCreateList create );

    virtual vector<IntervalCompose> *getLevelInterval( TWindowLevel whichLevel );

  protected:
    vector<KWindow *> parents;

  private:

};

#endif // KWINDOW_H_INCLUDED
