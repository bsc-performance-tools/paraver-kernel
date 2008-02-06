#ifndef INTERVALCOMPOSE_H_INCLUDED
#define INTERVALCOMPOSE_H_INCLUDED

#include "intervalhigh.h"
#include "semanticcompose.h"

class KWindow;

class IntervalCompose: public IntervalHigh
{
  public:
    IntervalCompose()
    {
      function = NULL;
      endRecord = NULL;
      beginRecord = NULL;
    }

    IntervalCompose( KWindow *whichWindow, TWindowLevel whichLevel,
                     TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      function = NULL;
      endRecord = NULL;
      beginRecord = NULL;
    }

    virtual ~IntervalCompose()
    {
      if( endRecord != NULL )
        delete endRecord;
      if( beginRecord != NULL )
        delete beginRecord;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL );
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false );
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return window;
    }

  protected:
    KWindow *window;
    SemanticCompose *function;
    TCreateList createList;

    virtual void setChilds()
    {
      if ( level == TOPCOMPOSE1 )
      {
        if ( lastLevel != TOPCOMPOSE2 )
        {
          lastLevel = TOPCOMPOSE2;
          childIntervals.push_back( getWindowInterval( TOPCOMPOSE2, order ) );
        }
      }
      else if ( level == TOPCOMPOSE2 )
      {
        if ( lastLevel != getWindowLevel() )
        {
          childIntervals.clear();
          lastLevel = getWindowLevel();
          if ( IsDerivedWindow() )
          {
            childIntervals.push_back( getWindowInterval( DERIVED, order ) );
          }
          else
          {
            childIntervals.push_back( getWindowInterval(
                                        getComposeLevel( getWindowLevel() ), order ) );
          }
        }
      }
      else if ( level == COMPOSEWORKLOAD )
      {
        if ( lastLevel != WORKLOAD )
        {
          lastLevel = WORKLOAD;
          childIntervals.push_back( getWindowInterval( WORKLOAD, order ) );
        }
      }
      else if ( level == COMPOSEAPPLICATION )
      {
        if ( lastLevel != APPLICATION )
        {
          lastLevel = APPLICATION;
          childIntervals.push_back( getWindowInterval( APPLICATION, order ) );
        }
      }
      else if ( level == COMPOSETASK )
      {
        if ( lastLevel != TASK )
        {
          lastLevel = TASK;
          childIntervals.push_back( getWindowInterval( TASK, order ) );
        }
      }
      else if ( level == COMPOSETHREAD )
      {
        if ( lastLevel != THREAD )
        {
          lastLevel = THREAD;
          childIntervals.push_back( getWindowInterval( THREAD, order ) );
        }
      }
      else if ( level == COMPOSESYSTEM )
      {
        if ( lastLevel != SYSTEM )
        {
          lastLevel = SYSTEM;
          childIntervals.push_back( getWindowInterval( SYSTEM, order ) );
        }
      }
      else if ( level == COMPOSENODE )
      {
        if ( lastLevel != NODE )
        {
          lastLevel = NODE;
          childIntervals.push_back( getWindowInterval( NODE, order ) );
        }
      }
      else if ( level == COMPOSECPU )
      {
        if ( lastLevel != CPU )
        {
          lastLevel = CPU;
          childIntervals.push_back( getWindowInterval( CPU, order ) );
        }
      }
    }

    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;
    virtual Trace *getWindowTrace() const;

  private:
    bool joinBursts;
    MemoryTrace::iterator *endRecord;
    MemoryTrace::iterator *beginRecord;
};


#endif // INTERVALCOMPOSE_H_INCLUDED
