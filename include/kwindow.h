#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

#include <vector>
#include "trace.h"
#include "intervalthread.h"
#include "semanticfunction.h"

class IntervalThread;

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

    virtual RecordList *init( TRecordTime initialTime, TCreateList create ) = 0;
    virtual RecordList *calcNext( TObjectOrder whichObject ) = 0;
    virtual RecordList *calcPrev( TObjectOrder whichObject ) = 0;
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const = 0;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const = 0;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const = 0;

  protected:
    Trace *myTrace;

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

    virtual RecordList *init( TRecordTime initialTime, TCreateList create );
    virtual RecordList *calcNext( TObjectOrder whichObject );
    virtual RecordList *calcPrev( TObjectOrder whichObject );
    virtual TRecordTime getBeginTime( TObjectOrder whichObject ) const;
    virtual TRecordTime getEndTime( TObjectOrder whichObject ) const;
    virtual TSemanticValue getValue( TObjectOrder whichObject ) const;

  protected:
    vector<MemoryTrace::iterator *> recordsByTime;
    TWindowLevel level;

    // Semantic interval structure
    vector<IntervalThread> intervalThread;
  private:

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

  protected:
    vector<KWindow *> parents;

  private:

};

#endif // KWINDOW_H_INCLUDED
