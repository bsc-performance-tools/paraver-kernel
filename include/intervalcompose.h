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


#include "intervalhigh.h"
#include "semanticcompose.h"

class KWindow;

class IntervalCompose: public IntervalHigh
{
  public:
    IntervalCompose()
    {
      function = nullptr;
      endRecord = nullptr;
      beginRecord = nullptr;
    }

    IntervalCompose( KWindow *whichWindow, TWindowLevel whichLevel,
                     TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      function = nullptr;
      endRecord = nullptr;
      beginRecord = nullptr;
    }

    virtual ~IntervalCompose()
    {
      if( endRecord != nullptr )
        delete endRecord;
      if( beginRecord != nullptr )
        delete beginRecord;
      if( begin != nullptr )
        delete begin;
      if( end != nullptr )
        delete end;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                               KRecordList *displayList = nullptr ) override;
    virtual KRecordList *calcNext( KRecordList *displayList = nullptr, bool initCalc = false ) override;
    virtual KRecordList *calcPrev( KRecordList *displayList = nullptr, bool initCalc = false ) override;

    virtual KWindow *getWindow() override
    {
      return window;
    }

    virtual void setSemanticFunction( SemanticCompose *whichFunction )
    {
      function = whichFunction;
    }

    virtual void setCustomChild( Interval *whichChild )
    {
      childIntervals.clear();
      childIntervals.push_back( whichChild );
    }

  protected:
    KWindow *window;
    SemanticCompose *function;
    TCreateList createList;

    virtual void setChildren() override;
    virtual TWindowLevel getWindowLevel() const override;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel, TObjectOrder whichOrder ) override;
    virtual bool IsDerivedWindow() const override;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const override;
    virtual KTrace *getWindowTrace() const override;

  private:
    bool joinBursts;
    MemoryTrace::iterator *endRecord;
    MemoryTrace::iterator *beginRecord;

};



