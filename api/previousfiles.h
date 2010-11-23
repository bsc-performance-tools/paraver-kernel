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

#ifndef PREVIOUSFILES_H_INCLUDED
#define PREVIOUSFILES_H_INCLUDED

#include <string>
#include <fstream>
using namespace std;
#include <vector>

#include "paraverkerneltypes.h"

class PreviousFiles
{
  public:
    static PreviousFiles *createPreviousTraces()
    {
      return new PreviousFiles( previousTracesFile );
    }
    static PreviousFiles *createPreviousCFGs()
    {
      return new PreviousFiles( previousCFGsFile );
    }

    ~PreviousFiles();

    bool add( const string &newFile );
    const vector<string>& getFiles() const;

    static const string previousTracesFile;
    static const string previousCFGsFile;

    static const PRV_UINT16 SIZE = 20;

  private:
    PreviousFiles( const string &filename );

    fstream myFile;
    string  myFileName;
    vector< string > listFiles;

    void create();
    bool update();
    void read( fstream &myFile );
};


#endif // PREVIOUSFILES_H_INCLUDED
