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

// Copies selection vector into selectedSet attribute
// Sets firstSelected and lastSelected
// Fills selected vector
template <typename SelType>
void SelectionManagement<SelType>::setSelected( vector< SelType > &selection,
                                                SelType maxElems )
{
  // Search for first and last selected
  selectedSet = selection;

  if ( !selection.empty() )
  {
    bool firstFound = false;
    typename vector< SelType >::iterator it;
    it = selection.begin();
    for ( size_t current = 0; current < ( size_t ) maxElems; ++current )
    {
      if ( current == ( size_t )*it )
      {
        if ( !firstFound )
          first = ( SelType )current;  // caution!

        last = ( SelType )current;
        selected.push_back( true );
        ++it;
      }
      else
        selected.push_back( false );
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
  typename vector<bool>::iterator it;

  for( it = selected.begin(); it != selected.end(); ++it )
    whichSelected.push_back( *it );
}

template <typename SelType>
void SelectionManagement< SelType >::getSelected( vector<SelType> &whichSelected )
{
  whichSelected.clear();
  typename vector<SelType>::iterator it;

  for( it = selectedSet.begin(); it != selectedSet.end(); ++it )
    whichSelected.push_back( *it );
}

template <typename SelType>
void SelectionManagement< SelType >::getSelected( vector<SelType> &whichSelected,
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


