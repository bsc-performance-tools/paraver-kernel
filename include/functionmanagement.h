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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef FUNCTIONMANAGEMENT_H_INCLUDED
#define FUNCTIONMANAGEMENT_H_INCLUDED


#include <map>
#include <vector>


using namespace std;

template <class T>
class FunctionManagement
{

  private:
    static FunctionManagement<T> *inst;

    vector<string> nameGroups;
    map<string, T *> hash;
    vector<vector<T *> > groups;

    FunctionManagement( vector<string>&,
                        vector<string>&,
                        vector<vector<T *> >& );


  public:
    static FunctionManagement<T> *getInstance();
    static FunctionManagement<T> *getInstance( vector<string>&,
                                               vector<string>&,
                                               vector<vector<T *> >& );

    ~FunctionManagement();

    T *getFunction( const string& ) const;
    UINT32 numGroups() const;
    void getNameGroups( vector<string>& );
    void getAll( vector<T *>& ) const;
    void getAll( vector<T *>&, UINT32 ) const;
    void getAll( vector<string>& ) const;
    void getAll( vector<string>&, UINT32 ) const;
};

#include "src/functionmanagement.cpp"

#endif // FUNCTIONMANAGEMENT_H_INCLUDED
