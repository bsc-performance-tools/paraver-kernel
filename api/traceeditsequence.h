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


#ifndef TRACEEDITSEQUENCE_H_INCLUDED
#define TRACEEDITSEQUENCE_H_INCLUDED

#include <map>
#include <vector>
#include <string>
#include "paraverkerneltypes.h"
#include "traceeditstates.h"

using std::map;
using std::vector;

class KernelConnection;
class TraceEditAction;
//class TraceEditState;


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
      outputTraceFileNameState,
      maxTraceTimeState,
      shiftTimesState,
      eofParsedState,
      shiftLevelState,
      onEventCutterState,
      pcfMergerReferenceState,
      eventTranslationTableState,
      copyAdditionalFilesState,
      onlyFilterState,
      numStates
    };

    enum TSequenceActions
    {
      testAction = 0,
      traceCutterAction,
      traceFilterAction,
      csvOutputAction,
      traceParserAction,
      recordTimeShifterAction,
      traceWriterAction,
      eventDrivenCutterAction,
      traceSortAction,
      numActions
    };

    static std::string dirNameClustering;
    static std::string dirNameFolding;
    static std::string dirNameDimemas;
    static std::string dirNameSpectral;

    static TraceEditSequence *create( const KernelConnection *whichKernel );

    TraceEditSequence() {}
    TraceEditSequence( const KernelConnection *whichKernel ) {}
    virtual ~TraceEditSequence() {}

    virtual const KernelConnection *getKernelConnection() const = 0;

    virtual TraceEditState *createState( TraceEditSequence::TSequenceStates whichState ) = 0;

    virtual bool addState( TraceEditSequence::TSequenceStates whichState ) = 0;
    virtual bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState ) = 0;
    virtual TraceEditState *getState( TraceEditSequence::TSequenceStates whichState ) = 0;
    virtual bool pushbackAction( TraceEditSequence::TSequenceActions whichAction ) = 0;
    virtual bool pushbackAction( TraceEditAction *newAction ) = 0;

    virtual bool execute( vector<std::string> traces ) = 0;

    virtual bool executeNextAction( std::string whichTrace ) = 0;

    virtual bool isEndOfSequence() const = 0;

    virtual TraceEditSequence *getConcrete()
    {
      return NULL;
    }

  protected:

  private:

};


class TraceEditSequenceProxy:public TraceEditSequence
{
  public:
    TraceEditSequenceProxy() {}
    TraceEditSequenceProxy( const KernelConnection *whichKernel );
    virtual ~TraceEditSequenceProxy();

    const KernelConnection *getKernelConnection() const;

    TraceEditState *createState( TraceEditSequence::TSequenceStates whichState );

    bool addState( TraceEditSequence::TSequenceStates whichState );
    bool addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState );
    TraceEditState *getState( TraceEditSequence::TSequenceStates whichState );
    bool pushbackAction( TraceEditSequence::TSequenceActions whichAction );
    bool pushbackAction( TraceEditAction *newAction );

    bool execute( vector<std::string> traces );

    bool executeNextAction( std::string whichTrace );

    bool isEndOfSequence() const;

    TraceEditSequence *getConcrete();

  protected:

  private:
    TraceEditSequence *mySequence;

};

#endif // TRACEEDITSEQUENCE_H_INCLUDED
