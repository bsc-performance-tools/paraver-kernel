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

    IntervalNotThread( KWindow *whichWindow, TWindowLevel whichLevel,
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
    KWindow *window;
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
          for ( TCPUOrder i = getWindowTrace()->getFirstCPU( order );
                i <= getWindowTrace()->getLastCPU( order ); i++ )
          {
            childIntervals.push_back( getWindowInterval( COMPOSECPU, i ) );
          }
        }
      }
    }

    virtual KTrace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

  private:
    SemanticHighInfo info;

};


#endif // INTERVALNOTTHREAD_H_INCLUDED
