#include "processmodel.h"
#include "traceheaderexception.h"
#include <sstream>
#include <iostream>


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


ProcessModel::ProcessModel( string headerInfo )

{
  istringstream headerBuffer( headerInfo );
  TApplOrder numberApplications;
  TTaskOrder globalTasks = 0;
  TThreadOrder globalThreads = 0;
  ready = false;

  string strNumberApplications;
  std::getline( headerBuffer, strNumberApplications, ':' );
  istringstream sstrNumberAppl( strNumberApplications );

  if ( !( sstrNumberAppl >> numberApplications ) ||
       numberApplications == 0 )
  {
    throw TraceHeaderException( TraceHeaderException::invalidApplNumber,
                                strNumberApplications.c_str() );
  }

  // Insert applications
  for ( TApplOrder countAppl = 0; countAppl < numberApplications; countAppl++ )
  {
    TTaskOrder numberTasks;

    applications.push_back( ProcessModelAppl( countAppl ) );

    string strNumberTasks;
    std::getline( headerBuffer, strNumberTasks, '(' );
    istringstream sstrNumberTasks( strNumberTasks );

    if ( !( sstrNumberTasks >> numberTasks ) ||
         numberTasks == 0 )
    {
      throw TraceHeaderException( TraceHeaderException::invalidTaskNumber,
                                  strNumberTasks.c_str() );
    }

    // Insert tasks
    for ( TTaskOrder countTask = 0; countTask < numberTasks; countTask++ )
    {
      TThreadOrder numberThreads;
      TNodeOrder numberNode;

      applications[ countAppl ].tasks.push_back( ProcessModelTask( globalTasks ) );
      globalTasks++;

      string strNumberThreads;
      std::getline( headerBuffer, strNumberThreads, ':' );
      istringstream sstrNumberThreads( strNumberThreads );

      if ( !( sstrNumberThreads >> numberThreads ) ||
           numberThreads == 0 )
      {
        throw TraceHeaderException( TraceHeaderException::invalidThreadNumber,
                                    strNumberThreads.c_str() );
      }

      string strNumberNode;

      if ( countTask < numberTasks - 1 )
        std::getline( headerBuffer, strNumberNode, ',' );
      else
        std::getline( headerBuffer, strNumberNode, ')' );

      istringstream sstrNumberNode( strNumberNode );

      if ( !( sstrNumberNode >> numberNode ) )
      {
        throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                    strNumberNode.c_str() );
      }

      // Insert threads
      for ( TThreadOrder countThread = 0; countThread < numberThreads; countThread++ )
      {
        applications[ countAppl ].tasks[ countTask ].threads.push_back( ProcessModelThread( globalThreads, numberNode ) );
        globalThreads++;
      }
    }
  }

  ready = true;
}

