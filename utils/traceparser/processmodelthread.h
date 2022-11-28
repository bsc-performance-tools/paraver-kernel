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


#pragma once



#include "processmodel.h"

template< typename ApplOrderT,
          typename TaskOrderT,
          typename ThreadOrderT,
          typename NodeOrderT >
class ProcessModelThread
{

  public:
    ProcessModelThread( ThreadOrderT order = 0,
                        NodeOrderT node = 0 ):
        traceGlobalOrder( order ),
        nodeExecution( node )
    {}

    ~ProcessModelThread()
    {}

    bool operator==( const ProcessModelThread< ApplOrderT, TaskOrderT, ThreadOrderT, NodeOrderT >& other ) const
    {
      return traceGlobalOrder == other.traceGlobalOrder &&
             nodeExecution    == other.nodeExecution;
    }

    NodeOrderT getNodeExecution() const { return nodeExecution; }

  protected:
    ThreadOrderT traceGlobalOrder;
    NodeOrderT   nodeExecution;

  private:
    template< typename ApplOrderU, typename TaskOrderU, typename ThreadOrderU, typename NodeOrderU > friend class ProcessModel;

};


