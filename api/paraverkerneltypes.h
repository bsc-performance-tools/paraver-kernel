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


#include <stddef.h>
#include <vector>
#include <string>
#include "prvtypes.h"
#include "config_traits.h"
#include "utils/traceparser/tracetypes.h"

static const TRecordTime factorTable[ DAY + 1 ] =
{
  1,   // ns
  1E3, // us
  1E3, // ms
  1E3, // seconds
  60,  // minutes
  60,  // hours
  24   // days
};


enum TWindowLevel
{
  NONE = static_cast<int>( TTraceLevel::NONE ),
  WORKLOAD = static_cast<int>( TTraceLevel::WORKLOAD ),
  APPLICATION = static_cast<int>( TTraceLevel::APPLICATION ),
  TASK = static_cast<int>( TTraceLevel::TASK ),
  THREAD = static_cast<int>( TTraceLevel::THREAD ),
  SYSTEM = static_cast<int>( TTraceLevel::SYSTEM ),
  NODE = static_cast<int>( TTraceLevel::NODE ),
  CPU = static_cast<int>( TTraceLevel::CPU ),
  // Compose levels (for setting the window functions only)
  TOPCOMPOSE1, TOPCOMPOSE2, COMPOSEWORKLOAD, COMPOSEAPPLICATION, COMPOSETASK,
  COMPOSETHREAD, COMPOSESYSTEM, COMPOSENODE, COMPOSECPU,
  // Derived window levels
  DERIVED,
  EXTRATOPCOMPOSE1
};

// Filter definitions
typedef PRV_UINT16 TFilterNumParam;

// Semantic function definitions
typedef PRV_UINT16 TParamIndex;
typedef std::vector<double> TParamValue;

// Semantic record list definitions
typedef PRV_UINT16 TCreateList;
static const TCreateList NOCREATE = 0x00;
static const TCreateList CREATEEVENTS = 0x01;
static const TCreateList CREATECOMMS = 0x02;

// Histogram definitions
typedef double     THistogramLimit;
typedef PRV_UINT32 THistogramColumn;

enum THistoTotals
{
  TOTAL = 0, AVERAGE, MAXIMUM, MINIMUM, STDEV, AVGDIVMAX, NUMTOTALS
};

enum SemanticInfoType
{
  NO_TYPE = 0, // without units
  SAME_TYPE, // same type as previous function
  OBJECT_TYPE, // window same level object
  APPL_TYPE,
  TASK_TYPE,
  THREAD_TYPE,
  NODE_TYPE,
  CPU_TYPE,
  TIME_TYPE,
  STATE_TYPE,
  EVENTTYPE_TYPE,
  EVENTVALUE_TYPE,
  COMMSIZE_TYPE,
  COMMTAG_TYPE,
  BANDWIDTH_TYPE
};

enum TSemanticGroup
{
  COMPOSE_GROUP = 0,
  DERIVED_GROUP, CPU_GROUP, NOTTHREAD_GROUP, STATE_GROUP,
  EVENT_GROUP, COMM_GROUP, OBJECT_GROUP
};

static const std::string        GZIPPED_PRV_SUFFIX = ".prv.gz";
static const std::string                PRV_SUFFIX = ".prv";
static const std::string                CFG_SUFFIX = ".cfg";
static const std::string        DIMEMAS_CFG_SUFFIX = ".cfg";
static const std::string                PCF_SUFFIX = ".pcf";
static const std::string                ROW_SUFFIX = ".row";
static const std::string TRACE_TOOL_OPTIONS_SUFFIX = ".xml";
static const std::string               OTF2_SUFFIX = ".otf2";

static const std::string FILTER_SEP = ".";

static const std::string  BMP_SUFFIX = ".bmp";
static const std::string JPEG_SUFFIX = ".jpg";
static const std::string  PNG_SUFFIX = ".png";
static const std::string  XPM_SUFFIX = ".xpm";

/*
 * PARAVER OLD CODE
 * Record class field defines for a state record.
 */

constexpr TState IDLE = 0;
constexpr TState RUNNING = 1;
