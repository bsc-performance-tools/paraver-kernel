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
  TNodeOrder numberNodes;
  TCPUOrder glogalCPUs = 0;
  ready = false;

  string stringNumberNodes;
  std::getline( headerBuffer, stringNumberNodes, ':' );
  istringstream sstreamNumberNodes( stringNumberNodes );

  if ( !( sstreamNumberNodes >> numberNodes ) )
  {
    throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                stringNumberNodes.c_str() );
  }

  if ( numberNodes == 0 )
    return;

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

  }
}

