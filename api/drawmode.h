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


#ifndef DRAWMODE_H_INCLUDED
#define DRAWMODE_H_INCLUDED

#include <vector>
#include "paraverkerneltypes.h"


enum DrawModeMethod
{
  DRAW_LAST = 0,
  DRAW_MAXIMUM, DRAW_MINNOTZERO,
  DRAW_RANDOM, DRAW_RANDNOTZERO,
  DRAW_AVERAGE, DRAW_AVERAGENOTZERO,
  DRAW_MODE,
  DRAW_ABSOLUTE_MAXIMUM, DRAW_ABSOLUTE_MINNOTZERO,
  DRAW_NUMMETHODS
};


class DrawMode
{
  public:
    static TSemanticValue selectValue( std::vector<TSemanticValue>& v,
                                       DrawModeMethod method );
};

#endif // DRAWMODE_H_INCLUDED
