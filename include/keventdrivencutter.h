/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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


#ifndef KEVENTDRIVENCUTTER_H_INCLUDED
#define KEVENTDRIVENCUTTER_H_INCLUDED

#include <string>
#include <vector>

#include "eventdrivencutter.h"
#include "ktraceeditsequence.h"

class ProgressController;
class KernelConnection;

class KEventDrivenCutter : public EventDrivenCutter
{
  public:
    KEventDrivenCutter( const KernelConnection *myKernel,
                        std::string traceIn,
                        std::string traceOut,
                        TEventType whichEvent,
                        ProgressController *progress = NULL );
    ~KEventDrivenCutter();

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = NULL );

  private:
    TraceEditSequence *mySequence;

    std::vector<std::string> traces;

};

#endif // KEVENTDRIVENCUTTER_H_INCLUDED
