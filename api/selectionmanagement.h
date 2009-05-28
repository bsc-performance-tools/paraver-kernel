#ifndef _SELECTION_MANAGEMENT_H
#define _SELECTION_MANAGEMENT_H

#include <vector>

template <typename SelType>
class SelectionManagement
{
  public:
    SelectionManagement();
    ~SelectionManagement();

    void setSelected( vector< bool > &selection );
    void setSelected( vector< SelType > &selection, SelType maxElems );

    void getSelected( vector< bool > &selection );
    void getSelected( vector< SelType > &selection );
    void getSelected( vector< SelType > &selection, SelType first, SelType last );

    bool isSelectedPosition( SelType whichSelected );

    SelType firstSelected();
    SelType lastSelected();

  private:
    SelType first;
    SelType last;
    vector< bool > selected;
    vector< SelType > selectedSet;
};

#include "selectionmanagement.cpp"

#endif // _SELECTION_MANAGEMENT_H
