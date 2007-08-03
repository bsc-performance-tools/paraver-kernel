#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

#include <vector>
#include "trace.h"

class KWindow
{
  public:
    KWindow();
    ~KWindow();

    Trace& getTrace() const
    {
      return myTrace;
    }

  protected:
    Trace& myTrace;

  private:

};


class KSingleWindow: public KWindow
{
  public:
    KSingleWindow();
    ~KSingleWindow();

    MemoryTrace::iterator *copyIterator( MemoryTrace::iterator *it )
    {
      return myTrace.copyIterator( it );
    }

    MemoryTrace::iterator *getThreadRecordByTime( TThreadOrder whichOrder )
    {
      return dynamic_cast<MemoryTrace::iterator *> ( recordsByTime[whichOrder] );
    }

    bool passFilter( MemoryTrace::iterator *it )
    {
      return true;
    }

  protected:
    vector<MemoryTrace::iterator *> recordsByTime;
  private:

};


class KDerivedWindow: public KWindow
{
  public:
    KDerivedWindow();
    ~KDerivedWindow();

    KWindow *getParent( UINT8 whichParent ) const
    {
      return parents[whichParent];
    }

  protected:
    vector<KWindow*> parents;

  private:

};

#endif // KWINDOW_H_INCLUDED
