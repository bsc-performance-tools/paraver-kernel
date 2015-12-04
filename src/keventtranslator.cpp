/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-kernel                           *
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

#include "keventtranslator.h"
#include "traceeditactions.h"
#include "traceoptions.h"

KEventTranslator::KEventTranslator( const KernelConnection *myKernel,
                                    std::string traceIn,
                                    std::string traceOut,
                                    std::string traceReference,
                                    ProgressController *progress )
{
  // Build sequence
  mySequence = TraceEditSequence::create( myKernel );
  mySequence->pushbackAction( new PCFEventMergerAction( mySequence ) );
  mySequence->pushbackAction( TraceEditSequence::traceFilterAction );

  // Add sequence states

  // Sequence state 1: OutputTraceFileNameState
  OutputTraceFileNameState *tmpOutputTraceFileNameState = new OutputTraceFileNameState( mySequence );
  tmpOutputTraceFileNameState->setData( traceOut );
  mySequence->addState( TraceEditSequence::outputTraceFileNameState, tmpOutputTraceFileNameState );

  // Sequence state 2: PCFMergerReferenceState
  PCFMergerReferenceState *tmpPCFMergerReference = new PCFMergerReferenceState( mySequence );
  tmpPCFMergerReference->setData( traceReference );
  mySequence->addState( TraceEditSequence::pcfMergerReferenceState, tmpPCFMergerReference );

  // Sequence state 3: TraceOptions
  TraceOptions *tmpOptions = TraceOptions::create( myKernel );

  // Put all the states
  tmpOptions->set_filter_states( true );
  tmpOptions->set_all_states( true );

  // Put all events
  // Negative filter! Discard type 6666666666  and put the rest...
  TraceOptions::TFilterTypes eventTypes;
  TraceOptions::allowed_types impossibleType;
  impossibleType.type = 6666666666;
  impossibleType.min_call_time = 0;
  impossibleType.max_type = 0;
  impossibleType.last_value = 0;
  eventTypes[ 0 ] = impossibleType;
  tmpOptions->set_filter_types( eventTypes );
  tmpOptions->set_discard_given_types( true );
  tmpOptions->set_filter_last_type( 1 );
  tmpOptions->set_filter_events( true );

  // Put all comms
  tmpOptions->set_filter_comms( true );
  tmpOptions->set_min_comm_size( 0 );

  TraceOptionsState *tmpOptionsState = new TraceOptionsState( mySequence );
  tmpOptionsState->setData( tmpOptions );
  mySequence->addState( TraceEditSequence::traceOptionsState, tmpOptionsState );

  traces.push_back( traceIn );
}


KEventTranslator::~KEventTranslator()
{
  delete mySequence;
}


void KEventTranslator::execute( std::string traceIn,
                                std::string traceOut,
                                ProgressController *progress )
{
  mySequence->execute( traces );
}
