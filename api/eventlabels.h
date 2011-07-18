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

#ifndef EVENTLABELS_H_INCLUDED
#define EVENTLABELS_H_INCLUDED

#include <set>
#include <map>
#include "paraverkerneltypes.h"
#include "pcfparser/ParaverTraceConfig.h"

using namespace libparaver;

class EventLabels
{
  public:
    static const string unknownLabel;

    EventLabels();
    EventLabels( const set<TEventType>& eventsLoaded );
    EventLabels( const ParaverTraceConfig& config,
                 const set<TEventType>& eventsLoaded );
    ~EventLabels();

    void getTypes( vector<TEventType>& onVector ) const;
    bool getEventTypeLabel( TEventType type, string& onStr ) const;
    bool getEventValueLabel( TEventType type, TEventValue value, string& onStr ) const;
    bool getEventValueLabel( TEventValue value, string& onStr ) const;
    bool getValues( TEventType type, vector<string> &values ) const;
    bool getValues( TEventType type, map<TEventValue, string> &values ) const;

  protected:

  private:
    map<TEventType, string> eventTypeLabel;
    map<TEventType, map<TEventValue, string> > eventValueLabel;
};


#endif // EVENTLABELS_H_INCLUDED
