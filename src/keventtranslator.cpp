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

KEventTranslator::KEventTranslator( const KernelConnection *myKernel,
                                    std::string traceIn,
                                    std::string traceOut,
                                    std::string traceReference,
                                    ProgressController *progress )
{
  // Build sequence
  mySequence = TraceEditSequence::create( myKernel );

  mySequence->pushbackAction( TraceEditSequence::pcfEventMergerAction );
  mySequence->pushbackAction( TraceEditSequence::traceFilterAction );

  OutputTraceFileNameState *tmpOutputTraceFileNameState = new OutputTraceFileNameState( mySequence );
  tmpOutputTraceFileNameState->setData( traceOut );
  mySequence->addState( TraceEditSequence::outputTraceFileNameState, tmpOutputTraceFileNameState );

  PCFMergerReferenceState *tmpPCFMergerReference = new PCFMergerReferenceState( mySequence );
  tmpPCFMergerReference->setData( traceReference );
  mySequence->addState( TraceEditSequence::pcfMergerReferenceState, tmpPCFMergerReference );

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
