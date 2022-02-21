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


#pragma once


#include <vector>
#include <string>
#include <map>
#include <sstream>
#include "processmodelappl.h"

template< typename ApplOrderT = TApplOrder,
          typename TaskOrderT = TTaskOrder,
          typename ThreadOrderT = TThreadOrder,
          typename NodeOrderT = TNodeOrder >
class ProcessModel
{

  public:
    struct ThreadLocation
    {
      ApplOrderT appl;
      TaskOrderT task;
      ThreadOrderT thread;

      bool operator==( const ThreadLocation& other ) const
      {
        return appl   == other.appl &&
               task   == other.task &&
               thread == other.thread;
      }
    };
    
    ProcessModel()
    {
      ready = false;
    }

    ProcessModel( std::istringstream& headerInfo, bool existResourceInfo );

    ~ProcessModel()
    {}

    bool operator<( const ProcessModel< ApplOrderT, TaskOrderT, ThreadOrderT, NodeOrderT >& other ) const;
    bool operator==( const ProcessModel< ApplOrderT, TaskOrderT, ThreadOrderT, NodeOrderT >& other ) const;

    bool isReady() const
    {
      return ready;
    }

    void setReady( bool newValue )
    {
      ready = newValue;
    }

    void dumpToFile( std::fstream& file, bool existResourceInfo ) const;

    ApplOrderT totalApplications() const;

    TaskOrderT totalTasks() const;
    TaskOrderT getGlobalTask( const ApplOrderT& inAppl,
                              const TaskOrderT& inTask ) const;
    void getTaskLocation( TaskOrderT globalTask,
                          ApplOrderT& inAppl,
                          TaskOrderT& inTask ) const;
    TaskOrderT getFirstTask( ApplOrderT inAppl ) const;
    TaskOrderT getLastTask( ApplOrderT inAppl ) const;

    ThreadOrderT totalThreads() const;
    ThreadOrderT getGlobalThread( const ApplOrderT& inAppl,
                                  const TaskOrderT& inTask,
                                  const ThreadOrderT& inThread ) const;
    void getThreadLocation( ThreadOrderT globalThread,
                            ApplOrderT& inAppl,
                            TaskOrderT& inTask,
                            ThreadOrderT& inThread ) const;
    ThreadOrderT getFirstThread( ApplOrderT inAppl, TaskOrderT inTask ) const;
    ThreadOrderT getLastThread( ApplOrderT inAppl, TaskOrderT inTask )const;

    void getThreadsPerNode( NodeOrderT inNode, std::vector<ThreadOrderT>& onVector ) const;

    bool isValidThread( ThreadOrderT whichThread ) const;
    bool isValidThread( ApplOrderT whichAppl,
                        TaskOrderT whichTask,
                        ThreadOrderT whichThread ) const;
    bool isValidTask( TaskOrderT whichTask ) const;
    bool isValidTask( ApplOrderT whichAppl,
                        TaskOrderT whichTask ) const;
    bool isValidAppl( ApplOrderT whichAppl ) const;

    void addApplication();
    void addTask( ApplOrderT whichAppl );
    void addThread( ApplOrderT whichAppl, TaskOrderT whichTask, NodeOrderT execNode );
    void addApplTaskThread( const ThreadLocation& whichLocation,
                            NodeOrderT execNode = 0 );


  protected:

    struct TaskLocation
    {
      ApplOrderT appl;
      TaskOrderT task;

      bool operator==( const TaskLocation& other ) const
      {
        return appl == other.appl &&
               task == other.task;
      }
    };

    std::vector< ThreadLocation > threads;
    std::vector< TaskLocation > tasks;
    std::vector< ProcessModelAppl< ApplOrderT, TaskOrderT, ThreadOrderT, NodeOrderT > > applications;
    std::map< NodeOrderT, std::vector< ThreadOrderT > > threadsPerNode;

    bool ready;

  private:

};

#include "processmodel.cpp"
