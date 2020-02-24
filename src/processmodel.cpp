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


#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include "processmodel.h"
#include "traceheaderexception.h"
#include "trace.h"

#include <stdlib.h>

using namespace std;

#ifdef WIN32
#undef max
#undef min
#endif

bool ProcessModel::operator==( const ProcessModel& other ) const
{
  return applications == other.applications &&
         tasks        == other.tasks        &&
         threads      == other.threads;
}

TApplOrder ProcessModel::totalApplications() const
{
  return applications.size();
}


TTaskOrder ProcessModel::totalTasks() const
{
  return tasks.size();
}


void ProcessModel::getTaskLocation( TTaskOrder globalTask,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask ) const
{
  inAppl = tasks[ globalTask ].appl;
  inTask = tasks[ globalTask ].task;
}


TTaskOrder ProcessModel::getGlobalTask( const TApplOrder& inAppl,
                                        const TTaskOrder& inTask ) const
{
  std::cout << "LEN: " << applications.size() << " -> " << inAppl << ", " << inTask << std::endl ;
  std::cout << "LEN: " << applications[ inAppl ].tasks.size() << std::endl ;
  return applications[ inAppl ].tasks[ inTask ].traceGlobalOrder;
}


TThreadOrder ProcessModel::totalThreads() const
{
  return threads.size();
}


TThreadOrder ProcessModel::getGlobalThread( const TApplOrder& inAppl,
    const TTaskOrder& inTask,
    const TThreadOrder& inThread ) const
{
  return applications[ inAppl ].tasks[ inTask ].threads[ inThread ].traceGlobalOrder;
}


void ProcessModel::getThreadLocation( TThreadOrder globalThread,
                                      TApplOrder& inAppl,
                                      TTaskOrder& inTask,
                                      TThreadOrder& inThread ) const
{
  //////////////////////////////////////
  inAppl = threads[ globalThread ].appl;
  inTask = threads[ globalThread ].task;
  inThread = threads[ globalThread ].thread;
}


ProcessModel::ProcessModel( istringstream& headerInfo, Trace *whichTrace )
{
  TApplOrder numberApplications;
  TTaskOrder globalTasks = 0;
  TThreadOrder globalThreads = 0;
  ready = false;

  string stringNumberApplications;
  std::getline( (istream&)headerInfo, stringNumberApplications, ':' );
  istringstream sstreamNumberAppl( stringNumberApplications );

  if ( !( sstreamNumberAppl >> numberApplications ) ||
       numberApplications == 0 )
  {
    throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                stringNumberApplications.c_str() );
  }

  // Insert applications
  for ( TApplOrder countAppl = 0; countAppl < numberApplications; ++countAppl )
  {
    TTaskOrder numberTasks;

    applications.push_back( ProcessModelAppl( countAppl ) );

    string stringNumberTasks;
    std::getline( headerInfo, stringNumberTasks, '(' );
    istringstream sstreamNumberTasks( stringNumberTasks );

    if ( !( sstreamNumberTasks >> numberTasks ) ||
         numberTasks == 0 )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTaskNumber,
                                  stringNumberTasks.c_str() );
    }

    // Insert tasks
    for ( TTaskOrder countTask = 0; countTask < numberTasks; ++countTask )
    {
      TThreadOrder numberThreads;
      TNodeOrder numberNode;

      if( globalTasks < std::numeric_limits<TTaskOrder>::max() )
      {
        applications[ countAppl ].tasks.push_back( ProcessModelTask( globalTasks ) );
        tasks.push_back( TaskLocation() );
        tasks[ globalTasks ].appl = countAppl;
        tasks[ globalTasks ].task = countTask;
        ++globalTasks;
      }
      string stringNumberThreads;
      std::getline( headerInfo, stringNumberThreads, ':' );
      istringstream sstreamNumberThreads( stringNumberThreads );

      if ( !( sstreamNumberThreads >> numberThreads ) ||
           numberThreads == 0 )
      {
        throw TraceHeaderException( TraceHeaderException::invalidThreadNumber,
                                    stringNumberThreads.c_str() );
      }

      string stringNumberNode;

      if ( countTask < numberTasks - 1 )
        std::getline( headerInfo, stringNumberNode, ',' );
      else
        std::getline( headerInfo, stringNumberNode, ')' );

      istringstream sstreamNumberNode( stringNumberNode );

      if ( !( sstreamNumberNode >> numberNode ) ||
           ( numberNode == 0 && whichTrace->existResourceInfo() ) )
      {
        throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                    stringNumberNode.c_str() );
      }

      // Insert threads
      for ( TThreadOrder countThread = 0; countThread < numberThreads; ++countThread )
      {
        if( globalThreads < std::numeric_limits<TThreadOrder>::max() )
        {
          applications[ countAppl ].tasks[ countTask ].threads.push_back( 
              ProcessModelThread( globalThreads, numberNode - 1 ) );
          threads.push_back( ThreadLocation() );
          threads[ globalThreads ].appl = countAppl;
          threads[ globalThreads ].task = countTask;
          threads[ globalThreads ].thread = countThread;

          map<TNodeOrder, vector<TThreadOrder> >::iterator nodeIt = threadsPerNode.find( numberNode - 1 );
          if( nodeIt == threadsPerNode.end() )
            threadsPerNode[ numberNode - 1 ] = vector<TThreadOrder>();
          threadsPerNode[ numberNode - 1 ].push_back( globalThreads );

          ++globalThreads;
        }
      }
      // End inserting threads
    }
    // End inserting tasks

    // Gets a useless character: ':' or ','
    headerInfo.get();

  }
  // End inserting applications

  ready = true;
}

ProcessModel::ProcessModel( Trace *whichTrace, const std::string& fileName)
{                                                              // headerInfo
  fstream file( fileName, ios::in );
  TApplOrder numberApplications = 0;
  TTaskOrder numberTasks = 0;
  TThreadOrder numberThreads = 0;

  TTaskOrder globalTasks = 0;
  TThreadOrder globalThreads = 0;
  ready = false;

  string attTuple, stringNumberApplications;
  while ( std::getline( file, attTuple, '\t' ) )
  {
    std::vector< std::string > ATT; // { App, Task, Thread }
    boost::split( ATT, attTuple, boost::is_any_of("."), boost::token_compress_on );

    // Insert application
    string stringNumberAppl = ATT[ 0 ];
    TApplOrder countAppl;
    istringstream sstreamNumberAppl( ATT[ 0 ] );
    if ( !( sstreamNumberAppl >> countAppl ) || countAppl == 0 )
    {
      throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                  ATT[ 0 ].c_str() );
    }

    if ( countAppl > numberApplications )
    {       
      applications.push_back( ProcessModelAppl( countAppl ) );
      numberApplications = countAppl;
      numberTasks = 0;
      numberThreads = 0;
    }


    // Insert task
    string stringNumberTasks = ATT[ 1 ];
    TTaskOrder countTask;
    istringstream sstreamNumberTasks( stringNumberTasks );

    if ( !( sstreamNumberTasks >> countTask ) || countTask == 0 )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTaskNumber,
                                  stringNumberTasks.c_str() );
    }

    if ( countTask > numberTasks && globalTasks < std::numeric_limits<TThreadOrder>::max() )
    {      
      applications[ countAppl - 1 ].tasks.push_back( ProcessModelTask( globalTasks ) );
      numberTasks = countTask;
      numberThreads = 0;

      tasks.push_back( TaskLocation() );
      tasks[ globalTasks ].appl = countAppl;
      tasks[ globalTasks ].task = countTask;
      ++globalTasks;
    }


    // Insert thread
    string stringNumberThreads = ATT[ 2 ];
    TThreadOrder countThread;
    istringstream sstreamNumberThreads( stringNumberThreads );
    if ( !( sstreamNumberThreads >> countThread ) || countThread == 0 )
    {
      throw TraceHeaderException( TraceHeaderException::invalidThreadNumber,
                                  stringNumberThreads.c_str() );
    }

    if ( countThread > numberThreads && globalThreads < std::numeric_limits<TThreadOrder>::max() )
    {        
      applications[ countAppl - 1 ].tasks[ countTask - 1 ].threads.push_back( 
          ProcessModelThread( globalThreads, 0 ) );
      threads.push_back( ThreadLocation() );
      threads[ globalThreads ].appl = countAppl;
      threads[ globalThreads ].task = countTask;
      threads[ globalThreads ].thread = countThread;
      numberThreads = countThread;

      map<TNodeOrder, vector<TThreadOrder> >::iterator nodeIt = threadsPerNode.find( 0 );
      if( nodeIt == threadsPerNode.end() )
        threadsPerNode[ 0 ] = vector<TThreadOrder>();
      threadsPerNode[ 0 ].push_back( globalThreads );

      ++globalThreads;
    }    
    std::getline( file, attTuple, '\n' ); //get next line
  }
  ready = true;
}

void ProcessModel::dumpToFile( fstream& file, bool existResourceInfo ) const
{
  ostringstream ostr;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  ostr << applications.size() << ':';
  for ( TApplOrder iAppl = 0; iAppl < applications.size(); ++iAppl )
  {
    ostr << applications[ iAppl ].tasks.size() << '(';
    for ( TTaskOrder iTask = 0; iTask < applications[ iAppl ].tasks.size(); ++iTask )
    {
      ostr << applications[ iAppl ].tasks[ iTask ].threads.size() << ':';
      if( existResourceInfo )
        ostr << applications[ iAppl ].tasks[ iTask ].threads[ 0 ].nodeExecution + 1;
      else
        ostr << "0";

      if ( iTask < applications[ iAppl ].tasks.size() - 1 )
        ostr << ',';
    }
    ostr << ')';

    if ( iAppl < applications.size() - 1 )
      ostr << ':';
  }
  file << ostr.str();
}


TTaskOrder ProcessModel::getFirstTask( TApplOrder inAppl ) const
{
  return applications[ inAppl ].tasks[ 0 ].traceGlobalOrder;
}


TTaskOrder ProcessModel::getLastTask( TApplOrder inAppl ) const
{
  return applications[ inAppl ].tasks[
           applications[ inAppl ].tasks.size() - 1 ].traceGlobalOrder;
}


TThreadOrder ProcessModel::getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const
{
  return applications[ inAppl ].tasks[ inTask ].threads[ 0 ].traceGlobalOrder;
}


TThreadOrder ProcessModel::getLastThread( TApplOrder inAppl, TTaskOrder inTask )const
{
  return applications[ inAppl ].tasks[ inTask ].threads[
           applications[ inAppl ].tasks[ inTask ].threads.size() - 1 ].traceGlobalOrder;
}

void ProcessModel::getThreadsPerNode( TNodeOrder inNode, vector<TThreadOrder>& onVector ) const
{
  onVector.clear();

  if( inNode == 0 ) return;

  map<TNodeOrder, vector<TThreadOrder> >::const_iterator nodeIt = threadsPerNode.find( inNode - 1 );
  if( nodeIt != threadsPerNode.end() )
    onVector = nodeIt->second;
}

bool ProcessModel::isValidThread( TThreadOrder whichThread ) const
{
  return whichThread < threads.size();
}

bool ProcessModel::isValidThread( TApplOrder whichAppl,
                                  TTaskOrder whichTask,
                                  TThreadOrder whichThread ) const
{
  if( !isValidAppl( whichAppl ) )
    return false;

  if( whichTask >= applications[ whichAppl ].tasks.size() )
    return false;

  return whichThread < applications[ whichAppl ].tasks[ whichTask ].threads.size();
}

bool ProcessModel::isValidTask( TTaskOrder whichTask ) const
{
  return whichTask < tasks.size();
}

bool ProcessModel::isValidTask( TApplOrder whichAppl,
                                TTaskOrder whichTask ) const
{
  if( !isValidAppl( whichAppl ) )
    return false;

  return whichTask < applications[ whichAppl ].tasks.size();
}

bool ProcessModel::isValidAppl( TApplOrder whichAppl ) const
{
  return whichAppl < applications.size();
}

void ProcessModel::addApplication(  )
{
  applications.push_back( ProcessModelAppl( applications.size() ) );
}

void ProcessModel::addTask(  TApplOrder whichAppl )
{
  if( whichAppl > applications.size() )
  {
    stringstream tmpstr;
    tmpstr << whichAppl;
    throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                tmpstr.str().c_str() );
  }

  tasks.push_back( TaskLocation() );
  tasks[ tasks.size() - 1 ].appl = whichAppl;
  tasks[ tasks.size() - 1 ].task = applications[ whichAppl ].tasks.size();
  applications[ whichAppl ].tasks.push_back( ProcessModelTask( tasks.size() - 1 ) );

}

void ProcessModel::addThread(  TApplOrder whichAppl, TTaskOrder whichTask,
                              TNodeOrder execNode )
{
  if( whichAppl > applications.size() )
  {
    stringstream tmpstr;
    tmpstr << whichAppl;
    throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                tmpstr.str().c_str() );
  }
  else if( whichTask > applications[ whichAppl ].tasks.size() )
  {
    stringstream tmpstr;
    tmpstr << whichAppl;
    throw TraceHeaderException( TraceHeaderException::invalidTaskNumber,
                                tmpstr.str().c_str() );
  }

  threads.push_back( ThreadLocation() );
  threads[ threads.size() - 1 ].appl = whichAppl;
  //threads[ threads.size() - 1 ].task = applications[ whichAppl ].tasks[ whichTask ].traceGlobalOrder;
  threads[ threads.size() - 1 ].task = whichTask;
  threads[ threads.size() - 1 ].thread = applications[ whichAppl ].tasks[ whichTask ].threads.size();
  applications[ whichAppl ].tasks[ whichTask ].threads.push_back( ProcessModelThread( threads.size() - 1, execNode ) );

}
