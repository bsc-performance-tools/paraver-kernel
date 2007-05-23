#include <sstream>
#include <iostream>
#include "processmodel.h"
#include "traceheaderexception.h"

TApplOrder ProcessModel::totalApplications() const
{
  return applications.size();
}


TTaskOrder ProcessModel::totalTasks() const
{
  TTaskOrder countTasks = 0;

  for ( TApplOrder countAppl = 0; countAppl < applications.size(); countAppl++ )
    countTasks += applications[ countAppl ].tasks.size();

  return countTasks;
}


TThreadOrder ProcessModel::totalThreads() const
{
  TThreadOrder countThreads = 0;

  for ( TApplOrder countAppl = 0; countAppl < applications.size(); countAppl++ )
  {
    for ( TTaskOrder countTask = 0; countTask < applications[ countAppl ].tasks.size(); countTask++ )
      countThreads += applications[ countAppl ].tasks[ countTask ].threads.size();
  }

  return countThreads;
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

