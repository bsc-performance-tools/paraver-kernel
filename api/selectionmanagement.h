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
    void getSelected( vector< bool > &selection );
    bool isSelectedPosition( SelType whichSelected );
    void getSelectedSet( vector< SelType > &selection );
    void getSelectedSetRange( vector< SelType > &selection,
                              SelType first,
                              SelType last );

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
