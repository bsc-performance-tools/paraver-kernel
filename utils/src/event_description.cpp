/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                            ptools_common_files                            *
 *                   Performance tools common files library                  *
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

#include "event_description.h"

EventDescription::EventDescription()
{}

EventDescription::EventDescription( bool            usedInExtrae,
                                    prvEventType_t  whichType,
                                    prvEventValue_t whichValue,
                                    string          whichStrType,
                                    string          whichStrValue,
                                    bool            isChangingState,
                                    prvState_t      whichStateTransition
                                  ):
                                    inExtrae( usedInExtrae ),
                                    type( whichType ),
                                    value( whichValue ),
                                    strType( whichStrType ),
                                    strValue( whichStrValue ),
                                    changeState( isChangingState ),
                                    stateTransition( whichStateTransition ),
                                    used( false )
{}

EventDescription::~EventDescription()
{}

bool EventDescription::getUsedInExtrae() const
{
  return inExtrae;
}

prvEventType_t EventDescription::getType() const
{
  return type;
}

prvEventValue_t EventDescription::getValue() const
{
  return value;
}

string EventDescription::getStrType() const
{
  return strType;
}

string EventDescription::getStrValue() const
{
  return strValue;
}

bool EventDescription::getChangeState() const
{
  return changeState;
}

prvState_t EventDescription::getStateTransition() const
{
  return stateTransition;
}

bool EventDescription::getUsed() const
{
  return used;
}

void EventDescription::setUsed( bool newValue )
{
  used = newValue;
}
