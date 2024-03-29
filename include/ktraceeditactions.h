/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
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


#include <fstream>

#include "traceeditactions.h"
#include "memorytrace.h"
#include "tracebodyiofactory.h"
#include "utils/traceparser/tracebodyio_v1.h"

class TraceToRecordAction: public TraceEditAction
{
public:
  TraceToRecordAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence )
  {}
  ~TraceToRecordAction()
  {}

  virtual TTraceEditActionType getType() const override
  {
    return TTraceEditActionType::TraceToRecord;
  }

  virtual bool execute( std::string whichTrace ) = 0;

protected:

private:

};


class RecordToTraceAction: public TraceEditAction
{
public:
  RecordToTraceAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence )
  {}
  ~RecordToTraceAction()
  {}

  virtual TTraceEditActionType getType() const override
  {
    return TTraceEditActionType::RecordToTrace;
  }

  virtual bool execute( MemoryTrace::iterator *whichRecord ) = 0;

protected:

private:

};

class RecordToRecordAction: public TraceEditAction
{
public:
  RecordToRecordAction( TraceEditSequence *whichSequence ) : TraceEditAction( whichSequence )
  {}
  ~RecordToRecordAction()
  {}

  virtual TTraceEditActionType getType() const override
  {
    return TTraceEditActionType::RecordToRecord;
  }

  virtual bool execute( MemoryTrace::iterator *whichRecord ) = 0;

protected:

private:

};


/****************************************************************************
 ********                  TestAction                                ********
 ****************************************************************************/
class TestAction: public TraceToTraceAction
{
  public:
    TestAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~TestAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********                  TraceCutterAction                         ********
 ****************************************************************************/
class TraceCutterAction: public TraceToTraceAction
{
  public:
    TraceCutterAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~TraceCutterAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********                  TraceFilterAction                         ********
 ****************************************************************************/
class TraceFilterAction: public TraceToTraceAction
{
  public:
    TraceFilterAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~TraceFilterAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********                  CSVOutputAction                           ********
 ****************************************************************************/
class CSVOutputAction: public TraceToTraceAction
{
  public:
    CSVOutputAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~CSVOutputAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};


/****************************************************************************
 ********            TraceShifterTimesLoaderAction                   ********
 ****************************************************************************/
/*
class TraceShifterTimesLoaderAction: public TraceToTraceAction
{
  public:
    TraceShifterTimesLoaderAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~TraceShifterTimesLoaderAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};
*/

/****************************************************************************
 ********                  TraceParserAction                         ********
 ****************************************************************************/
class TraceParserAction: public TraceToRecordAction
{
  public:
    TraceParserAction( TraceEditSequence *whichSequence ) : TraceToRecordAction( whichSequence )
    {}
    ~TraceParserAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:
};


/****************************************************************************
 ********                 RecordTimeShifterAction                    ********
 ****************************************************************************/
class RecordTimeShifterAction: public RecordToRecordAction
{
  public:
    RecordTimeShifterAction( TraceEditSequence *whichSequence ) :
      RecordToRecordAction( whichSequence ), availableShiftTime( false ), checkedAvailableShiftTime( false ),
      objects( TObjectOrder(0) ), count( 0 )
    {}
    ~RecordTimeShifterAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( MemoryTrace::iterator *whichRecord ) override;

  protected:

  private:
    bool availableShiftTime;
    bool checkedAvailableShiftTime;
    TObjectOrder objects;
    int count;

};


/****************************************************************************
 ********                  TraceWriterAction                         ********
 ****************************************************************************/
class TraceWriterAction: public RecordToTraceAction
{
  public:
    TraceWriterAction( TraceEditSequence *whichSequence ) : RecordToTraceAction( whichSequence )
    {}
    ~TraceWriterAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( MemoryTrace::iterator *whichRecord ) override;

  protected:

  private:
    std::fstream outputTrace;
    TraceBodyIO_v1< PARAM_TRACEBODY_CLASS > body;
};


/****************************************************************************
 ********               EventDrivenCutterAction                      ********
 ****************************************************************************/
class EventDrivenCutterAction: public RecordToTraceAction
{
  public:
    EventDrivenCutterAction( TraceEditSequence *whichSequence ) : RecordToTraceAction( whichSequence )
    {}
    ~EventDrivenCutterAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( MemoryTrace::iterator *whichRecord ) override;

  protected:

  private:
    vector<std::fstream *> outputTraces;
    vector<PRV_UINT32> currentThreadFile;
    map<PRV_INT32, TObjectOrder> countThreadsPerFile;
    TraceBodyIO_v1< PARAM_TRACEBODY_CLASS > body;
};

//
// Sort could be done in two ways:
//   - Calling an external program/script to perform trace-to-trace sort
//   - Internally, receiving every record and sorting in a record-to-trace object
//
// This TraceSortAction is the first one.
/****************************************************************************
 ********                  TraceSortAction                           ********
 ****************************************************************************/
class TraceSortAction: public TraceToTraceAction
{
  public:
    TraceSortAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~TraceSortAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};

/****************************************************************************
 ********                  PCFEventMergerAction                      ********
 ****************************************************************************/
class PCFEventMergerAction: public TraceToTraceAction
{
  public:
    PCFEventMergerAction( TraceEditSequence *whichSequence ) : TraceToTraceAction( whichSequence )
    {}
    ~PCFEventMergerAction()
    {}

    virtual vector<TSequenceStates> getStateDependencies() const override;

    virtual bool execute( std::string whichTrace ) override;

  protected:

  private:

};




