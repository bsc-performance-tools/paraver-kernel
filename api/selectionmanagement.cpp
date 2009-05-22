#include "selectionmanagement.h"

template < typename SelType >
SelectionManagement::SelectionManagement()
{
}

template < typename SelType >
~SelectionManagement::SelectionManagement()
{
}

// Copies selection vector into selected attribute
// Sets firstSelected and lastSelected
// Fills selectedSet vector with i positions that verify selected[i] == true
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
bool SelectionManagement::isSelectedPosition( SelType &selected )
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
void SelectionManagement::getSelectedSetRange( vector<SelType> &whichSelected,
                                               SelType first,
                                               SelType last )
{
  whichSelected.clear();
  for ( vector<SelType>::iterator it = selectedSet.begin(); it != selectedSet.end(); ++it )
  {
    if (( *it >= first ) && ( *it <= last ))
      whichSelected.push_back( *it );
  }
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


