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


#ifndef TRACEEDITACTIONS_H_INCLUDED
#define TRACEEDITACTIONS_H_INCLUDED

#include <string>
#include <vector>
#include "traceeditsequence.h"

using std::vector;


enum class TTraceEditActionType
{
  TraceToTrace = 0,
  TraceToRecord,
  RecordToTrace,
  RecordToRecord
};

class TraceEditAction
{
public:
  TraceEditAction( TraceEditSequence *whichSequence ) : mySequence( whichSequence ) {}
  virtual ~TraceEditAction() {}

  virtual TTraceEditActionType getType() const = 0;
  virtual vector<TSequenceStates> getStateDependencies() const = 0;

protected:
  TraceEditSequence *mySequence;

private:

};


class TraceToTraceAction: public TraceEditAction
{
public:
  TraceToTraceAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence ) {}
  virtual ~TraceToTraceAction() {}

  virtual TTraceEditActionType getType() const override
  {
    return TTraceEditActionType::TraceToTrace;
  }

  virtual bool execute( std::string whichTrace ) = 0;

protected:

private:

};




#endif // TRACEEDITACTIONS_H_INCLUDED
