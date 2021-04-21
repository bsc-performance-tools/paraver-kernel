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


#ifndef EVENTDRIVENCUTTER_H_INCLUDED
#define EVENTDRIVENCUTTER_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"

class KernelConnection;
class ProgressController;

class EventDrivenCutter
{
  public:
    static EventDrivenCutter *create( const KernelConnection *whichKernel,
                                      std::string traceIn,
                                      std::string traceOut,
                                      TEventType whichEvent,
                                      ProgressController *progress );

    static std::string getID();
    static std::string getName();
    static std::string getExtension();

    virtual ~EventDrivenCutter()
    {}

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = nullptr ) = 0;

  private:
    static std::string traceToolID;
    static std::string traceToolName;
    static std::string traceToolExtension;
};


class EventDrivenCutterProxy : public EventDrivenCutter
{
  public:
    virtual ~EventDrivenCutterProxy();

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = nullptr ) override;

  private:
    EventDrivenCutter *myEventDrivenCutter;
    const KernelConnection *myKernel;

    EventDrivenCutterProxy( const KernelConnection *whichKernel,
                            std::string traceIn,
                            std::string traceOut,
                            TEventType whichEvent,
                            ProgressController *progress );

    friend EventDrivenCutter *EventDrivenCutter::create( const KernelConnection *whichKernel,
                                                         std::string traceIn,
                                                         std::string traceOut,
                                                         TEventType whichEvent,
                                                         ProgressController *progress );
};


#endif // EVENTDRIVENCUTTER_H_INCLUDED
