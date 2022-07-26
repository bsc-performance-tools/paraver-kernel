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


#include <unordered_map>

#include "intervalhigh.h"
#include "semanticcpu.h"
#include "semanticthread.h"
#include "semanticcompose.h"
#include "intervalcompose.h"
#include "intervalthread.h"

class KSingleWindow;
class SemanticCPU;

class IntervalCPU: public IntervalHigh
{
  public:
    IntervalCPU()
    {
      begin = nullptr;
      end = nullptr;
      function = nullptr;
      functionThread = nullptr;
      functionComposeThread = nullptr;
    }

    IntervalCPU( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                 TObjectOrder whichOrder );

    virtual ~IntervalCPU()
    {
      if ( begin != nullptr )
        delete begin;
      if ( end != nullptr )
        delete end;
      if( functionThread != nullptr )
        delete functionThread;
      if( functionComposeThread != nullptr )
        delete functionComposeThread;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = nullptr ) override;
    virtual KRecordList *calcNext( KRecordList *displayList = nullptr, bool initCalc = false ) override;
    virtual KRecordList *calcPrev( KRecordList *displayList = nullptr, bool initCalc = false ) override;

    virtual KTimeline *getWindow() override
    {
      return ( KTimeline * ) window;
    }

  protected:
    KSingleWindow *window;
    SemanticCPU *function;
    TCreateList createList;
    SemanticThread *functionThread;
    SemanticCompose *functionComposeThread;
    std::unordered_map<TThreadOrder, IntervalCompose *> intervalCompose;
    std::unordered_map<TThreadOrder, IntervalThread *> intervalThread;
    TRecordTime currentInitialTime;

  private:
    virtual MemoryTrace::iterator *getPrevRecord( MemoryTrace::iterator *it,
                                                  KRecordList *displayList );
    virtual MemoryTrace::iterator *getNextRecord( MemoryTrace::iterator *it,
                                                  KRecordList *displayList );

    virtual void setChildren() override {};
    virtual KTrace *getWindowTrace() const override;
    virtual TTraceLevel getWindowLevel() const override;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder ) override;
    virtual bool IsDerivedWindow() const override;
    virtual TWindowLevel getComposeLevel( TTraceLevel whichLevel ) const override;

};



