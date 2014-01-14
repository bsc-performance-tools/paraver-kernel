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

#include "traceeditstates.h"
#include "traceoptions.h"
#include "window.h"


/****************************************************************************
 ********                  TestState                                 ********
 ****************************************************************************/

void TestState::init()
{
  myData = 0;
}

int TestState::getData() const
{
  return myData;
}

void TestState::setData( int whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  TraceOptionsState                         ********
 ****************************************************************************/

TraceOptionsState::~TraceOptionsState()
{
  if( myData != NULL )
    delete myData;
}

void TraceOptionsState::init()
{}

TraceOptions *TraceOptionsState::getData() const
{
  return myData;
}

void TraceOptionsState::setData( TraceOptions *whichData )
{
  if( myData != NULL )
    delete myData;

  myData = whichData;
}


/****************************************************************************
 ********                  CSVWindowState                            ********
 ****************************************************************************/

CSVWindowState::~CSVWindowState()
{}

void CSVWindowState::init()
{}

Window *CSVWindowState::getData() const
{
  return myData;
}

void CSVWindowState::setData( Window *whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  CSVFileNameState                          ********
 ****************************************************************************/

CSVFileNameState::~CSVFileNameState()
{}

void CSVFileNameState::init()
{}

std::string CSVFileNameState::getData() const
{
  return myData;
}

void CSVFileNameState::setData( std::string whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  CSVOutputState                          ********
 ****************************************************************************/

CSVOutputState::~CSVOutputState()
{}

void CSVOutputState::init()
{}

TextOutput CSVOutputState::getData() const
{
  return myData;
}

void CSVOutputState::setData( TextOutput whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********               OutputDirSuffixState                         ********
 ****************************************************************************/

OutputDirSuffixState::~OutputDirSuffixState()
{}

void OutputDirSuffixState::init()
{}

std::string OutputDirSuffixState::getData() const
{
  return myData;
}

void OutputDirSuffixState::setData( std::string whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********               OutputTraceFileNameState                     ********
 ****************************************************************************/

OutputTraceFileNameState::~OutputTraceFileNameState()
{}

void OutputTraceFileNameState::init()
{}

std::string OutputTraceFileNameState::getData() const
{
  return myData;
}

void OutputTraceFileNameState::setData( std::string whichData )
{
  myData = whichData;
}
