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



template <class T>
FunctionManagement<T> *FunctionManagement<T>::inst = nullptr;


template <class T>
FunctionManagement<T>::~FunctionManagement()
{
  typename std::map<std::string, T*>::iterator it = hash.begin();
  while ( it != hash.end() )
  {
    delete ( *it ).second;
    ++it;
  }

  inst = nullptr;
}


template <class T>
FunctionManagement<T>::FunctionManagement( std::vector<std::string>& whichGroups,
    std::vector<std::string>& whichFunctions,
    std::vector<std::vector<T *> >& objects )
{
  PRV_UINT32 iName = 0;

  nameGroups = whichGroups;

  for ( PRV_UINT32 iGroup = 0; iGroup < objects.size(); iGroup++ )
  {
    groups.push_back( std::vector<T *>() );
    for ( PRV_UINT32 iObject = 0; iObject < objects[ iGroup ].size(); iObject++ )
    {
      hash.insert( std::pair<std::string, T*>( whichFunctions[ iName ], objects[ iGroup ][ iObject ] ) );
      groups[ iGroup ].push_back( objects[ iGroup ][ iObject ] );
      iName++;
    }
  }
}


template <class T>
FunctionManagement<T> *FunctionManagement<T>::getInstance()
{
  return inst;
}


template <class T>
FunctionManagement<T> *FunctionManagement<T>::getInstance( std::vector<std::string>& whichGroups,
    std::vector<std::string>& whichFunctions,
    std::vector<std::vector<T *> >& objects )
{
  if ( inst == nullptr )
    inst = new FunctionManagement( whichGroups, whichFunctions, objects );
  return inst;
}


template <class T>
T *FunctionManagement<T>::getFunction( const std::string& name ) const
{
  T *retval = nullptr;

  typename std::map<std::string, T*>::const_iterator it = hash.find( name );
  if ( it != hash.end() )
  {
    retval = ( ( *it ).second )->clone();
  }

  return retval;
}

template <class T>
PRV_UINT32 FunctionManagement<T>::numGroups() const
{
  return groups.size();
}


template <class T>
void FunctionManagement<T>::getAll( std::vector<T *>& onVector ) const
{
  for ( PRV_UINT32 iGroup = 0; iGroup < groups.size(); iGroup++ )
  {
    for ( PRV_UINT32 iObject = 0; iObject < groups[ iGroup ].size(); iObject++ )
      onVector.push_back( groups[ iGroup ][ iObject ] );
  }
}


template <class T>
void FunctionManagement<T>::getAll( std::vector<T *>& onVector, PRV_UINT32 whichGroup ) const
{
  for ( PRV_UINT32 iObject = 0; iObject < groups[ whichGroup ].size(); iObject++ )
    onVector.push_back( groups[ whichGroup ][ iObject ] );
}


template <class T>
void FunctionManagement<T>::getNameGroups( std::vector<std::string>& onVector )
{
  onVector = nameGroups;
}


template <class T>
void FunctionManagement<T>::getAll( std::vector<std::string>& onVector ) const
{
  for ( PRV_UINT32 iGroup = 0; iGroup < groups.size(); iGroup++ )
  {
    for ( PRV_UINT32 iObject = 0; iObject < groups[ iGroup ].size(); iObject++ )
      onVector.push_back( groups[ iGroup ][ iObject ]->getName() );
  }
}


template <class T>
void FunctionManagement<T>::getAll( std::vector<std::string>& onVector, PRV_UINT32 whichGroup ) const
{
  for ( PRV_UINT32 iObject = 0; iObject < groups[ whichGroup ].size(); iObject++ )
    onVector.push_back( ( groups[ whichGroup ][ iObject ] )->getName() );
}
