#ifndef _SELECTION_MANAGEMENT_H
#define _SELECTION_MANAGEMENT_H

template <typename SelType>
class SelectionManagement
{
  public:
    SelectionManagement();
    ~SelectionManagement();

    void setSelected( vector<bool> &selection );
    bool getSelectedPosition( SelType );

    SelType firstSelected();
    SelType lastSelected();
    UINT32 countSelected();

  private:
    UINT32 countSelected
    SelType firstSelected;
    SelType lastSelected;
    vector< bool > selected;
};

#endif // _SELECTION_MANAGEMENT_H
