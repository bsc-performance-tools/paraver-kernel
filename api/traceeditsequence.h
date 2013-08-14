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

#ifndef TRACEEDITSEQUENCE_H_INCLUDED
#define TRACEEDITSEQUENCE_H_INCLUDED

#include <map>
#include <vector>
#include <string>
#include "paraverkerneltypes.h"
#include "memorytrace.h"

using std::map;
using std::vector;

class TraceEditAction;
class TraceEditState;


class TraceEditSequence
{
  public:

    enum TSequenceStates
    {
      testState = 0,
      numStates
    };

    TraceEditSequence();
    ~TraceEditSequence();

    TraceEditState *createState( TraceEditSequence::TSequenceStates whichState );

    bool addState( TraceEditSequence::TSequenceStates whichState );
    bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState );
    TraceEditState *getState( TraceEditSequence::TSequenceStates whichState );
    bool pushbackAction( TraceEditAction *newAction );

    void execute( vector<std::string> traces );

    void executeNextAction( std::string whichTrace );
    void executeNextAction( MemoryTrace::iterator * );

  protected:

  private:
    map<TraceEditSequence::TSequenceStates, TraceEditState *> activeStates;
    vector<TraceEditAction *> sequenceActions;

    PRV_UINT16 currentAction;

};


#endif // TRACEEDITSEQUENCE_H_INCLUDED
