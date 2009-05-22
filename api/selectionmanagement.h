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
    bool isSelectedPosition( SelType );
    void getSelectedSet( vector< SelType > &selection,
                         SelType first = ( SelType )0,
                         SelType last  = ( SelType )0 );
    void getSelectedSetRange( vector< SelType > &selection,
                              SelType first,
                              SelType last );

    SelType firstSelected();
    SelType lastSelected();

  private:
    vector< bool > selected;
    vector< SelType > selectedSet;
};

#endif // _SELECTION_MANAGEMENT_H
