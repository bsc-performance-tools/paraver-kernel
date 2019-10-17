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


#include <algorithm>
#include <functional>
#include "trace.h"
#include "histogramtotals.h"
#include "paraverkernelexception.h"

template < typename SelType, typename LevelType >
SelectionManagement< SelType, LevelType >::SelectionManagement()
{
}

template < typename SelType, typename LevelType >
SelectionManagement< SelType, LevelType >::SelectionManagement( const SelectionManagement& whichSelection )
{
  for ( size_t i = 0 ; i < whichSelection.selectedSet.size(); ++i )
  {
    selectedSet.push_back( whichSelection.selectedSet[ i ] );
  }
  for ( size_t i = 0 ; i < whichSelection.selected.size(); ++i )
  {
    selected.push_back( whichSelection.selected[ i ] );
  }
}


template < typename SelType, typename LevelType >
SelectionManagement< SelType, LevelType >::~SelectionManagement()
{
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::init( Trace *trace )
{
  selected.clear();
  selectedSet.clear();

  std::vector< bool > auxSelected;

  for ( PRV_UINT32 level = ( PRV_UINT32 )NONE; level <= ( PRV_UINT32 )CPU; ++level )
  {
    auxSelected.clear();
    selected.push_back( std::vector< bool >( ) );
    selectedSet.push_back( std::vector< TObjectOrder >( ) );

    switch ( level )
    {
      case APPLICATION:
        auxSelected.insert( auxSelected.begin(),
                            ( size_t ) trace->totalApplications(),
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;

      case TASK:
        auxSelected.insert( auxSelected.begin(),
                            ( size_t ) trace->totalTasks(),
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;

      case THREAD:
        auxSelected.insert( auxSelected.begin(),
                            ( size_t ) trace->totalThreads(),
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;

      case NODE:
        auxSelected.insert( auxSelected.begin(),
                            trace->totalNodes(),
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;

      case CPU:
        auxSelected.insert( auxSelected.begin(),
                            trace->totalCPUs(),
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;

//    NONE, WORKLOAD, SYSTEM
      default:
        auxSelected.insert( auxSelected.begin(),
                            1,
                            true );
        setSelected( auxSelected, ( TWindowLevel )level );
        break;
    }
  }
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::init( HistogramTotals *totals,
    PRV_UINT16 idStat,
    THistogramColumn numColumns,
    THistogramColumn whichPlane )
{
  selected.clear();
  selectedSet.clear();

  selected.push_back( std::vector< bool >( ) );
  selectedSet.push_back( std::vector< SelType >( ) );

  std::vector< bool > auxSelected;
  for ( THistogramColumn i = 0; i < numColumns; ++i )
  {
    if ( totals->getTotal( idStat, i, whichPlane ) == 0 )
      auxSelected.push_back( false );
    else
      auxSelected.push_back( true );
  }

  setSelected( auxSelected, 0 );
}


template < typename SelType, typename LevelType >
void SelectionManagement<SelType, LevelType>::copy( const SelectionManagement &whichSelection )
{
  selected = whichSelection.selected;
  selectedSet = whichSelection.selectedSet;
}


template < typename SelType, typename LevelType >
bool SelectionManagement<SelType, LevelType>:: operator== ( const SelectionManagement<SelType, LevelType> &whichSelection ) const
{
  bool equal = false;

  if ( selected.size()    == whichSelection.selected.size() &&
       selectedSet.size() == whichSelection.selectedSet.size() )
  {
    std::vector< std::vector< bool > >::const_iterator it1 = whichSelection.selected.begin();
    for ( std::vector< std::vector< bool > >::const_iterator it2 = selected.begin(); it2 != selected.end(); ++it2 )
    {
      equal = std::equal( (*it1).begin(), (*it1).end(), (*it2).begin() );
      if ( !equal )
      {
        break;
      }
      else
      {
        ++it1;
      }
    }

    if ( equal )
    {
      for ( size_t i = 0 ; i < selectedSet.size(); ++i ) // O( selected.size() )
      {
        equal = whichSelection.selectedSet[ i ].size() == selectedSet[ i ].size();
        if ( !equal )
        {
          break;
        }
      }

      if ( equal ) // O( selected.size() * num_elems )
      {
        for ( size_t i = 0 ; i < selectedSet.size(); ++i )
        {
          equal = std::equal( selectedSet[ i ].begin(), selectedSet[ i ].end(), whichSelection.selectedSet[ i ].begin() );
          if ( !equal )
          {
            break;
          }
        }
      }
    }
  }

  return equal;
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::setSelected( std::vector< bool > &selection, LevelType level )
{
  selectedSet[ level ].clear();
  if ( selected[ level ].size() >= selection.size() )
  {
    std::copy( selection.begin(), selection.end(), selected[ level ].begin() );
  }
  else
  {
    size_t size = selected[ level ].size();
    if ( size > 0 )
      selected[ level ].resize( size );
    selected[ level ] = selection;
  }

  if ( !selected[ level ].empty() )
  {
    for ( size_t current = 0; current < selected[ level ].size(); ++current )
    {
      if ( selected[ level ][ current ] )
        selectedSet[ level ].push_back( current );
    }
  }
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::setSelected( std::vector< SelType > &selection,
    SelType maxElems,
    LevelType level )
{
  // Prepare vectors for update
  selected[ level ].clear();
  selectedSet[ level ] = selection;

  typename std::vector<SelType>::iterator it;

  // Delete any SelType greater than maxElems ( number of level objects)
  it = std::find_if( selectedSet[ level ].begin(),
                     selectedSet[ level ].end(),
                     std::bind2nd( std::greater_equal<SelType>(), maxElems ) );
  if ( it != selectedSet[ level ].end() )
    selectedSet[ level ].erase( it, selectedSet[ level ].end() );

  // Any reamining row?
  if ( !selectedSet[ level ].empty() )
  {
    it = selectedSet[ level ].begin();
    for ( size_t current = 0; current < ( size_t ) maxElems; ++current )
    {
      if ( it != selectedSet[level].end() && current == ( size_t )*it )
      {
        selected[ level ].push_back( true );
        ++it;
      }
      else
      {
        selected[ level ].push_back( false );
      }
    }
  }
}


template < typename SelType, typename LevelType >
bool SelectionManagement< SelType, LevelType >::isSelectedPosition( SelType whichSelected,
    LevelType level ) const
{
  return selected[ level ][ whichSelected ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( std::vector< bool > &whichSelected,
    LevelType level )
{
  whichSelected = selected[ level ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( std::vector< bool > &whichSelected,
    SelType first,
    SelType last,
    LevelType level )
{
  whichSelected.clear();

  for ( SelType i = first; i <= last; ++ i )
    whichSelected.push_back( ( selected[ level ] )[ i ] );
}

template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( std::vector< SelType > &whichSelected,
    LevelType level )
{
  whichSelected = selectedSet[ level ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( std::vector< SelType > &whichSelected,
    SelType first,
    SelType last,
    LevelType level )
{
  whichSelected.clear();
  typename std::vector< SelType >::iterator it;
  for ( it = selectedSet[ level ].begin(); it != selectedSet[ level ].end(); ++it )
  {
    if ( ( *it >= first ) && ( *it <= last ) )
      whichSelected.push_back( *it );
    if ( *it == last )
      break;
  }
}


template < typename SelType, typename LevelType >
SelType SelectionManagement< SelType, LevelType >::shiftFirst( SelType whichFirst, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, LevelType level ) const
{
  const typename std::vector<SelType>& tmpSelectedSet = selectedSet[ level ];
  const std::vector<bool>& tmpSelected = selected[ level ];

  if( whichFirst >= tmpSelected.size() )
    throw ParaverKernelException( ParaverKernelException::indexOutOfRange );

  SelType iFirst = 0;
  if( tmpSelected.size() == tmpSelectedSet.size() )
  {
    iFirst = whichFirst;
  }
  else
  {
    while( whichFirst > tmpSelectedSet[ iFirst ] )
    {
      ++iFirst;
      if( iFirst >= tmpSelectedSet.size() )
      {
        iFirst = tmpSelectedSet.size() - 1;
        break;
      }
    }
  }

  if( (PRV_INT64)iFirst + shiftAmount < 0 )
  {
    appliedAmount = -(PRV_INT64)iFirst;
    return tmpSelectedSet[ 0 ];
  }
  else if( (PRV_INT64)iFirst + shiftAmount >= tmpSelectedSet.size() )
  {
    appliedAmount = (PRV_INT64)tmpSelectedSet.size() - 1 - (PRV_INT64)iFirst;
    return tmpSelectedSet[ tmpSelectedSet.size() - 1 ];
  }

  appliedAmount = shiftAmount;
  return tmpSelectedSet[ iFirst + shiftAmount ];
}


template < typename SelType, typename LevelType >
SelType SelectionManagement< SelType, LevelType >::shiftLast( SelType whichLast, PRV_INT64 shiftAmount, PRV_INT64& appliedAmount, LevelType level ) const
{
  const typename std::vector<SelType>& tmpSelectedSet = selectedSet[ level ];
  const std::vector<bool>& tmpSelected = selected[ level ];

  if( whichLast >= tmpSelected.size() )
    throw ParaverKernelException( ParaverKernelException::indexOutOfRange );

  SelType iLast = tmpSelectedSet.size() - 1;
  if( tmpSelected.size() == tmpSelectedSet.size() )
  {
    iLast = whichLast;
  }
  else
  {
    while( whichLast < tmpSelectedSet[ iLast ] )
    {
      --iLast;
      if( iLast == 0 )
      {
        break;
      }
    }
  }

  if( (PRV_INT64)iLast + shiftAmount < 0 )
  {
    appliedAmount = -(PRV_INT64)iLast;
    return tmpSelectedSet[ 0 ];
  }
  else if( (PRV_INT64)iLast + shiftAmount >= tmpSelectedSet.size() )
  {
    appliedAmount = (PRV_INT64)tmpSelectedSet.size() - 1 - (PRV_INT64)iLast;
    return tmpSelectedSet[ tmpSelectedSet.size() - 1 ];
  }

  appliedAmount = shiftAmount;
  return tmpSelectedSet[ iLast + shiftAmount ];
}
