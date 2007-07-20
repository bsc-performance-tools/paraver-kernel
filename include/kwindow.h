#ifndef KWINDOW_H_INCLUDED
#define KWINDOW_H_INCLUDED

#include <vector>
#include "trace.h"

class KWindow
{
  public:
    KWindow();
    ~KWindow();
  protected:

  private:

};


class KSingleWindow: public KWindow
{
  public:
    KSingleWindow();
    ~KSingleWindow();

    Trace& getTrace() const
    {
      return myTrace;
    }

  protected:
    Trace& myTrace;

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
