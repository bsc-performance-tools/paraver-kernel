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

#ifndef PROCESSMODEL_H_INCLUDED
#define PROCESSMODEL_H_INCLUDED

#include <vector>
#include <string>
#include "processmodelappl.h"

class Trace;

class ProcessModel
{

  public:
    ProcessModel()
    {
      ready = false;
    }

    ProcessModel( std::istringstream& headerInfo, Trace *whichTrace );

    ~ProcessModel()
    {}

    bool operator==( const ProcessModel& other ) const;

    bool isReady() const
    {
      return ready;
    }

    void setReady( bool newValue )
    {
      ready = newValue;
    }

    void dumpToFile( std::fstream& file ) const;

    TApplOrder totalApplications() const;

    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;
    void getTaskLocation( TTaskOrder globalTask,
                          TApplOrder& inAppl,
                          TTaskOrder& inTask ) const;
    TTaskOrder getFirstTask( TApplOrder inAppl ) const;
    TTaskOrder getLastTask( TApplOrder inAppl ) const;

    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;
    void getThreadLocation( TThreadOrder globalThread,
                            TApplOrder& inAppl,
                            TTaskOrder& inTask,
                            TThreadOrder& inThread ) const;
    TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const;

    void getThreadsPerNode( TNodeOrder inNode, std::vector<TThreadOrder>& onVector ) const;

    bool isValidThread( TThreadOrder whichThread ) const;
    bool isValidThread( TApplOrder whichAppl,
                        TTaskOrder whichTask,
                        TThreadOrder whichThread ) const;
    bool isValidTask( TTaskOrder whichTask ) const;
    bool isValidTask( TApplOrder whichAppl,
                        TTaskOrder whichTask ) const;
    bool isValidAppl( TApplOrder whichAppl ) const;

    void addApplication( TApplOrder whichAppl );
    void addTask( TApplOrder whichAppl, TTaskOrder whichTask );
    void addThread( TApplOrder whichAppl, TTaskOrder whichTask,
                    TThreadOrder whichThread, TNodeOrder execNode );

  protected:

    struct ThreadLocation
    {
      TApplOrder appl;
      TTaskOrder task;
      TThreadOrder thread;

      bool operator==( const ThreadLocation& other ) const
      {
        return appl   == other.appl &&
               task   == other.task &&
               thread == other.thread;
      }
    };

    struct TaskLocation
    {
      TApplOrder appl;
      TTaskOrder task;

      bool operator==( const TaskLocation& other ) const
      {
        return appl == other.appl &&
               task == other.task;
      }
    };

    std::vector<ThreadLocation> threads;
    std::vector<TaskLocation> tasks;
    std::vector<ProcessModelAppl> applications;

    bool ready;

  private:

};




#endif // PROCESSMODEL_H_INCLUDED
