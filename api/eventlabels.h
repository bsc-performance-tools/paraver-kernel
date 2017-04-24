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
#include <ext/hash_map>
#include "paraverkerneltypes.h"

#ifdef OLD_PCFPARSER
#include "utils/pcfparser/old/ParaverTraceConfig.h"
#else
#include "utils/pcfparser/UIParaverTraceConfig.h"
#endif

using namespace libparaver;
using namespace __gnu_cxx;

class EventLabels
{
  public:
    static const std::string unknownLabel;

    EventLabels();
    EventLabels( const std::set<TEventType>& eventsLoaded );
#ifdef OLD_PCFPARSER
    EventLabels( const ParaverTraceConfig& config,
                 const std::set<TEventType>& eventsLoaded );
#else
    EventLabels( const UIParaverTraceConfig& config,
                 const std::set<TEventType>& eventsLoaded );
#endif
    ~EventLabels();

    void getTypes( std::vector<TEventType>& onVector ) const;
    bool getEventTypeLabel( TEventType type, std::string& onStr ) const;
    bool getEventValueLabel( TEventType type, TEventValue value, std::string& onStr ) const;
    bool getEventValueLabel( TEventValue value, std::string& onStr ) const;
    bool getValues( TEventType type, std::vector<std::string> &values ) const;
    bool getValues( TEventType type, std::map<TEventValue, std::string> &values ) const;

    bool getEventType( const std::string& whichTypeLabel, TEventType& onType ) const;
    bool getEventValue( const std::string& whichValueLabel, std::multimap< TEventType, TEventValue >& onMap ) const;

  protected:

  private:
    std::map<TEventType, std::string> eventType2Label;
    std::map<TEventType, std::map<TEventValue, std::string> > eventValue2Label;

    hash_map<const char *, TEventType, hash<const char *> > label2eventType;
    hash_map<const char *, std::multimap< TEventType, TEventValue >, hash<const char *> > label2eventValue;
};


#endif // EVENTLABELS_H_INCLUDED
