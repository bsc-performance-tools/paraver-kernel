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


#pragma once

#include <set>
#include <map>
#include "paraverkerneltypes.h"
#include "utils/traceparser/pcffileparser.h"

class EventLabels
{
  public:
    static const std::string unknownLabel;

    EventLabels();
    EventLabels( const PCFFileParser<>& pcfParser );
    ~EventLabels();

    void getTypes( std::vector<TEventType>& onVector ) const;
    bool getEventTypeLabel( TEventType type, std::string& onStr ) const;
    bool getEventValueLabel( TEventType type, TEventValue value, std::string& onStr ) const;
    bool getEventValueLabel( TEventValue value, std::string& onStr ) const;
    bool getValues( TEventType type, std::vector<std::string> &values ) const;
    bool getValues( TEventType type, std::map<TEventValue, std::string> &values ) const;

    bool getEventType( const std::string& whichTypeLabel, std::vector<TEventType>& onVector ) const;
    bool getEventValue( const std::string& whichValueLabel, std::multimap< TEventType, TEventValue >& onMap ) const;

  protected:

  private:
    std::map<TEventType, std::string> eventType2Label;
    std::map<TEventType, std::map<TEventValue, std::string> > eventValue2Label;

    std::map<std::string, TEventType> label2eventType;
    std::map<std::string, std::multimap< TEventType, TEventValue > > label2eventValue;
};



