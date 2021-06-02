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

#include "intervalshift.h"
#include "kwindow.h"

KRecordList *IntervalShift::init( TRecordTime initialTime, TCreateList create,
                                  KRecordList *displayList )
{
  queue<ShiftSemanticInfo>().swap( semanticBuffer );

  return displayList;
}

KRecordList *IntervalShift::calcNext( KRecordList *displayList, bool initCalc )
{

  return displayList;
}


KRecordList *IntervalShift::calcPrev( KRecordList *displayList, bool initCalc )
{

  return displayList;
}

TWindowLevel IntervalShift::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalShift::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalShift::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalShift::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalShift::getWindowTrace() const
{
  return (KTrace*)window->getTrace();
}

void IntervalShift::setChildren()
{
}
