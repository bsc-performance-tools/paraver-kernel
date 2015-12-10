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

#include "traceeditactions.h"
#include "eventtranslator.h"
#include "pcfparser/libtools/UIParaverTraceConfig.h"


/****************************************************************************
 ********                  PCFEventMergerAction                      ********
 ****************************************************************************/

vector<TraceEditSequence::TSequenceStates> PCFEventMergerAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  tmpStates.push_back( TraceEditSequence::eventTranslationTableState );
  tmpStates.push_back( TraceEditSequence::outputTraceFileNameState );
  tmpStates.push_back( TraceEditSequence::outputDirSuffixState );
  tmpStates.push_back( TraceEditSequence::pcfMergerReferenceState );

  return tmpStates;
}


bool PCFEventMergerAction::execute( std::string whichTrace )
{
  TraceEditSequence *tmpSequence = mySequence;

  std::string newName = ( (OutputTraceFileNameState *)tmpSequence->getState( TraceEditSequence::outputTraceFileNameState ) )->getData();
  if ( !tmpSequence->isEndOfSequence() || newName.empty() )
  {
    std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TraceEditSequence::outputDirSuffixState ) )->getData();
    std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                           mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;

    vector< std::string > tmpID;
    tmpID.push_back( EventTranslator::getID() );

    newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );
  }


  std::map< TTypeValuePair, TTypeValuePair > tmpTrans;
  std::string referenceTrace = ( (PCFMergerReferenceState *)tmpSequence->getState( TraceEditSequence::pcfMergerReferenceState ) )->getData();
  referenceTrace = LocalKernel::composeName( referenceTrace, "pcf" );
  std::string sourceTrace = LocalKernel::composeName( whichTrace, "pcf" );
  UIParaverTraceConfig *tmpReferenceConfig = new UIParaverTraceConfig();
  UIParaverTraceConfig *tmpSourceConfig = new UIParaverTraceConfig();

  tmpReferenceConfig->parse( referenceTrace );
  tmpSourceConfig->parse( sourceTrace );



/*
  tmpTrans[ TTypeValuePair( 50000003, 31 ) ] = TTypeValuePair( 50000003, 51 );
*/

  ( (EventTranslationTableState *)tmpSequence->getState( TraceEditSequence::eventTranslationTableState ) )->setData( tmpTrans );

  delete tmpReferenceConfig;
  delete tmpSourceConfig;

  if ( !tmpTrans.empty() )
  {
    mySequence->getKernelConnection()->copyROW( whichTrace, newName );
  }

  tmpSequence->executeNextAction( whichTrace );

  return true;
}
