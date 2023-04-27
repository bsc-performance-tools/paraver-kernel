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


#include <algorithm>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "paraverkerneltypes.h"

template <typename ValueType, size_t NStats>
Cell<ValueType, NStats>::Cell():
    row( 0 ), isNotZeroValue( false )
{}


template <typename ValueType, size_t NStats>
Cell<ValueType, NStats>::Cell( TObjectOrder idRow ):
    row( idRow ), isNotZeroValue( false )
{
  values.fill( 0 );
}

template <typename ValueType, size_t NStats>
Cell<ValueType, NStats>::Cell( const Cell< ValueType, NStats >& source ):
    row( source.row ), isNotZeroValue( source.isNotZeroValue )
{
  values = source.values;
}

template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::init( PRV_UINT16 idStat )
{
  if ( NStats > idStat )
    values[ idStat ] = (ValueType) 0;
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::init( )
{
  values.fill( 0 );
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::setValue( PRV_UINT16 idStat, ValueType semVal )
{
  if ( NStats > idStat )
    values[ idStat ] = semVal;
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::setValue( const std::array<ValueType, NStats>& semVal, bool isNotZeroValue )
{
  values = semVal;
  this->isNotZeroValue = isNotZeroValue;
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::addValue( PRV_UINT16 idStat, ValueType semVal )
{
  if ( NStats > idStat )
    values[ idStat ] += semVal;
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::addValue( const std::array<ValueType, NStats>& semVal )
{
  std::transform( values.begin(), values.end(), semVal.begin(), values.begin(), std::plus<ValueType>() );
}


template <typename ValueType, size_t NStats>
inline ValueType Cell<ValueType, NStats>::getValue( PRV_UINT16 idStat ) const
{
  if ( NStats <= idStat )
    throw std::out_of_range( "Cell::getValue: invalid statistic id" );

  return values[ idStat ];
}


template <typename ValueType, size_t NStats>
inline bool Cell<ValueType, NStats>::getNotZeroValue( PRV_UINT16 idStat ) const
{
  return isNotZeroValue;
}


template <typename ValueType, size_t NStats>
inline std::array<ValueType, NStats> Cell<ValueType, NStats>::getValue() const
{
  return values;
}


template <typename ValueType, size_t NStats>
inline TObjectOrder Cell<ValueType, NStats>::getRow( ) const
{
  return row;
}


template <typename ValueType, size_t NStats>
inline void Cell<ValueType, NStats>::setRow( TObjectOrder row )
{
  this->row = row;
}


template <typename ValueType, size_t NStats>
inline bool Cell<ValueType, NStats>::operator==( const ValueType& anotherCell ) const
{
  return row == anotherCell.row;
}


template <typename ValueType, size_t NStats>
inline bool Cell<ValueType, NStats>::operator<( const ValueType& anotherCell ) const
{
  return row < anotherCell.row;
}
