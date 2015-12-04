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

#ifndef TRACEFILTER_H_INCLUDED
#define TRACEFILTER_H_INCLUDED

#include <map>

#include "localkernel.h"

class TraceOptions;
class KernelConnection;
class ProgressController;

class TraceFilter
{
  public:
   static TraceFilter *create( const KernelConnection *whichKernel,
                               char *traceIn,
                               char *traceOut,
                               TraceOptions *options,
                               ProgressController *progress,
                               const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable );

    static std::string getID();
    static std::string getName();
    static std::string getExtension();

    virtual ~TraceFilter()
    {}

  private:
    static std::string traceToolID;
    static std::string traceToolName;
    static std::string traceToolExtension;
};

class TraceFilterProxy : public TraceFilter
{
  public:
    virtual ~TraceFilterProxy();

  private:
    TraceFilter *myTraceFilter;

    TraceFilterProxy( const KernelConnection *whichKernel,
                      char *traceIn,
                      char *traceOut,
                      TraceOptions *options,
                      const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable,
                      ProgressController *progress );

    friend TraceFilter *TraceFilter::create( const KernelConnection *kernelConnection,
                                             char *traceIn,
                                             char *traceOut,
                                             TraceOptions *options,
                                             ProgressController *progress,
                                             const std::map< TTypeValuePair, TTypeValuePair >& whichTranslationTable );
};


#endif // TRACEFILTER_H_INCLUDED
