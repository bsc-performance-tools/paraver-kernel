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

#include <algorithm>
#include <functional>
#include "trace.h"
#include "histogramtotals.h"

template < typename SelType, typename LevelType >
SelectionManagement< SelType, LevelType >::SelectionManagement()
{
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

  vector< bool > auxSelected;

  for ( UINT32 level = ( UINT32 )NONE; level <= ( UINT32 )CPU; ++level )
  {
    auxSelected.clear();
    selected.push_back( vector< bool >( ) );
    selectedSet.push_back( vector< TObjectOrder >( ) );

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
    UINT16 idStat,
    THistogramColumn numColumns,
    THistogramColumn whichPlane )
{
  selected.clear();
  selectedSet.clear();

  selected.push_back( vector< bool >( ) );
  selectedSet.push_back( vector< SelType >( ) );

  vector< bool > auxSelected;
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
void SelectionManagement<SelType, LevelType>::copy( const SelectionManagement &selection )
{
  selected = selection.selected;
  selectedSet = selection.selectedSet;
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::setSelected( vector< bool > &selection,
    LevelType level )
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

  if ( !selection.empty() )
  {
    for ( size_t current = 0; current < selected[ level ].size(); ++current )
    {
      if ( selected[ level ][ current ] )
        selectedSet[ level ].push_back( current );
    }
  }
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::setSelected( vector< SelType > &selection,
    SelType maxElems,
    LevelType level )
{
  selected[ level ].clear();
  selectedSet[ level ] = selection;
  typename vector<SelType>::iterator maxIt;
  maxIt = std::find_if( selectedSet[ level ].begin(),
                        selectedSet[ level ].end(),
                        std::bind2nd( std::greater_equal<SelType>(), maxElems ) );
  if ( maxIt != selectedSet[ level ].end() )
    selectedSet[ level ].erase( maxIt, selectedSet[ level ].end() );

  if ( !selection.empty() )
  {
    typename vector< SelType >::iterator it;
    it = selection.begin();
    for ( size_t current = 0; current < ( size_t ) maxElems; ++current )
    {
      if ( it != selection.end() && current == ( size_t )*it )
      {
        selected[ level ].push_back( true );
        ++it;
      }
      else
        selected[ level ].push_back( false );
    }
  }
}


template < typename SelType, typename LevelType >
bool SelectionManagement< SelType, LevelType >::isSelectedPosition( SelType whichSelected,
    LevelType level )
{
  return selected[ level ][ whichSelected ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( vector< bool > &whichSelected,
    LevelType level )
{
  whichSelected = selected[ level ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( vector< bool > &whichSelected,
    SelType first,
    SelType last,
    LevelType level )
{
  whichSelected.clear();

  for ( SelType i = first; i <= last; ++ i )
    whichSelected.push_back( ( selected[ level ] )[ i ] );
}

template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( vector< SelType > &whichSelected,
    LevelType level )
{
  whichSelected = selectedSet[ level ];
}


template < typename SelType, typename LevelType >
void SelectionManagement< SelType, LevelType >::getSelected( vector< SelType > &whichSelected,
    SelType first,
    SelType last,
    LevelType level )
{
  whichSelected.clear();
  typename vector< SelType >::iterator it;
  for ( it = selectedSet[ level ].begin(); it != selectedSet[ level ].end(); ++it )
  {
    if ( ( *it >= first ) && ( *it <= last ) )
      whichSelected.push_back( *it );
    if ( *it == last )
      break;
  }
}
