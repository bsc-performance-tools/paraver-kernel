#include "selectionmanagement.h"

template < typename SelType >
SelectionManagement::SelectionManagement()
{
}

template < typename SelType >
~SelectionManagement::SelectionManagement()
{
}

template <typename SelType>
void SelectionManagement::setSelected( vector< bool > &selection )
{
  // Search for first and last selected
  selected = selection;

  if ( !selection.empty() )
  {
    bool firstFound = false;
    for ( size_t current = 0; current < selected.size(); ++current )
    {
      if ( selected[ current ] )
      {
        if ( !firstFound )
          firstSelected = ( SelType )current;  // caution!

        lastFound = ( SelType )current;
        selectedSet.push_back( current );
      }
    }
  }
}

template <typename SelType>
bool SelectionManagement::getSelectedPosition( SelType &selected )
{
  return selected[ SelType ];
}

template <typename SelType>
void SelectionManagement::getSelectedSet( vector<SelType> &whichSelected )
{
  whichSelected.clear();
  for ( vector<SelType>::iterator it = selectedSet.begin(); it != selectedSet.end(); ++it )
    whichSelected.push_back( *it );
}

template <typename SelType>
SelType SelectionManagement::firstSelected()
{
  return ( SelType ) firstSelected;
}

template <typename SelType>
SelType SelectionManagement::lastSelected()
{
  return  ( SelType ) lastSelected;
}

template <typename SelType>
UINT32 SelectionManagement::countSelected()
{
  return countSelected;
}

