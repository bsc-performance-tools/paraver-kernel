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


#include "traceeditsequence.h"
#include "ktrace.h"

class KTrace;

class KTraceEditSequence:public TraceEditSequence
{
  public:
    KTraceEditSequence() {}
    KTraceEditSequence( const KernelConnection *whichKernel );
    virtual ~KTraceEditSequence();

    const KernelConnection *getKernelConnection() const override;
    
    TraceEditState *createState( TSequenceStates whichState ) override;
    void setCurrentTrace( KTrace *whichTrace );
    KTrace *getCurrentTrace();

    bool addState( TSequenceStates whichState ) override;
    bool addState( TSequenceStates whichState, TraceEditState *newState ) override;
    TraceEditState *getState( TSequenceStates whichState ) override;
    bool pushbackAction( TSequenceActions whichAction ) override;
    bool pushbackAction( TraceEditAction *newAction ) override;

    bool execute( vector<std::string> traces ) override;

    bool executeNextAction( std::string whichTrace ) override;
    bool executeNextAction( MemoryTrace::iterator *whichRecord );

    bool isEndOfSequence() const override;

  protected:

  private:
    map<TSequenceStates, TraceEditState *> activeStates;
    vector<TraceEditAction *> sequenceActions;
    KTrace *currentTrace;
    std::string currentTraceName;
    map< std::string, bool > sequenceExecError;

    PRV_UINT16 currentAction;

    const KernelConnection *myKernel;

};



