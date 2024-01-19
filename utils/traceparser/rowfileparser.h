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

#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include "tracetypes.h"

template< typename dummy = std::nullptr_t >
class RowFileParser
{
  public:
    static bool openRowFileParser( const std::string& filename, RowFileParser<dummy>& outRowFile );

    RowFileParser() = default;
    RowFileParser( const std::string& filename );
    ~RowFileParser() = default;

    void dumpToFile( const std::string& filename ) const;

    std::string getRowLabel( TTraceLevel whichLevel, TObjectOrder whichRow ) const;
    void pushBack( TTraceLevel whichLevel, const std::string& rowLabel );

    // whichLevel == NONE (by default) ==> all levels MaxLength
    size_t getMaxLength( TTraceLevel whichLevel = TTraceLevel::NONE ) const;

  protected:

  private:
    static const size_t LEVEL_NAME = 0;
    static const size_t LEVEL_MAXLENGTH = 1;
    static const size_t LEVEL_LABELS = 2;

    // tuple elements are: level name, level max length, level objects labels
    std::map<TTraceLevel, std::tuple< std::string, size_t, std::vector<std::string> > > levelLabels;

    size_t globalMaxLength;

    void dumpLevel( const std::tuple< std::string, size_t, std::vector<std::string> >& whichLevel, std::ofstream& whichFile ) const;
};

#include "rowfileparser.cpp"
