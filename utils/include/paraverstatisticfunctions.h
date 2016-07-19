#ifndef PARAVERSTATISTICFUNCIONS_H_INCLUDED
#define PARAVERSTATISTICFUNCIONS_H_INCLUDED

#include <map>

// STATISTICAL FUNCTIONS
//  - mode( data[], size ) :
//    The type of 'data' must be transformable to double, and 'size' must be
//    greater than 0.

template <typename T>

class ParaverStatisticFunctions
{

  public:
    typedef std::map< double, unsigned int > CountMap;

    static T mode( T data[], unsigned int size );
};



template <typename T>
T ParaverStatisticFunctions<T>::mode( T data[], unsigned int size )
{
  T currentMode = 0;
  unsigned int currentCount = 0;
  CountMap modeMap;
  CountMap::iterator insertObj;

  for ( unsigned int index = 0; index < size; index++ )
  {
    insertObj = modeMap.find( data[ index ] );

    if ( insertObj == modeMap.end())
    {
      modeMap[ data[ index ] ] = 1;
    }
    else
    {
      insertObj->second++;
    }

    if ( modeMap[ data[ index ] ] > currentCount )
    {
      currentCount = modeMap[ data[ index ] ];
      currentMode = data[ index ];
    }
  }

  return currentMode;
}



#endif // PARAVERSTATISTICFUNCIONS_H_INCLUDED
