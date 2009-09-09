#ifndef _SELECTION_MANAGEMENT_H
#define _SELECTION_MANAGEMENT_H

#include <vector>
//#include "trace.h"
class Trace;
class HistogramTotals;

template <typename SelType, typename LevelType>
class SelectionManagement
{
  public:
    SelectionManagement();
    ~SelectionManagement();

    void init( Trace *trace );
    void init( HistogramTotals *totals,
               UINT16 idStat,
               THistogramColumn numColumns,
               THistogramColumn whichPlane );

    void copy( const SelectionManagement &selection );

    void setSelected( vector< bool > &selection, LevelType level = 0 );
    void setSelected( vector< SelType > &selection, SelType maxElems, LevelType level = 0 );

    void getSelected( vector< bool > &selection, LevelType level = 0 );
    void getSelected( vector< SelType > &selection, LevelType level = 0 );
    void getSelected( vector< SelType > &selection, SelType first, SelType last, LevelType level = 0 );

    bool isSelectedPosition( SelType whichSelected, LevelType level = 0 );

  private:
    vector< vector< bool > > selected;
    vector< vector< SelType > > selectedSet;
};


#include "selectionmanagement.cpp"

#endif // _SELECTION_MANAGEMENT_H
