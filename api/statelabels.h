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


#ifndef STATELABELS_H_INCLUDED
#define STATELABELS_H_INCLUDED

#include <map>
#include "paraverkerneltypes.h"

#ifdef OLD_PCFPARSER
#include "utils/pcfparser/old/ParaverTraceConfig.h"
#else
#include "utils/pcfparser/UIParaverTraceConfig.h"
#endif

using namespace libparaver;

class StateLabels
{
  public:
    static const std::string unknownLabel;

    StateLabels();
#ifdef OLD_PCFPARSER
    StateLabels( const ParaverTraceConfig& config );
#else
    StateLabels( const UIParaverTraceConfig& config );
#endif
    ~StateLabels();

    void getStates( std::vector<TState>& onVector ) const;
    bool getStateLabel( TState state, std::string& onStr ) const;

  protected:

  private:
    std::map<TState, std::string> stateLabel;
};


#endif // STATELABELS_H_INCLUDED
