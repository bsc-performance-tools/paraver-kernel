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

    void addZoom( Dimension1 begin1, Dimension1 end1,
                  Dimension2 begin2, Dimension2 end2 );
    void addZoom( Dimension2 begin, Dimension2 end );
    void addZoom( Dimension1 begin, Dimension1 end );

    pair<Dimension1, Dimension1> getFirstDimension() const;
    pair<Dimension2, Dimension2> getSecondDimension() const;

    bool isEmpty() const;
    bool emptyNextZoom() const;
    bool emptyPrevZoom() const;

    void firstZoom();
    void nextZoom();
    void prevZoom();

  private:
    int currentZoom;
    vector< pair< pair<Dimension1,Dimension1>, pair<Dimension2, Dimension2> > > zooms;
};

#endif // _ZOOMHISTORY_H
