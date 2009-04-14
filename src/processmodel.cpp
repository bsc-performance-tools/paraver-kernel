#include <sstream>
#include <iostream>
#include <fstream>
#include "processmodel.h"
#include "traceheaderexception.h"

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
  inAppl = threads[ globalThread ].appl;
  inTask = threads[ globalThread ].task;
  inThread = threads[ globalThread ].thread;
}


ProcessModel::ProcessModel( istringstream& headerInfo )
{
  TApplOrder numberApplications;
  TTaskOrder globalTasks = 0;
  TThreadOrder globalThreads = 0;
  ready = false;

  string stringNumberApplications;
  std::getline( headerInfo, stringNumberApplications, ':' );
  istringstream sstreamNumberAppl( stringNumberApplications );

  if ( !( sstreamNumberAppl >> numberApplications ) ||
       numberApplications == 0 )
  {
    throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                stringNumberApplications.c_str() );
  }

  // Insert applications
  for ( TApplOrder countAppl = 0; countAppl < numberApplications; countAppl++ )
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
    for ( TTaskOrder countTask = 0; countTask < numberTasks; countTask++ )
    {
      TThreadOrder numberThreads;
      TNodeOrder numberNode;

      applications[ countAppl ].tasks.push_back( ProcessModelTask( globalTasks ) );
      tasks.push_back( TaskLocation() );
      tasks[ globalTasks ].appl = countAppl;
      tasks[ globalTasks ].task = countTask;
      globalTasks++;

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

      if ( !( sstreamNumberNode >> numberNode ) )
      {
        throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                    stringNumberNode.c_str() );
      }

      // Insert threads
      for ( TThreadOrder countThread = 0; countThread < numberThreads; countThread++ )
      {
        applications[ countAppl ].tasks[ countTask ].threads.push_back( ProcessModelThread( globalThreads, numberNode ) );
        threads.push_back( ThreadLocation() );
        threads[ globalThreads ].appl = countAppl;
        threads[ globalThreads ].task = countTask;
        threads[ globalThreads ].thread = countThread;
        globalThreads++;
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


void ProcessModel::dumpToFile( fstream& file ) const
{
  ostringstream ostr;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  ostr << applications.size() << ':';
  for( TApplOrder iAppl = 0; iAppl < applications.size(); ++iAppl )
  {
    ostr << applications[ iAppl ].tasks.size() << '(';
    for( TTaskOrder iTask = 0; iTask < applications[ iAppl ].tasks.size(); ++iTask )
    {
      ostr << applications[ iAppl ].tasks[ iTask ].threads.size() << ':';
      ostr << applications[ iAppl ].tasks[ iTask ].threads[ 0 ].nodeExecution;

      if( iTask < applications[ iAppl ].tasks.size() - 1 )
        ostr << ',';
    }
    ostr << ')';

    if( iAppl < applications.size() - 1 )
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
