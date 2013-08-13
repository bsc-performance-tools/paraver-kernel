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

#ifndef TRACEEDITACTIONS_H_INCLUDED
#define TRACEEDITACTIONS_H_INCLUDED

#include <string>
#include <vector>
#include "memorytrace.h"
#include "traceeditsequence.h"

using std::vector;


class TraceEditAction
{
public:
  enum TTraceEditActionType
  {
    TraceToTrace = 0,
    TraceToRecord,
    RecordToTrace,
    RecordToRecord
  };

  TraceEditAction( TraceEditSequence *whichSequence ) : mySequence( whichSequence ) {}
  ~TraceEditAction() {}

  virtual TraceEditAction::TTraceEditActionType getType() const = 0;
  virtual vector<TraceEditSequence::TSequenceStates> getStateDependencies() const = 0;

protected:
  TraceEditSequence *mySequence;

private:

};

class TraceToTraceAction: public TraceEditAction
{
public:
  TraceToTraceAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence ) {}
  ~TraceToTraceAction() {}

  virtual TraceEditAction::TTraceEditActionType getType() const
  {
    return TraceEditAction::TraceToTrace;
  }

  virtual std::string execute( std::string whichTrace ) = 0;

protected:

private:

};

class TraceToRecordAction: public TraceEditAction
{
public:
  TraceToRecordAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence ) {}
  ~TraceToRecordAction() {}

  virtual TraceEditAction::TTraceEditActionType getType() const
  {
    return TraceEditAction::TraceToRecord;
  }

  virtual MemoryTrace::iterator *execute( std::string whichTrace ) = 0;

protected:

private:

};


class RecordToTraceAction: public TraceEditAction
{
public:
  RecordToTraceAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence ) {}
  ~RecordToTraceAction() {}

  virtual TraceEditAction::TTraceEditActionType getType() const
  {
    return TraceEditAction::RecordToTrace;
  }

  virtual std::string execute( MemoryTrace::iterator *whichRecord ) = 0;

protected:

private:

};

class RecordToRecordAction: public TraceEditAction
{
public:
  RecordToRecordAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence ) {}
  ~RecordToRecordAction() {}

  virtual TraceEditAction::TTraceEditActionType getType() const
  {
    return TraceEditAction::RecordToRecord;
  }

  virtual MemoryTrace::iterator *execute( MemoryTrace::iterator *whichRecord ) = 0;

protected:

private:

};

class testAction: public TraceToTraceAction
{
  public:
    testAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~testAction()
    {}

    virtual vector<TraceEditSequence::TSequenceStates> getStateDependencies() const;

    virtual std::string execute( std::string whichTrace );

  protected:

  private:

};

#endif // TRACEEDITACTIONS_H_INCLUDED
