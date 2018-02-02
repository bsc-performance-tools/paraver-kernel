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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include "resourcemodel.h"
#include "traceheaderexception.h"

using namespace std;

bool ResourceModel::operator==( const ResourceModel& other ) const
{
  return nodes == other.nodes &&
         CPUs  == other.CPUs;
}

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
  if( inCPU == 0 )
    return 0;
  return nodes[ inNode ].CPUs[ inCPU - 1 ].traceGlobalOrder;
}


void ResourceModel::getCPULocation( TCPUOrder globalCPU,
                                    TNodeOrder& inNode,
                                    TCPUOrder& inCPU ) const
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


TCPUOrder ResourceModel::getFirstCPU( TNodeOrder inNode ) const
{
  return nodes[ inNode ].CPUs[ 0 ].traceGlobalOrder + 1;
}


TCPUOrder ResourceModel::getLastCPU( TNodeOrder inNode ) const
{
  return nodes[ inNode ].CPUs[
           nodes[ inNode ].CPUs.size() - 1 ].traceGlobalOrder + 1;
}


void ResourceModel::addNode()
{
  nodes.push_back( ResourceModelNode( nodes.size() ) );
}

void ResourceModel::addCPU( TNodeOrder whichNode )
{
  if( whichNode >= nodes.size() )
  {
    stringstream tmpstr;
    tmpstr << whichNode;
    throw TraceHeaderException( TraceHeaderException::invalidNodeNumber,
                                tmpstr.str().c_str() );
  }

  CPUs.push_back( CPULocation() );
  CPUs[ CPUs.size() - 1 ].node = whichNode;
  CPUs[ CPUs.size() - 1 ].CPU = nodes[ whichNode ].CPUs.size();
  nodes[ whichNode ].CPUs.push_back( ResourceModelCPU( CPUs.size() - 1 ) );
}


bool ResourceModel::isValidCPU( TCPUOrder whichCPU ) const
{
  return whichCPU == 0 || whichCPU < CPUs.size();
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
  for ( TNodeOrder countNode = 0; countNode < numberNodes; ++countNode )
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
    for ( TCPUOrder countCPU = 0; countCPU < numberCPUs; ++countCPU )
    {
      if( globalCPUs < std::numeric_limits<TCPUOrder>::max() )
      {
        nodes[ countNode ].CPUs.push_back( ResourceModelCPU( globalCPUs ) );
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

void ResourceModel::dumpToFile( fstream& file ) const
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
    for ( TNodeOrder iNode = 0; iNode < nodes.size(); ++iNode )
    {
      ostr << nodes[ iNode ].CPUs.size();
      if ( iNode < nodes.size() - 1 )
        ostr << ',';
    }
    ostr << ')';
  }
  file << ostr.str();
}
