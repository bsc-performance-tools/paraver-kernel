#ifndef _SELECTION_MANAGEMENT_H
#define _SELECTION_MANAGEMENT_H

#include <vector>
//#include "trace.h"
class Trace;

template <typename SelType>
class SelectionManagement
{
  public:
    SelectionManagement();
    ~SelectionManagement();

    void init( Trace *trace );
    void copy( const SelectionManagement &selection );

    void setSelected( vector< bool > &selection, TWindowLevel level );
    void setSelected( vector< SelType > &selection, SelType maxElems, TWindowLevel level );

    void getSelected( vector< bool > &selection, TWindowLevel level );
    void getSelected( vector< SelType > &selection, TWindowLevel level );
    void getSelected( vector< SelType > &selection, SelType first, SelType last, TWindowLevel level );

    bool isSelectedPosition( SelType whichSelected, TWindowLevel level );

  private:
    vector< vector< bool > > selected;
    vector< vector< SelType > > selectedSet;
};

#include "selectionmanagement.cpp"

#endif // _SELECTION_MANAGEMENT_H
