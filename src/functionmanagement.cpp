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
    it++;
  }
}


template <class T>
FunctionManagement<T>::FunctionManagement( vector<string>& names, vector<T *>& objects )
{
  UINT16 i;

  for ( i = 0; i < names.size(); i++ )
  {
    hash.insert( pair<string, T*>( names[i], objects[i] ) );
  }

}


template <class T>
FunctionManagement<T> *FunctionManagement<T>::getInstance()
{

  return inst;

}


template <class T>
FunctionManagement<T> *FunctionManagement<T>::getInstance( vector<string>& names, vector<T *>& objects )
{

  inst = new FunctionManagement( names, objects );
  return inst;

}


template <class T>
T *FunctionManagement<T>::getFunction( const string& name )
{
  T *retval = NULL;

  typename map<string, T*>::iterator it = hash.find( name );
  if ( it != hash.end() )
    retval = ( ( *it ).second )->clone();

  return retval;
}

