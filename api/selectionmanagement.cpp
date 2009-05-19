template < typename SelType >
SelectionManagement::SelectionManagement()
{
  firstSelected = ( SelType )0;
  lastSelected = ( SelType )0;
  countSelected = 0;
}

template < typename SelType >
~SelectionManagement::SelectionManagement()
{
}

template <typename SelType>
void SelectionManagement::setSelected( vector< bool > &selection )
{
  firstSelected = ( SelType ) 0;
  lastSelected = ( SelType ) 0;
  countSelected = 0;

  // Search for first and last selected
  if ( !selection.empty() )
  {
    for ( size_t current = 0; current < selected.size(); ++current )
    {
      if ( selected[ current ] )
      {
        firstSelected = ( SelType )current;  // caution!
        break;
      }
    }

    for ( size_t current = selected.size() - 1 ; current >= 0; --current )
    {
      if ( selected[ current ] )
      {
        lastSelected = ( SelType )current;  // caution!
        break;
      }
    }

    // Count values
    for ( vector<bool>::iterator it = selected.begin(); it != selected.end(); ++it )
    {
      if ( *it )
        ++countSelected;
    }

    // Copy; doesn't modify parameter selection.
    vector<bool> aux( selection );
    selected.swap( aux );
  }
}

template <typename SelType>
bool SelectionManagement::getSelectedPosition( SelType &selected )
{
  return selected[ SelType ];
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
