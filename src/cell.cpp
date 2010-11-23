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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include <string>
#include <iostream>
#include <string.h>

#include "paraverkerneltypes.h"

using namespace std;

template <typename ValueType>
Cell<ValueType>::Cell( TObjectOrder idRow, PRV_UINT16 numStats ):
    row( idRow ), nStats( numStats )
{
  values.insert( values.begin(), nStats, (ValueType) 0 );
}

template <typename ValueType>
Cell<ValueType>::Cell( Cell< ValueType >& source ):
    row( source.row ), nStats( source.nStats )
{
  values = source.values;
}

template <typename ValueType>
Cell<ValueType>::~Cell()
{}


template <typename ValueType>
inline void Cell<ValueType>::init( PRV_UINT16 idStat )
{
  if ( nStats > idStat )
    values[ idStat ] = (ValueType) 0;
}


template <typename ValueType>
inline void Cell<ValueType>::init( )
{
  for ( PRV_UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] = (ValueType) 0;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( PRV_UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ] = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( ValueType semVal )
{
  for ( PRV_UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::setValue( const vector<ValueType>& semVal )
{
  values = semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( PRV_UINT16 idStat, ValueType semVal )
{
  if ( nStats > idStat )
    values[ idStat ] += semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( ValueType semVal )
{
  for ( PRV_UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] += semVal;
}


template <typename ValueType>
inline void Cell<ValueType>::addValue( const vector<ValueType>& semVal )
{
  for ( PRV_UINT16 ii = 0; ii < nStats; ++ii )
    values[ ii ] += semVal[ ii ];
}


template <typename ValueType>
inline ValueType Cell<ValueType>::getValue( PRV_UINT16 idStat ) const
{
  if ( nStats > idStat )
    return values[ idStat ];

  ValueType tmp;

  memset( ( void * ) &tmp, 0, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
inline vector<ValueType> Cell<ValueType>::getValue() const
{
  return values;
}


template <typename ValueType>
inline TObjectOrder Cell<ValueType>::getRow( ) const
{
  return row;
}


template <typename ValueType>
inline void Cell<ValueType>::setRow( TObjectOrder row )
{
  this->row = row;
}


template <typename ValueType>
inline void Cell<ValueType>::print( ) const
{
  cout << "[" << row << "] " ;
  for ( PRV_UINT16 ii = 0; ii < nStats; ++ii )
  {
    cout << values[ ii ];
    cout << " ";
  }

  cout << endl;
}
