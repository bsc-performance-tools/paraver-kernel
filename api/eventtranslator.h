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


#pragma once


#include <string>
#include "paraverkerneltypes.h"

class KernelConnection;
class ProgressController;

class EventTranslator
{
  public:
    static EventTranslator *create( const KernelConnection *whichKernel,
                                      std::string traceIn,
                                      std::string traceOut,
                                      std::string traceReference,
                                      ProgressController *progress );

    static std::string getID();
    static std::string getName();
    static std::string getExtension();

    virtual ~EventTranslator()
    {}

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = nullptr ) = 0;

    virtual bool translationEmpty() = 0;

  private:
    static std::string traceToolID;
    static std::string traceToolName;
    static std::string traceToolExtension;
};


class EventTranslatorProxy : public EventTranslator
{
  public:
    virtual ~EventTranslatorProxy();

    virtual void execute( std::string traceIn,
                          std::string traceOut,
                          ProgressController *progress = nullptr ) override;

    virtual bool translationEmpty() override;

  private:
    EventTranslator *myEventTranslator;
    const KernelConnection *myKernel;

    EventTranslatorProxy( const KernelConnection *whichKernel,
                            std::string traceIn,
                            std::string traceOut,
                            std::string traceReference,
                            ProgressController *progress );

    friend EventTranslator *EventTranslator::create( const KernelConnection *whichKernel,
                                                         std::string traceIn,
                                                         std::string traceOut,
                                                         std::string traceReference,
                                                         ProgressController *progress );
};



