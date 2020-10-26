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


#ifndef PREVIOUSFILES_H_INCLUDED
#define PREVIOUSFILES_H_INCLUDED

#include <string>
#include <fstream>
#include <vector>

#include "paraverkerneltypes.h"

class PreviousFiles
{
  public:
    static PreviousFiles *createPreviousTraces();
    static PreviousFiles *createPreviousCFGs();
    static PreviousFiles *createPreviousSessions();
    static PreviousFiles *createPreviousTreatedTraces();

    ~PreviousFiles();

    bool add( const std::string &newFile );
    const std::vector<std::string>& getFiles() const;

    static const std::string previousTracesFile;
    static const std::string previousCFGsFile;
    static const std::string previousSessionsFile;
    static const std::string previousTreatedTracesFile;

    static const PRV_UINT16 SIZE = 20;

  private:
    PreviousFiles( const std::string &filename, bool purge = false );

    std::fstream myFile;
    std::string  myFileName;
    std::vector< std::string > listFiles;

    void create();
    bool update();
    void read( std::fstream &myFile );
};


#endif // PREVIOUSFILES_H_INCLUDED
