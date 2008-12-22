#include <vector>
#include <map>


template <class T>
FunctionManagement<T> *FunctionManagement<T>::inst = NULL;


template <class T>
FunctionManagement<T>::~FunctionManagement()
{
  typename map<string, T*>::iterator it = hash.begin();
  while ( it != hash.end() )
  {
    delete ( *it ).second;
    ++it;
  }

  inst = NULL;
}


template <class T>
FunctionManagement<T>::FunctionManagement( vector<string>& whichGroups,
    vector<string>& whichFunctions,
    vector<vector<T *> >& objects )
{
  UINT32 iName = 0;

  nameGroups = whichGroups;

  for ( UINT32 iGroup = 0; iGroup < objects.size(); iGroup++ )
  {
    groups.push_back( vector<T *>() );
    for ( UINT32 iObject = 0; iObject < objects[ iGroup ].size(); iObject++ )
    {
      hash.insert( pair<string, T*>( whichFunctions[ iName ], objects[ iGroup ][ iObject ] ) );
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
FunctionManagement<T> *FunctionManagement<T>::getInstance( vector<string>& whichGroups,
    vector<string>& whichFunctions,
    vector<vector<T *> >& objects )
{
  if ( inst == NULL )
    inst = new FunctionManagement( whichGroups, whichFunctions, objects );
  return inst;
}


template <class T>
T *FunctionManagement<T>::getFunction( const string& name ) const
{
  T *retval = NULL;

  typename map<string, T*>::const_iterator it = hash.find( name );
  if ( it != hash.end() )
    retval = ( ( *it ).second )->clone();

  return retval;
}

template <class T>
UINT32 FunctionManagement<T>::numGroups() const
{
  return groups.size();
}


template <class T>
void FunctionManagement<T>::getAll( vector<T *>& onVector ) const
{
  for ( UINT32 iGroup = 0; iGroup < groups.size(); iGroup++ )
  {
    for ( UINT32 iObject = 0; iObject < groups[ iGroup ].size(); iObject++ )
      onVector.push_back( groups[ iGroup ][ iObject ] );
  }
}


template <class T>
void FunctionManagement<T>::getAll( vector<T *>& onVector, UINT32 whichGroup ) const
{
  for ( UINT32 iObject = 0; iObject < groups[ whichGroup ].size(); iObject++ )
    onVector.push_back( groups[ whichGroup ][ iObject ] );
}


template <class T>
void FunctionManagement<T>::getNameGroups( vector<string>& onVector )
{
  onVector = nameGroups;
}


template <class T>
void FunctionManagement<T>::getAll( vector<string>& onVector ) const
{
  for ( UINT32 iGroup = 0; iGroup < groups.size(); iGroup++ )
  {
    for ( UINT32 iObject = 0; iObject < groups[ iGroup ].size(); iObject++ )
      onVector.push_back( groups[ iGroup ][ iObject ]->getName() );
  }
}


template <class T>
void FunctionManagement<T>::getAll( vector<string>& onVector, UINT32 whichGroup ) const
{
  for ( UINT32 iObject = 0; iObject < groups[ whichGroup ].size(); iObject++ )
    onVector.push_back( ( groups[ whichGroup ][ iObject ] )->getName() );
}
