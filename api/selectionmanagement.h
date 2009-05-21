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
    bool getSelectedPosition( SelType );
    void getSelectedSet( vector< SelType > &selection );

    SelType firstSelected();
    SelType lastSelected();
    UINT32 countSelected();

  private:
    vector< bool > selected;
    vector< SelType > selectedSet;
};

#endif // _SELECTION_MANAGEMENT_H
