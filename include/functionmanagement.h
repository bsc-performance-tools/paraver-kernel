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
#include <string>

template <class T>
class FunctionManagement
{

  private:
    static FunctionManagement<T> *inst;

    std::vector<std::string> nameGroups;
    std::map<std::string, T *> hash;
    std::vector<std::vector<T *> > groups;

    FunctionManagement( std::vector<std::string>&,
                        std::vector<std::string>&,
                        std::vector<std::vector<T *> >& );


  public:
    static FunctionManagement<T> *getInstance();
    static FunctionManagement<T> *getInstance( std::vector<std::string>&,
                                               std::vector<std::string>&,
                                               std::vector<std::vector<T *> >& );

    ~FunctionManagement();

    T *getFunction( const std::string& ) const;
    PRV_UINT32 numGroups() const;
    void getNameGroups( std::vector<std::string>& );
    void getAll( std::vector<T *>& ) const;
    void getAll( std::vector<T *>&, PRV_UINT32 ) const;
    void getAll( std::vector<std::string>& ) const;
    void getAll( std::vector<std::string>&, PRV_UINT32 ) const;
};

#include "src/functionmanagement.cpp"

#endif // FUNCTIONMANAGEMENT_H_INCLUDED
