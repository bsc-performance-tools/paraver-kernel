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


#pragma once


// SERIALIZATION INCLUDES
#include <fstream>
#include <iostream>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "utils/traceparser/tracetypes.h"

struct rgb
{
  template< class Archive >
  void serialize( Archive & ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp( "red", red );
    ar & boost::serialization::make_nvp( "green", green );
    ar & boost::serialization::make_nvp( "blue", blue );
  }


  ParaverColor red;
  ParaverColor green;
  ParaverColor blue;

  bool operator==( const rgb& b ) const
  {
    return red == b.red && green == b.green && blue == b.blue;
  }

  bool operator!=( const rgb& b ) const
  {
    return !( red == b.red && green == b.green && blue == b.blue );
  }

  bool operator<( const rgb& b ) const
  {
    return (red < b.red) ||
           (red == b.red && blue < b.blue ) ||
           (red == b.red && blue == b.blue && green < b.green);
  }
};
