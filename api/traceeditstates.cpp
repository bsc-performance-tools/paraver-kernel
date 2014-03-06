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
//#include "window.h"

#include <boost/type_traits.hpp> // is_pointer

template< bool v > struct BoolToType { enum { value = v }; };

/****************************************************************************
 ********                  BaseTraceEditState                        ********
 ****************************************************************************/

template< class SeqT >
BaseTraceEditState< SeqT >::BaseTraceEditState()
{
}

template< class SeqT >
BaseTraceEditState< SeqT >::BaseTraceEditState( SeqT whichSequence )
{
  mySequence = whichSequence;
}

template< class SeqT >
BaseTraceEditState< SeqT >::~BaseTraceEditState()
{}

template< class SeqT >
void BaseTraceEditState< SeqT >::init()
{}


/****************************************************************************
 ********                  DerivedTraceEditState                     ********
 ****************************************************************************/

template< class SeqT, class DataT >
DerivedTraceEditState< SeqT, DataT >::DerivedTraceEditState( SeqT whichSequence )
{
  BaseTraceEditState< SeqT >::mySequence = whichSequence;
  firstInit();
}


template< class SeqT, class DataT >
DerivedTraceEditState< SeqT, DataT >::~DerivedTraceEditState()
{
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::firstInit()
{
  firstInit( BoolToType< boost::is_pointer< DataT >::value >() );
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::firstInit( BoolToType< true > )
{
  myData = NULL;
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::firstInit( BoolToType< false > )
{
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::init()
{
  init( BoolToType< boost::is_pointer< DataT >::value >() );
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::init( BoolToType< true > )
{
  if ( myData != NULL )
    delete myData;
  myData = NULL;
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::init( BoolToType< false > )
{
}


template< class SeqT, class DataT >
DataT DerivedTraceEditState< SeqT, DataT >::getData() const
{
  return myData;
}


template< class SeqT, class DataT >
void DerivedTraceEditState< SeqT, DataT >::setData( DataT whichData )
{
  myData = whichData;
}



/****************************************************************************
 ********                  DerivedTraceEditStateBool                 ********
 ****************************************************************************/
template< class SeqT >
DerivedTraceEditStateBool< SeqT >::DerivedTraceEditStateBool( SeqT whichSequence )
{
  BaseTraceEditState< SeqT >::mySequence = whichSequence;
  init();
}

template< class SeqT >
DerivedTraceEditStateBool< SeqT >::~DerivedTraceEditStateBool()
{
}

template< class SeqT >
void DerivedTraceEditStateBool< SeqT >::init()
{
  myData = false;
}

template< class SeqT >
bool DerivedTraceEditStateBool< SeqT >::getData() const
{
  return myData;
}

template< class SeqT >
void DerivedTraceEditStateBool< SeqT >::setData( bool whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  DerivedTraceEditStateInt                  ********
 ****************************************************************************/
template< class SeqT >
DerivedTraceEditStateInt< SeqT >::DerivedTraceEditStateInt( SeqT whichSequence )
{
  BaseTraceEditState< SeqT >::mySequence = whichSequence;
  init();
}

template< class SeqT >
DerivedTraceEditStateInt< SeqT >::~DerivedTraceEditStateInt()
{
}

template< class SeqT >
void DerivedTraceEditStateInt< SeqT >::init()
{
  myData = 0;
}

template< class SeqT >
int DerivedTraceEditStateInt< SeqT >::getData() const
{
  return myData;
}

template< class SeqT >
void DerivedTraceEditStateInt< SeqT >::setData( int whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  DerivedTraceEditStateString               ********
 ****************************************************************************/
template< class SeqT >
DerivedTraceEditStateString< SeqT >::DerivedTraceEditStateString( SeqT whichSequence )
{
  BaseTraceEditState< SeqT >::mySequence = whichSequence;
  init();
}

template< class SeqT >
DerivedTraceEditStateString< SeqT >::~DerivedTraceEditStateString()
{
}

template< class SeqT >
void DerivedTraceEditStateString< SeqT >::init()
{
  myData.erase();
}

template< class SeqT >
std::string DerivedTraceEditStateString< SeqT >::getData() const
{
  return myData;
}

template< class SeqT >
void DerivedTraceEditStateString< SeqT >::setData( std::string whichData )
{
  myData = whichData;
}


/****************************************************************************
 ********                  DerivedTraceEditStateTTime                ********
 ****************************************************************************/
template< class SeqT >
DerivedTraceEditStateTTime< SeqT >::DerivedTraceEditStateTTime( SeqT whichSequence )
{
  BaseTraceEditState< SeqT >::mySequence = whichSequence;
  init();
}

template< class SeqT >
DerivedTraceEditStateTTime< SeqT >::~DerivedTraceEditStateTTime()
{
}

template< class SeqT >
void DerivedTraceEditStateTTime< SeqT >::init()
{
  myData = (TTime)0.0;
}

template< class SeqT >
TTime DerivedTraceEditStateTTime< SeqT >::getData() const
{
  return myData;
}

template< class SeqT >
void DerivedTraceEditStateTTime< SeqT >::setData( TTime whichData )
{
  myData = whichData;
}

