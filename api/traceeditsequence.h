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

using std::map;
using std::vector;

class KernelConnection;
class TraceEditAction;
class TraceEditState;


class TraceEditSequence
{
  public:

    enum TSequenceStates
    {
      testState = 0,
      traceOptionsState,
      csvWindowState,
      csvFileNameState,
      csvOutputState,
      outputDirSuffixState,
      numStates
    };

    enum TSequenceActions
    {
      testAction = 0,
      traceCutterAction,
      csvOutputAction,
      numActions
    };

    static std::string dirNameClustering;
    static std::string dirNameFolding;

    static TraceEditSequence *create( KernelConnection *whichKernel );

    TraceEditSequence() { }
    TraceEditSequence( KernelConnection *whichKernel );
    virtual ~TraceEditSequence();

    KernelConnection *getKernelConnection() const;

    virtual TraceEditState *createState( TraceEditSequence::TSequenceStates whichState ) = 0;

    virtual bool addState( TraceEditSequence::TSequenceStates whichState ) = 0;
    virtual bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState ) = 0;
    virtual TraceEditState *getState( TraceEditSequence::TSequenceStates whichState ) = 0;
    virtual bool pushbackAction( TraceEditSequence::TSequenceActions whichAction ) = 0;
    virtual bool pushbackAction( TraceEditAction *newAction ) = 0;

    virtual void execute( vector<std::string> traces ) = 0;

    virtual void executeNextAction( std::string whichTrace ) = 0;

    virtual TraceEditSequence *getConcrete()
    {
      return NULL;
    }

  protected:
    KernelConnection *myKernel;

  private:

};


class TraceEditSequenceProxy:public TraceEditSequence
{
  public:
    TraceEditSequenceProxy();
    TraceEditSequenceProxy( KernelConnection *whichKernel );
    virtual ~TraceEditSequenceProxy();

    TraceEditState *createState( TraceEditSequence::TSequenceStates whichState );

    bool addState( TraceEditSequence::TSequenceStates whichState );
    bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState );
    TraceEditState *getState( TraceEditSequence::TSequenceStates whichState );
    bool pushbackAction( TraceEditSequence::TSequenceActions whichAction );
    bool pushbackAction( TraceEditAction *newAction );

    void execute( vector<std::string> traces );

    void executeNextAction( std::string whichTrace );

    TraceEditSequence *getConcrete();

  protected:

  private:
    TraceEditSequence *mySequence;

};

#endif // TRACEEDITSEQUENCE_H_INCLUDED
