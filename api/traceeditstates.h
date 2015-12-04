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

#ifndef TRACEEDITSTATES_H_INCLUDED
#define TRACEEDITSTATES_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"
#include "textoutput.h"

class TraceEditSequence;
class TraceOptions;
class Window;

template< bool v > struct BoolToType;

template< class SeqT >
class BaseTraceEditState
{
  public:
    BaseTraceEditState();
    BaseTraceEditState( SeqT whichSequence );
    ~BaseTraceEditState();
    void init();

  protected:
    SeqT mySequence;
};


template< class SeqT, class DataT >
class DerivedTraceEditState : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditState( SeqT whichSequence );
    ~DerivedTraceEditState();
    void init();

    DataT getData() const;
    void setData( DataT whichData );

  private:
    DataT myData;

    void firstInit();
    void firstInit( BoolToType< true > );
    void firstInit( BoolToType< false > );

    void init( BoolToType< true > );
    void init( BoolToType< false > );
};


template< class SeqT >
class DerivedTraceEditStateBool : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateBool( SeqT whichSequence );
    ~DerivedTraceEditStateBool();
    void init();

    bool getData() const;
    void setData( bool whichData );

  private:
    bool myData;
};


template< class SeqT >
class DerivedTraceEditStateInt : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateInt( SeqT whichSequence );
    ~DerivedTraceEditStateInt();
    void init();

    int getData() const;
    void setData( int whichData );

  private:
    int myData;
};


template< class SeqT >
class DerivedTraceEditStateString : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateString( SeqT whichSequence );
    ~DerivedTraceEditStateString();
    void init();

    std::string getData() const;
    void setData( std::string whichData );

  private:
    std::string myData;
};


template< class SeqT >
class DerivedTraceEditStateTTime : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateTTime( SeqT whichSequence );
    ~DerivedTraceEditStateTTime();
    void init();

    TTime getData() const;
    void setData( TTime whichData );

  private:
    TTime myData;
};


template< class SeqT >
class DerivedTraceEditStateTWindowLevel : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateTWindowLevel( SeqT whichSequence );
    ~DerivedTraceEditStateTWindowLevel();
    void init();

    TWindowLevel getData() const;
    void setData( TWindowLevel whichData );

  private:
    TWindowLevel myData;
};

template< class SeqT >
class DerivedTraceEditStateTEventType : public BaseTraceEditState< SeqT >
{
  public:
    DerivedTraceEditStateTEventType( SeqT whichSequence );
    ~DerivedTraceEditStateTEventType();
    void init();

    TEventType getData() const;
    void setData( TEventType whichData );

  private:
    TEventType myData;
};

#include "traceeditstates.cpp"

typedef BaseTraceEditState< TraceEditSequence * > TraceEditState;

typedef DerivedTraceEditState< TraceEditSequence *, TextOutput > CSVOutputState;
typedef DerivedTraceEditState< TraceEditSequence *, TraceOptions * > TraceOptionsState;
typedef DerivedTraceEditState< TraceEditSequence *, Window * > CSVWindowState;
typedef DerivedTraceEditState< TraceEditSequence *, std::vector< TTime > * > ShiftTimesState;
typedef DerivedTraceEditState< TraceEditSequence *, std::map< TTypeValuePair, TTypeValuePair > EventTranslationTableState;

typedef DerivedTraceEditStateBool< TraceEditSequence * > EOFParsedState;
typedef DerivedTraceEditStateBool< TraceEditSequence * > CopyAdditionalFilesState;

typedef DerivedTraceEditStateInt< TraceEditSequence * > TestState;

typedef DerivedTraceEditStateString< TraceEditSequence * > CSVFileNameState;
typedef DerivedTraceEditStateString< TraceEditSequence * > OutputDirSuffixState;
typedef DerivedTraceEditStateString<TraceEditSequence * > OutputTraceFileNameState;
typedef DerivedTraceEditStateString<TraceEditSequence * > PCFMergerReferenceState;

typedef DerivedTraceEditStateTTime< TraceEditSequence * > MaxTraceTimeState;
typedef DerivedTraceEditStateTWindowLevel< TraceEditSequence * > ShiftLevelState;
typedef DerivedTraceEditStateTEventType< TraceEditSequence * > OnEventCutter;

#endif // TRACEEDITSTATES_H_INCLUDED
