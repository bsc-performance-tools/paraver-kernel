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

#include <cstdint>

using TTraceSize = uint64_t;

#ifdef EXTENDED_OBJECTS_ENABLED
using TObjectOrder = uint32_t;
#else
using TObjectOrder = uint16_t;
#endif

using TThreadOrder = TObjectOrder;
using TTaskOrder   = TObjectOrder;
using TApplOrder   = TObjectOrder;

using TNodeOrder   = TObjectOrder;
using TCPUOrder    = TObjectOrder;

using TSemanticValue = double;

using TTime       = double;
using TRecordTime = TTime;
using TTimeUnit   = uint16_t;
/*
 *  TTimeUnit defines
 */
constexpr TTimeUnit NS   = 0;
constexpr TTimeUnit US   = 1;
constexpr TTimeUnit MS   = 2;
constexpr TTimeUnit SEC  = 3;
constexpr TTimeUnit MIN  = 4;
constexpr TTimeUnit HOUR = 5;
constexpr TTimeUnit DAY  = 6;

using TRecordType = uint16_t;
/*
 * TRecordType defines.
 */
constexpr TRecordType BEGIN    = 0x0001;
constexpr TRecordType END      = 0x0002;
constexpr TRecordType STATE    = 0x0004;
constexpr TRecordType EVENT    = 0x0008;
constexpr TRecordType LOG      = 0x0010;
constexpr TRecordType PHY      = 0x0020;
constexpr TRecordType SEND     = 0x0040;
constexpr TRecordType RECV     = 0x0080;
constexpr TRecordType COMM     = 0x0100;
constexpr TRecordType GLOBCOMM = 0x0200;
constexpr TRecordType RRECV    = 0x0400;
constexpr TRecordType RSEND    = 0x0800;

constexpr TRecordType EMPTYREC = STATE + EVENT + COMM;

using TCommID     = uint64_t;
using TCommSize   = int64_t;
using TCommTag    = int64_t;
using TEventType  = uint32_t;
using TEventValue = int64_t;
using TState      = uint32_t;

enum class TTraceLevel
{
  NONE = 0,
  WORKLOAD, APPLICATION, TASK, THREAD,
  SYSTEM, NODE, CPU
};

constexpr TTraceLevel& operator++( TTraceLevel& whichLevel )
{
  return whichLevel = static_cast<TTraceLevel>( static_cast<size_t>( whichLevel ) + 1 );
}

constexpr TTraceLevel operator++( TTraceLevel& whichLevel, int )
{
  TTraceLevel tmp = whichLevel;
  whichLevel = static_cast<TTraceLevel>( static_cast<size_t>( whichLevel ) + 1 );
  return tmp;
}
