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
  return CPUs.size();
}


TCPUOrder ResourceModel::getGlobalCPU( const TNodeOrder& inNode,
                                       const TCPUOrder& inCPU ) const
{
  return nodes[ inNode ].CPUs[ inCPU ].traceGlobalOrder;
}


void ResourceModel::getCPULocation( TCPUOrder globalCPU,
                                    TNodeOrder& inNode,
                                    TCPUOrder& inCPU ) const
{
  inNode = CPUs[ globalCPU ].node;
  inCPU = CPUs[ globalCPU ].CPU;
}


TCPUOrder ResourceModel::getFirstCPU( TNodeOrder inNode ) const
{
  return nodes[ inNode ].CPUs[ 0 ].traceGlobalOrder;
}


TCPUOrder ResourceModel::getLastCPU( TNodeOrder inNode ) const
{
  return nodes[ inNode ].CPUs[
           nodes[ inNode ].CPUs.size() - 1 ].traceGlobalOrder;
}


ResourceModel::ResourceModel( istringstream& headerInfo )
{
  string stringNumberNodes;
  TNodeOrder numberNodes;
  TCPUOrder globalCPUs = 0;
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
      nodes[ countNode ].CPUs.push_back( ResourceModelCPU( globalCPUs ) );
      CPUs.push_back( CPULocation() );
      CPUs[ globalCPUs ].node = countNode;
      CPUs[ globalCPUs ].CPU = countCPU;
      globalCPUs++;
    }
    // End inserting CPUs

  }
  // End inserting nodes

  // Gets a useless character: ':'
  if ( readCPUs )
    headerInfo.get();

  ready = true;
}


