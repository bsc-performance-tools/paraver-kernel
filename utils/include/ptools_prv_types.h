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


#pragma once


#include "ptools_types.h"

typedef char          prvMagic_t[8];
typedef char          prvDate_t[23];
typedef PTOOLS_UINT64 prvTime_t;
typedef char          prvTimeUnit_t[2];

typedef PTOOLS_UINT32 prvObjectID_t;

// Resources
typedef prvObjectID_t prvNodeID_t;
typedef prvObjectID_t prvCPUID_t;

typedef prvObjectID_t prvNumNodes_t;
typedef prvObjectID_t prvNumCPUs_t;

// Applications
typedef prvObjectID_t prvApplicationID_t;
typedef prvObjectID_t prvTaskID_t;
typedef prvObjectID_t prvThreadID_t;

typedef prvObjectID_t prvNumApplications_t;
typedef prvObjectID_t prvNumTasks_t;
typedef prvObjectID_t prvNumThreads_t;

// Communicators
typedef PTOOLS_UINT32 prvCommunicatorID_t;

typedef PTOOLS_UINT32 prvNumCommunicators_t;

// Records
typedef char          prvRecordType_t;

typedef PTOOLS_UINT16 prvState_t;

typedef PTOOLS_UINT32 prvEventType_t;
typedef PTOOLS_INT64  prvEventValue_t;

typedef PTOOLS_UINT32 prvCommSize_t;
typedef PTOOLS_UINT32 prvCommTag_t;



