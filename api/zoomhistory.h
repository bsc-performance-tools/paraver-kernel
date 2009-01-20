#ifndef _ZOOMHISTORY_H
#define _ZOOMHISTORY_H

using namespace std;

//#include <utility>
#include <vector>

template <typename Dimension1, typename Dimension2>
class ZoomHistory
{
  public:
    ZoomHistory();
    ~ZoomHistory();

    void AddZoom( Dimension1 begin1, Dimension1 end1,
                  Dimension2 begin2, Dimension2 end2 );

    pair<Dimension1, Dimension1> GetFirstDimension();
    pair<Dimension2, Dimension2> GetSecondDimension();

    bool IsEmpty();
    bool EmptyNextZoom();
    bool EmptyPrevZoom();

    void FirstZoom();
    void NextZoom();
    void PrevZoom();

  private:
    int currentZoom;
    int totalZooms;
    vector< pair< pair<Dimension1,Dimension1>, pair<Dimension2, Dimension2> > > zooms;
};

#endif // _ZOOMHISTORY_H
