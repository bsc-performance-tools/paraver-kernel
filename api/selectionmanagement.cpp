template < typename SelType >
SelectionManagement<SelType>::SelectionManagement()
{
}

template < typename SelType >
SelectionManagement<SelType>::~SelectionManagement()
{
}

// Copies selection vector into selected attribute
// Sets firstSelected and lastSelected
// Fills selectedSet vector with i positions that verify selected[i] == true
template <typename SelType>
void SelectionManagement<SelType>::setSelected( vector< bool > &selection )
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
          first = ( SelType )current;  // caution!

        last = ( SelType )current;
        selectedSet.push_back( current );
      }
    }
  }
}

template <typename SelType>
bool SelectionManagement< SelType >::isSelectedPosition( SelType whichSelected )
{
  return selected[ whichSelected ];
}


template <typename SelType>
void SelectionManagement< SelType >::getSelected( vector<bool> &whichSelected )
{
  whichSelected.clear();
  typename vector<SelType>::iterator it;

  for( it = selected.begin(); it != selected.end(); ++it )
    whichSelected.push_back( *it );
}

template <typename SelType>
void SelectionManagement< SelType >::getSelectedSet( vector<SelType> &whichSelected )
{
  whichSelected.clear();
  typename vector<SelType>::iterator it;

  for( it = selectedSet.begin(); it != selectedSet.end(); ++it )
    whichSelected.push_back( *it );
}

template <typename SelType>
void SelectionManagement< SelType >::getSelectedSetRange( vector<SelType> &whichSelected,
                                               SelType first,
                                               SelType last )
{
  whichSelected.clear();
  typename vector<SelType>::iterator it;
  for ( it = selectedSet.begin(); it != selectedSet.end(); ++it )
  {
    if (( *it >= first ) && ( *it <= last ))
      whichSelected.push_back( *it );
  }
}

template <typename SelType>
SelType SelectionManagement< SelType >::firstSelected()
{
  return first;
}

template <typename SelType>
SelType SelectionManagement< SelType >::lastSelected()
{
  return last;
}


