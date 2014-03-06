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

#include <iostream>
#include "traceeditsequence.h"
#include "traceeditactions.h"
#include "traceeditstates.h"
#include "localkernel.h"

std::string TraceEditSequence::dirNameClustering( "clustering" );
std::string TraceEditSequence::dirNameFolding( "folding" );
std::string TraceEditSequence::dirNameDimemas( "dimemas" );

TraceEditSequence *TraceEditSequence::create( const KernelConnection *whichKernel )
{
  return new TraceEditSequenceProxy( whichKernel );
}


TraceEditSequence::TraceEditSequence( const KernelConnection *whichKernel )
  : myKernel( whichKernel )
{
}


TraceEditSequence::~TraceEditSequence()
{
}


const KernelConnection *TraceEditSequence::getKernelConnection() const
{
  return myKernel;
}


TraceEditSequenceProxy::TraceEditSequenceProxy()
{
}


TraceEditSequenceProxy::TraceEditSequenceProxy( const KernelConnection *whichKernel )
 : TraceEditSequence( whichKernel )
{
  mySequence = myKernel->newTraceEditSequence();
}


TraceEditSequenceProxy::~TraceEditSequenceProxy()
{
  delete mySequence;
}


TraceEditState *TraceEditSequenceProxy::createState( TraceEditSequence::TSequenceStates whichState )
{
  return mySequence->createState( whichState );
}


bool TraceEditSequenceProxy::addState( TraceEditSequence::TSequenceStates whichState )
{
  return mySequence->addState( whichState );
}


bool TraceEditSequenceProxy::addState( TraceEditSequence::TSequenceStates whichState, TraceEditState *newState )
{
  return mySequence->addState( whichState, newState );
}


TraceEditState *TraceEditSequenceProxy::getState( TraceEditSequence::TSequenceStates whichState )
{
  return mySequence->getState( whichState );
}


bool TraceEditSequenceProxy::pushbackAction( TraceEditSequence::TSequenceActions whichAction )
{
  return mySequence->pushbackAction( whichAction );
}


bool TraceEditSequenceProxy::pushbackAction( TraceEditAction *newAction )
{
  return mySequence->pushbackAction( newAction );
}


void TraceEditSequenceProxy::execute( vector<std::string> traces )
{
  mySequence->execute( traces );
}

TraceEditSequence *TraceEditSequenceProxy::getConcrete()
{
  return mySequence;
}

void TraceEditSequenceProxy::executeNextAction( std::string whichTrace )
{
  mySequence->executeNextAction( whichTrace );
}
