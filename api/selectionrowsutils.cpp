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


#include "selectionrowsutils.h"

using namespace std;

void SelectionRowsUtils::getAllLevelsSelectedRows( const Trace *whichTrace,
                                                   const SelectionManagement< TObjectOrder, TTraceLevel > &selectedRow,
                                                   TTraceLevel onLevel,
                                                   vector< TObjectOrder > &selected )
{
  vector< TObjectOrder > allLevelsSelected;
  TObjectOrder iAppl, iTask, globalTask, iNode, aux;
  switch( onLevel )
  {
    case TTraceLevel::TASK:
      for( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getTaskLocation( *it, iAppl, aux );
        if( selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case TTraceLevel::THREAD:
      for( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getThreadLocation( *it, iAppl, iTask, aux );
        globalTask = whichTrace->getGlobalTask( iAppl, iTask );

        if( selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) && selectedRow.isSelectedPosition( globalTask, TTraceLevel::TASK ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case TTraceLevel::CPU:
      for( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getCPULocation( *it, iNode, aux );
        if( selectedRow.isSelectedPosition( iNode, TTraceLevel::NODE ) )
          allLevelsSelected.push_back( *it );
      }
      break;

    default:
      break;
  }

  if( allLevelsSelected.size() > 0 )
    selected.swap( allLevelsSelected );
}

void SelectionRowsUtils::getAllLevelsSelectedRows( const Trace *whichTrace,
                                                   const SelectionManagement< TObjectOrder, TTraceLevel > &selectedRow,
                                                   TTraceLevel onLevel,
                                                   TObjectOrder firstObject,
                                                   TObjectOrder lastObject,
                                                   vector< bool > &selected )
{
  TObjectOrder iAppl, jTask, globalTask, iNode, aux;
  switch( onLevel )
  {
    case TTraceLevel::TASK:
      for( TObjectOrder iTask = firstObject; iTask <= lastObject; ++iTask )
      {
        whichTrace->getTaskLocation( iTask, iAppl, aux );
        selected[ iTask ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) && selected[ iTask ];
      }

      break;

    case TTraceLevel::THREAD:
      for( TObjectOrder iThread = firstObject; iThread <= lastObject; ++iThread )
      {
        whichTrace->getThreadLocation( iThread, iAppl, jTask, aux );
        globalTask          = whichTrace->getGlobalTask( iAppl, jTask );
        selected[ iThread ] = selectedRow.isSelectedPosition( iAppl, TTraceLevel::APPLICATION ) &&
                              selectedRow.isSelectedPosition( globalTask, TTraceLevel::TASK ) && selected[ iThread ];
      }

      break;

    case TTraceLevel::CPU:
      for( TObjectOrder iCPU = firstObject; iCPU <= lastObject; ++iCPU )
      {
        whichTrace->getCPULocation( iCPU, iNode, aux );
        selected[ iCPU ] = selected[ iCPU ] && selectedRow.isSelectedPosition( iNode, TTraceLevel::NODE );
      }
      break;

    default:
      break;
  }
}