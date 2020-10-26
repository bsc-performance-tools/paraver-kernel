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


#ifndef RESOURCEMODEL_H_INCLUDED
#define RESOURCEMODEL_H_INCLUDED

#include <vector>
#include <string>
#include "resourcemodelnode.h"

class ResourceModel
{
  public:
    ResourceModel()
    {
      ready = false;
    }

    ResourceModel( std::istringstream& headerInfo );

    ~ResourceModel()
    {}

    bool operator<( const ResourceModel& other ) const;
    bool operator==( const ResourceModel& other ) const;

    bool isReady() const
    {
      return ready;
    }

    void setReady( bool newValue )
    {
      ready = newValue;
    }

    void dumpToFile( std::fstream& file ) const;

    TNodeOrder totalNodes() const;
    TCPUOrder totalCPUs() const;

    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;
    void getCPULocation( TCPUOrder globalCPU,
                         TNodeOrder& inNode,
                         TCPUOrder& inCPU ) const;
    TCPUOrder getFirstCPU( TNodeOrder inNode ) const;
    TCPUOrder getLastCPU( TNodeOrder inNode ) const;

    void addNode();
    void addCPU( TNodeOrder whichNode );

    bool isValidNode( TNodeOrder whichNode ) const;
    bool isValidCPU( TNodeOrder whichNode, TCPUOrder whichCPU ) const;
    bool isValidGlobalCPU( TCPUOrder whichCPU ) const;

  protected:
    struct CPULocation
    {
      TNodeOrder node;
      TCPUOrder CPU;

      bool operator==( const CPULocation& other ) const
      {
        return node == other.node &&
               CPU  == other.CPU;
      }
    };

    std::vector<CPULocation> CPUs;
    std::vector<ResourceModelNode> nodes;
    bool ready;

  private:

};

#endif // RESOURCEMODEL_H_INCLUDED
