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

#include "statelabels.h"
#include "../common-files/pcfparser/ParaverState.h"

const string StateLabels::unknownLabel = "Unknown";

StateLabels::StateLabels()
{}

StateLabels::StateLabels( const ParaverTraceConfig& config )
{
  const vector<ParaverState *>& states = config.get_states();
  for ( vector<ParaverState *>::const_iterator it = states.begin();
        it != states.end(); ++it )
    stateLabel[ ( *it )->get_key() ] = ( *it )->get_value();
}

StateLabels::~StateLabels()
{}

void StateLabels::getStates( vector<TState>& onVector ) const
{
  for ( map<TState, string>::const_iterator it = stateLabel.begin();
        it != stateLabel.end(); ++it )
    onVector.push_back( ( *it ).first );
}

bool StateLabels::getStateLabel( TState state, string& onStr ) const
{
  map<TState, string>::const_iterator it = stateLabel.find( state );
  if ( it == stateLabel.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *it ).second;
  return true;
}
