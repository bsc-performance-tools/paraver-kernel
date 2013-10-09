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

#ifndef KTRACEEDITSEQUENCE_H_INCLUDED
#define KTRACEEDITSEQUENCE_H_INCLUDED

#include "traceeditsequence.h"

class KTraceEditSequence:public TraceEditSequence
{
  public:
    KTraceEditSequence();
    virtual ~KTraceEditSequence();

    TraceEditState *createState( TraceEditSequence::TSequenceStates whichState );

    bool addState( TraceEditSequence::TSequenceStates whichState );
    bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState );
    TraceEditState *getState( TraceEditSequence::TSequenceStates whichState );
    bool pushbackAction( TraceEditSequence::TSequenceActions whichAction );
    bool pushbackAction( TraceEditAction *newAction );

    void execute( vector<std::string> traces );

    void executeNextAction( std::string whichTrace );
    void executeNextAction( MemoryTrace::iterator *whichRecord );

  protected:

  private:
    map<TraceEditSequence::TSequenceStates, TraceEditState *> activeStates;
    vector<TraceEditAction *> sequenceActions;

    PRV_UINT16 currentAction;

};


#endif // KTRACEEDITSEQUENCE_H_INCLUDED
