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

#ifndef INTERVALCPU_H_INCLUDED
#define INTERVALCPU_H_INCLUDED

#include "intervalhigh.h"
#include "semanticcpu.h"
#include "semanticthread.h"

class KSingleWindow;
class SemanticCPU;

class IntervalCPU: public IntervalHigh
{
  public:
    IntervalCPU()
    {
      begin = NULL;
      end = NULL;
      function = NULL;
    }

    IntervalCPU( KSingleWindow *whichWindow, TWindowLevel whichLevel,
                 TObjectOrder whichOrder ):
        IntervalHigh( whichLevel, whichOrder ), window( whichWindow )
    {
      begin = NULL;
      end = NULL;
      function = NULL;
    }

    virtual ~IntervalCPU()
    {
      if ( begin != NULL )
        delete begin;
      if ( end != NULL )
        delete end;
    }

    virtual KRecordList *init( TRecordTime initialTime, TCreateList create,
                              KRecordList *displayList = NULL );
    virtual KRecordList *calcNext( KRecordList *displayList = NULL, bool initCalc = false );
    virtual KRecordList *calcPrev( KRecordList *displayList = NULL, bool initCalc = false );

    virtual KWindow *getWindow()
    {
      return ( KWindow * ) window;
    }

  protected:
    KSingleWindow *window;
    SemanticCPU *function;
    SemanticThread *functionThread;
    TCreateList createList;

  private:
    virtual MemoryTrace::iterator *getNextRecord( MemoryTrace::iterator *it,
        KRecordList *displayList );
    virtual MemoryTrace::iterator *getPrevRecord( MemoryTrace::iterator *it,
        KRecordList *displayList );

    virtual void setChilds() {}
    virtual KTrace *getWindowTrace() const;
    virtual TWindowLevel getWindowLevel() const;
    virtual Interval *getWindowInterval( TWindowLevel whichLevel,
                                         TObjectOrder whichOrder );
    virtual bool IsDerivedWindow() const;
    virtual TWindowLevel getComposeLevel( TWindowLevel whichLevel ) const;

};


#endif // INTERVALCPU_H_INCLUDED
