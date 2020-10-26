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

void SelectionRowsUtils::getAllLevelsSelectedRows( const Trace* whichTrace,
                                                   const SelectionManagement< TObjectOrder, TWindowLevel > &selectedRow,
                                                   TWindowLevel onLevel,
                                                   vector< TObjectOrder > &selected )
{
  vector< TObjectOrder > allLevelsSelected;
  TObjectOrder iAppl, iTask, globalTask, iNode, aux;
  switch ( onLevel )
  {
    case TASK:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getTaskLocation( *it, iAppl, aux );
        if ( selectedRow.isSelectedPosition( iAppl, APPLICATION ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case THREAD:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getThreadLocation( *it, iAppl, iTask, aux );
        globalTask = whichTrace->getGlobalTask( iAppl, iTask );

        if ( selectedRow.isSelectedPosition( iAppl, APPLICATION ) &&
             selectedRow.isSelectedPosition( globalTask, TASK ) )
          allLevelsSelected.push_back( *it );
      }

      break;

    case CPU:
      for ( vector< TObjectOrder >::iterator it = selected.begin(); it != selected.end(); ++it )
      {
        whichTrace->getCPULocation( *it, iNode, aux );
        if ( selectedRow.isSelectedPosition( iNode, NODE ) )
          allLevelsSelected.push_back( *it );
      }
      break;

    default:
      break;
  }

  if ( allLevelsSelected.size() > 0 )
    selected.swap( allLevelsSelected );
}
