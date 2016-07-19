#ifndef VECTORUTILS_H_INCLUDED
#define VECTORUTILS_H_INCLUDED

#include <vector>
#include <iostream>
#include <algorithm>

//------------------------------------------------------------------
// SortIndex: create a vector of sorted positions from the original
//            vector passed in the constructor.
//
//  SortIndex::SortIndex( const vector<T>& ): contructor where you
//            pass the original vector to sort.
//  SortIndex::sort(): sort the positions in a new vector and returns
//            it.
//  SortIndex::getSorted(): returns the sorted vector of positions.
//------------------------------------------------------------------
template<class T>
class SortIndex
{
  public:
    SortIndex( const std::vector<T>& whichV ):v( whichV )
    {}

    bool operator()( int i1, int i2 )
    {
      return v[i1] < v[i2];
    }

    std::vector<int>& sort()
    {
      pos.clear();
      for( unsigned int i = 0; i < v.size(); i++ )
        pos.push_back(i);
      stable_sort( pos.begin(), pos.end(), *this );
      return pos;
    }

    std::vector<int>& getSorted()
    {
      return pos;
    }

  private:
    const std::vector<T>& v;
    std::vector<int> pos;

};


#endif // VECTORUTILS_H_INCLUDED
