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

#include "tracetypes.h"

constexpr char LEVEL_WORKLOAD[] =     "WORKLOAD";
constexpr char LEVEL_APPLICATION[] =  "APPL";
constexpr char LEVEL_TASK[] =         "TASK";
constexpr char LEVEL_THREAD[] =       "THREAD";
constexpr char LEVEL_SYSTEM[] =       "SYSTEM";
constexpr char LEVEL_NODE[] =         "NODE";
constexpr char LEVEL_CPU[] =          "CPU";

static const std::string LABEL_LEVELS[ static_cast<int>( TTraceLevel::CPU ) + 1 ] =
{
  "NONE",
  LEVEL_WORKLOAD,
  LEVEL_APPLICATION,
  LEVEL_TASK,
  LEVEL_THREAD,
  LEVEL_SYSTEM,
  LEVEL_NODE,
  LEVEL_CPU
};
