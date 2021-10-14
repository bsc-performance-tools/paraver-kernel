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


#include <string>
#include <vector>
#include <map>
#include "paraverkerneltypes.h"

class RowLabels
{
  public:
    RowLabels();
    RowLabels( const std::string& filename );
    ~RowLabels();

    std::string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const;
    void pushBack( TWindowLevel whichLevel, const std::string& rowLabel );

    // whichLevel == NONE (by default) ==> all levels MaxLength
    size_t getMaxLength( TWindowLevel whichLevel = NONE ) const;

  protected:

  private:
    std::vector<std::string> workload;
    std::vector<std::string> appl;
    std::vector<std::string> task;
    std::vector<std::string> thread;

    std::vector<std::string> system;
    std::vector<std::string> node;
    std::vector<std::string> cpu;

    std::map< TWindowLevel, size_t > maxLength;
};



