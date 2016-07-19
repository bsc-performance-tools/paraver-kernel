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

#ifndef EVENT_DESCRIPTION_H_INCLUDED
#define EVENT_DESCRIPTION_H_INCLUDED

#include <string>
#include "ptools_prv_types.h"

using std::string;

class EventDescription
{
  public:
    EventDescription();

    EventDescription( bool            usedInExtrae,
                      prvEventType_t  whichType,
                      prvEventValue_t whichValue,
                      string          whichStrType,
                      string          whichStrValue,
                      bool            isChangingState,
                      prvState_t      whichStateTransition
                    );

    ~EventDescription();

    bool            getUsedInExtrae() const;
    prvEventType_t  getType() const;
    prvEventValue_t getValue() const;
    string          getStrType() const;
    string          getStrValue() const;
    bool            getChangeState() const;
    prvState_t      getStateTransition() const;
    bool            getUsed() const;
    void            setUsed( bool newValue );

  protected:
    bool            inExtrae;
    prvEventType_t  type;
    prvEventValue_t value;
    string          strType;
    string          strValue;
    bool            changeState;
    prvState_t      stateTransition;
    bool            used;

  private:

};


#endif // EVENT_DESCRIPTION_H_INCLUDED
