#include <sstream>
#include <iostream>
#include "resourcemodel.h"
#include "traceheaderexception.h"


TNodeOrder ResourceModel::totalNodes() const
{
  return nodes.size();
}


TCPUOrder ResourceModel::totalCPUs() const
{
  TCPUOrder countCPUs = 0;

  for ( TNodeOrder countNode = 0; countNode < nodes.size(); countNode++ )
    countCPUs += nodes[ countNode ].CPUs.size();

  return countCPUs;
}


TCPUOrder ResourceModel::getGlobalCPU( const TNodeOrder& inNode,
                                       const TCPUOrder& inCPU ) const
{
  return nodes[ inNode ].CPUs[ inCPU ].traceGlobalOrder;
}


ResourceModel::ResourceModel( istringstream& headerInfo )
{
  string stringNumberNodes;
  TNodeOrder numberNodes;
  TCPUOrder glogalCPUs = 0;
  bool readCPUs;
  ready = false;

  // Number of nodes is 0 -> No definition of nodes nor cpus
  if ( headerInfo.peek() == '0' )
  {
    std::getline( headerInfo, stringNumberNodes, ':' );
    return;
  }

  if ( headerInfo.str().find_first_of( '(', headerInfo.tellg() ) <
       headerInfo.str().find_first_of( ':', headerInfo.tellg() ) )
  {
    std::getline( headerInfo, stringNumberNodes, '(' );
    readCPUs = true;
  }
  else
  {
    std::getline( headerInfo, stringNumberNodes, ':' );
    readCPUs = false;
  }
  istringstream sstreamNumberNodes( stringNumberNodes );

  if ( !( sstreamNumberNodes >> numberNodes ) )
  {
    throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                stringNumberNodes.c_str() );
  }

  // Insert nodes
  for ( TNodeOrder countNode = 0; countNode < numberNodes; countNode++ )
  {
    TCPUOrder numberCPUs;

    nodes.push_back( ResourceModelNode( countNode ) );

    if ( readCPUs )
    {
      string stringNumberCPUs;
      if ( countNode < numberNodes - 1 )
        std::getline( headerInfo, stringNumberCPUs, ',' );
      else
        std::getline( headerInfo, stringNumberCPUs, ')' );

      istringstream sstreamNumberCPUs( stringNumberCPUs );

      if ( !( sstreamNumberCPUs >> numberCPUs ) )
      {
        throw TraceHeaderException( TraceHeaderException::invalidCPUNumber,
                                    stringNumberCPUs.c_str() );
      }
    }
    else
      numberCPUs = 1;

    // Insert CPUs
    for ( TCPUOrder countCPU = 0; countCPU < numberCPUs; countCPU++ )
    {
      nodes[ countNode ].CPUs.push_back( ResourceModelCPU( glogalCPUs ) );
      glogalCPUs++;
    }
    // End inserting CPUs

  }
  // End inserting nodes

  // Gets a useless character: ':'
  if ( readCPUs )
    headerInfo.get();

  ready = true;
}

