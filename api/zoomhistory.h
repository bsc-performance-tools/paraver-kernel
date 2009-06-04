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
                  vector< Dimension2 >& selected );
    void addZoom( vector< Dimension2 >& selected );
    void addZoom( Dimension1 begin, Dimension1 end );

    pair<Dimension1, Dimension1> getFirstDimension() const;
    void getSecondDimension( vector< Dimension2> &selected ) const;

    bool isEmpty() const;
    bool emptyNextZoom() const;
    bool emptyPrevZoom() const;

    void firstZoom();
    void nextZoom();
    void prevZoom();

    ZoomHistory *clone();

  private:
    int currentZoom;
    vector< pair< pair<Dimension1,Dimension1>, vector<Dimension2> > > zooms;
};

#endif // _ZOOMHISTORY_H
