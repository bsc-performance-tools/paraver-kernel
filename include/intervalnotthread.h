#ifndef INTERVALNOTTHREAD_H_INCLUDED
#define INTERVALNOTTHREAD_H_INCLUDED

#include "intervalhigh.h"
#include "semanticnotthread.h"

class KSingleWindow;
class SemanticNotThread;

class IntervalNotThread: public IntervalHigh
{
  public:
    IntervalNotThread()
    {
      function = NULL;
    }

    IntervalNotThread( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                       TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      function = NULL;
    }

    virtual ~IntervalNotThread()
    {
      if ( begin != NULL )
        delete begin;
      if ( end != NULL )
        delete end;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL );
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false );
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return ( KWindow * ) window;
    }

  protected:
    KSingleWindow *window;
    SemanticNotThread *function;
    TCreateList createList;

    virtual void setChilds()
    {
      if ( level == WORKLOAD )
      {
        if ( lastLevel != COMPOSEAPPLICATION )
        {
          lastLevel = COMPOSEAPPLICATION;
          for ( TApplOrder i = 0; i < getWindowTrace()->totalApplications(); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSEAPPLICATION, i ) );
          }
        }
      }
      else if ( level == APPLICATION )
      {
        if ( lastLevel != COMPOSETASK )
        {
          lastLevel = COMPOSETASK;
          for ( TTaskOrder i = getWindowTrace()->getFirstTask( order );
                i <= getWindowTrace()->getLastTask( order ); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSETASK, i ) );
          }
        }
      }
      else if ( level == TASK )
      {
        if ( lastLevel != COMPOSETHREAD )
        {
          lastLevel = COMPOSETHREAD;
          TApplOrder myAppl;
          TTaskOrder myTask;
          getWindowTrace()->getTaskLocation( order, myAppl, myTask );
          for ( TThreadOrder i = getWindowTrace()->getFirstThread( myAppl, myTask );
                i <= getWindowTrace()->getLastThread( myAppl, myTask ); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSETHREAD, i ) );
          }
        }
      }
      else if ( level == SYSTEM )
      {
        if ( lastLevel != COMPOSENODE )
        {
          lastLevel = COMPOSENODE;
          for ( TNodeOrder i = 0; i < getWindowTrace()->totalNodes(); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSENODE, i ) );
          }
        }
      }
      else if ( level == NODE )
      {
        if ( lastLevel != COMPOSECPU )
        {
          lastLevel = COMPOSECPU;
          TNodeOrder myNode;
          TCPUOrder myCPU;
          getWindowTrace()->getCPULocation( order, myNode, myCPU );
          for ( TCPUOrder i = getWindowTrace()->getFirstCPU( myNode );
                i <= getWindowTrace()->getLastCPU( myNode ); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSECPU, i ) );
          }
        }
      }
    }

    virtual Trace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

  private:

};


#endif // INTERVALNOTTHREAD_H_INCLUDED
