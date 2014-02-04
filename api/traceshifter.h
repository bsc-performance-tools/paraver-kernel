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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef TRACESHIFTER_H_INCLUDED
#define TRACESHIFTER_H_INCLUDED

#include <string>
#include <vector>

#include "paraverkerneltypes.h"

class KernelConnection;
class ProgressController;

class TraceShifter
{
  public:
    static TraceShifter *create( const KernelConnection *whichKernel,
                                 std::string traceIn,
                                 std::string traceOut,
                                 std::string shiftTimesFile,
                                 //TraceOptions *options,
                                 ProgressController *progress );

    static std::string getID() { return TraceShifter::traceToolID; }
    static std::string getName() { return TraceShifter::traceToolName; }
    static std::string getExtension() { return TraceShifter::traceToolExtension; }

    virtual ~TraceShifter()
    {}

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = NULL ) = 0;

    virtual const std::vector< TTime > getShiftTimes() = 0;

  private:
    static std::string traceToolID;
    static std::string traceToolName;
    static std::string traceToolExtension;
};


class TraceShifterProxy : public TraceShifter
{
  public:
    virtual ~TraceShifterProxy();

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = NULL );

    virtual const std::vector< TTime > getShiftTimes(); // maybe not needed
    // TODO: think about how to decide which level is going to be shifted
    //virtual size_t getShiftTimesSize(); // Thread, Task, App, All?

  private:
    TraceShifter *myTraceShifter;
    const KernelConnection *myKernel;

    TraceShifterProxy( const KernelConnection *whichKernel,
                       std::string traceIn,
                       std::string traceOut,
                       std::string shiftTimesFile,
                       //TraceOptions *options,
                       ProgressController *progress );

    friend TraceShifter *TraceShifter::create( const KernelConnection *whichKernel,
                                               std::string traceIn,
                                               std::string traceOut,
                                               std::string shiftTimesFile,
                                               //TraceOptions *options,
                                               ProgressController *progress );
};

#endif // TRACESHIFTER_H_INCLUDED
