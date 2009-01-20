#include "zoomhistory.h"

template <typename Dimension1, typename Dimension2>
ZoomHistory<Dimension1,Dimension2>::ZoomHistory( )
{
  currentZoom = -1;
}

template <typename Dimension1, typename Dimension2>
ZoomHistory<Dimension1,Dimension2>::~ZoomHistory( )
{
}

template <typename Dimension1, typename Dimension2>
pair<Dimension1, Dimension1> ZoomHistory<Dimension1,Dimension2>::GetFirstDimension()
{
  return zooms[ currentZoom ].first;
}

template <typename Dimension1, typename Dimension2>
pair<Dimension2, Dimension2> ZoomHistory<Dimension1,Dimension2>::GetSecondDimension()
{
  return zooms[ currentZoom ].second;
}


template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::AddZoom( Dimension1 begin1, Dimension1 end1,
                                                  Dimension2 begin2, Dimension2 end2 )
{
  int totalZooms = int(zooms.size());
  for (int i = currentZoom + 1; i < totalZooms; i++ )
      zooms.pop_back();

  zooms.push_back( make_pair( make_pair(begin1,end1), make_pair(begin2,end2)));
  currentZoom = zooms.size() - 1;
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::IsEmpty()
{
  return ( zooms.size() == 0 );
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::EmptyNextZoom()
{
  return ( currentZoom == int( zooms.size() - 1 ) );
}

template <typename Dimension1, typename Dimension2>
bool ZoomHistory<Dimension1,Dimension2>::EmptyPrevZoom()
{
  return ( currentZoom <= 0 );
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::FirstZoom( )
{
  currentZoom = 0;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::PrevZoom( )
{
  if ( currentZoom > 0 )
    currentZoom--;
}

template <typename Dimension1, typename Dimension2>
void ZoomHistory<Dimension1,Dimension2>::NextZoom( )
{
  if ( currentZoom < int(zooms.size() - 1) )
    currentZoom++;
}
