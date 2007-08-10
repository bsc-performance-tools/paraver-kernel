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
      return myTrace.copyIterator( it );
    }

    MemoryTrace::iterator *getThreadRecordByTime( TThreadOrder whichOrder )
    {
      return recordsByTime[whichOrder];
    }

    bool passFilter( MemoryTrace::iterator *it )
    {
      return true;
    }

  protected:
    vector<MemoryTrace::iterator *> recordsByTime;
    TWindowLevel level;
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
