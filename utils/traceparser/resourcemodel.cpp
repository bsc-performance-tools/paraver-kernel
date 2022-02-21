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
#ifdef USE_PARAVER_EXCEPTIONS
#include "traceheaderexception.h"
#endif

using namespace std;

// subset
template< typename NodeOrderT,
          typename CPUOrderT >
bool ResourceModel< NodeOrderT, CPUOrderT >::operator<( const ResourceModel& other ) const
{
  bool isSubset = true;

  NodeOrderT iNode;
  CPUOrderT iCPU;
  for ( CPUOrderT iGlobalCPU = 0; iGlobalCPU < totalCPUs(); ++iGlobalCPU )
  {
    getCPULocation( iGlobalCPU, iNode, iCPU );
    if ( !other.isValidCPU( iNode, iCPU ) )
    {
      isSubset = false;
      break;
    }
  }

  return isSubset;
}

template< typename NodeOrderT,
          typename CPUOrderT >
bool ResourceModel< NodeOrderT, CPUOrderT >::operator==( const ResourceModel& other ) const
{
  /*
  return nodes == other.nodes &&
         CPUs  == other.CPUs;
         */
 return ( ( totalCPUs() == other.totalCPUs() ) && ( *this < other ) );
}

template< typename NodeOrderT,
          typename CPUOrderT >
NodeOrderT ResourceModel< NodeOrderT, CPUOrderT >::totalNodes() const
{
  return nodes.size();
}


template< typename NodeOrderT,
          typename CPUOrderT >
CPUOrderT ResourceModel< NodeOrderT, CPUOrderT >::totalCPUs() const
{
  return CPUs.size();
}


template< typename NodeOrderT,
          typename CPUOrderT >
CPUOrderT ResourceModel< NodeOrderT, CPUOrderT >::getGlobalCPU( const NodeOrderT& inNode,
                                       const CPUOrderT& inCPU ) const
{
  if( inCPU == 0 )
    return 0;
  return nodes[ inNode ].CPUs[ inCPU - 1 ].traceGlobalOrder;
}


template< typename NodeOrderT,
          typename CPUOrderT >
void ResourceModel< NodeOrderT, CPUOrderT >::getCPULocation( CPUOrderT globalCPU,
                                    NodeOrderT& inNode,
                                    CPUOrderT& inCPU ) const
{
  if( globalCPU == 0 )
  {
    inNode = 0;
    inCPU = 0;
  }
  else
  {
    inNode = CPUs[ globalCPU - 1 ].node;
    inCPU = CPUs[ globalCPU - 1 ].CPU;
  }
}


template< typename NodeOrderT,
          typename CPUOrderT >
CPUOrderT ResourceModel< NodeOrderT, CPUOrderT >::getFirstCPU( NodeOrderT inNode ) const
{
  return nodes[ inNode ].CPUs[ 0 ].traceGlobalOrder + 1;
}


template< typename NodeOrderT,
          typename CPUOrderT >
CPUOrderT ResourceModel< NodeOrderT, CPUOrderT >::getLastCPU( NodeOrderT inNode ) const
{
  return nodes[ inNode ].CPUs[
           nodes[ inNode ].CPUs.size() - 1 ].traceGlobalOrder + 1;
}


template< typename NodeOrderT,
          typename CPUOrderT >
void ResourceModel< NodeOrderT, CPUOrderT >::addNode()
{
  nodes.push_back( ResourceModelNode< NodeOrderT, CPUOrderT >( nodes.size() ) );
}

template< typename NodeOrderT,
          typename CPUOrderT >
void ResourceModel< NodeOrderT, CPUOrderT >::addCPU( NodeOrderT whichNode )
{
  if( whichNode >= nodes.size() )
  {
    stringstream tmpstr;
    tmpstr << whichNode;
#ifdef USE_PARAVER_EXCEPTIONS
    throw TraceHeaderException( TTraceHeaderErrorCode::invalidNodeNumber,
                                tmpstr.str().c_str() );
#else
    std::cerr << "[Error] Invalid node number: " << tmpstr.str() << std::endl;
    exit( 1 );
#endif
  }

  CPUs.push_back( CPULocation() );
  CPUs[ CPUs.size() - 1 ].node = whichNode;
  CPUs[ CPUs.size() - 1 ].CPU = nodes[ whichNode ].CPUs.size();
  nodes[ whichNode ].CPUs.push_back( ResourceModelCPU< NodeOrderT, CPUOrderT >( CPUs.size() - 1 ) );
}


template< typename NodeOrderT,
          typename CPUOrderT >
bool ResourceModel< NodeOrderT, CPUOrderT >::isValidNode( NodeOrderT whichNode ) const
{
  return whichNode < nodes.size();
}


template< typename NodeOrderT,
          typename CPUOrderT >
bool ResourceModel< NodeOrderT, CPUOrderT >::isValidCPU( NodeOrderT whichNode, CPUOrderT whichCPU ) const
{
  if ( !isValidNode( whichNode ) )
    return false;
  
  return whichCPU < nodes[ whichNode ].CPUs.size();
}


template< typename NodeOrderT,
          typename CPUOrderT >
bool ResourceModel< NodeOrderT, CPUOrderT >::isValidGlobalCPU( CPUOrderT whichCPU ) const
{
  return whichCPU <= CPUs.size();
}


template< typename NodeOrderT,
          typename CPUOrderT >
ResourceModel< NodeOrderT, CPUOrderT >::ResourceModel( istringstream& headerInfo )
{
  string stringNumberNodes;
  NodeOrderT numberNodes;
  CPUOrderT globalCPUs = 0;
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
#ifdef USE_PARAVER_EXCEPTIONS
    throw TraceHeaderException( TTraceHeaderErrorCode::invalidNodeNumber,
                                stringNumberNodes.c_str() );
#else
    std::cerr << "[Error] Invalid number of nodes: " << stringNumberNodes << std::endl;
    exit( 1 );
#endif
  }

  // Insert nodes
  for ( NodeOrderT countNode = 0; countNode < numberNodes; ++countNode )
  {
    CPUOrderT numberCPUs;

    nodes.push_back( ResourceModelNode< NodeOrderT, CPUOrderT >( countNode ) );

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
#ifdef USE_PARAVER_EXCEPTIONS
        throw TraceHeaderException( TTraceHeaderErrorCode::invalidCPUNumber,
                                    stringNumberCPUs.c_str() );
#else
        std::cerr << "[Error] Invalid number of CPUs: " << stringNumberCPUs << std::endl;
        exit( 1 );
#endif
      }
    }
    else
      numberCPUs = 1;

    // Insert CPUs
    for ( CPUOrderT countCPU = 0; countCPU < numberCPUs; ++countCPU )
    {
      if( globalCPUs < std::numeric_limits<CPUOrderT>::max() )
      {
        nodes[ countNode ].CPUs.push_back( ResourceModelCPU< NodeOrderT, CPUOrderT >( globalCPUs ) );
        CPUs.push_back( CPULocation() );
        CPUs[ globalCPUs ].node = countNode;
        CPUs[ globalCPUs ].CPU = countCPU;
        ++globalCPUs;
      }
    }
    // End inserting CPUs

  }
  // End inserting nodes

  // Gets a useless character: ':'
  if ( readCPUs )
    headerInfo.get();

  ready = true;
}

template< typename NodeOrderT,
          typename CPUOrderT >
void ResourceModel< NodeOrderT, CPUOrderT >::dumpToFile( fstream& file ) const
{
  ostringstream ostr;
  ostr << fixed;
  ostr << dec;
  ostr.precision( 0 );

  if ( !ready )
  {
    ostr << '0';
  }
  else
  {
    ostr << nodes.size() << '(';
    for ( NodeOrderT iNode = 0; iNode < nodes.size(); ++iNode )
    {
      ostr << nodes[ iNode ].CPUs.size();
      if ( iNode < nodes.size() - 1 )
        ostr << ',';
    }
    ostr << ')';
  }
  file << ostr.str();
}
