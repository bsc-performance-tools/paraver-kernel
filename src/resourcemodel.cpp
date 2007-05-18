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


ResourceModel::ResourceModel( string headerInfo )
{
  istringstream headerBuffer( headerInfo );
  string stringNumberNodes;
  TNodeOrder numberNodes;
  TCPUOrder glogalCPUs = 0;
  ready = false;

  // Number of nodes is 0 -> No definition of nodes nor cpus
  if ( headerBuffer.peek() == '0' )
  {
    std::getline( headerBuffer, stringNumberNodes, ':' );
    return;
  }

  std::getline( headerBuffer, stringNumberNodes, '(' );
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

    string stringNumberCPUs;
    if ( countNode < numberNodes - 1 )
      std::getline( headerBuffer, stringNumberCPUs, ',' );
    else
      std::getline( headerBuffer, stringNumberCPUs, ')' );

    istringstream sstreamNumberCPUs( stringNumberCPUs );

    if ( !( sstreamNumberCPUs >> numberCPUs ) )
    {
      throw TraceHeaderException( TraceHeaderException::invalidCPUNumber,
                                  stringNumberCPUs.c_str() );
    }

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
  headerBuffer.get();

  ready = true;
}

