#include <string>
#include <iostream>

using namespace std;


template <typename ValueType>
Cube<ValueType>::Cube( UINT32 numPlanes, UINT32 numCols, UINT16 numStats ):
    nplanes( 0 ), ncols( numCols ), nstat( numStats ), crow( 0 )
{
  Matrix<ValueType> *tmp_mat = NULL;

  for ( UINT32 ii = 0; ii < numPlanes; ii++ )
  {
    planes.push_back( tmp_mat );
  }
}


template <typename ValueType>
Cube<ValueType>::~Cube()
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      if ( planes[ ii ] != NULL )
        delete planes[ ii ];
    }
  }
  planes.clear();
}


template <typename ValueType>
inline void Cube<ValueType>::init( UINT16 idStat )
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      if ( planes[ ii ] != NULL )
        planes[ ii ]->init( idStat );
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::init( )
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      if ( planes[ ii ] != NULL )
        planes[ ii ]->init( );
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::setValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal )
{
  if ( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->setValue( col, idStat, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::setValue( UINT32 plane, UINT32 col, ValueType semVal )
{
  if ( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->setValue( col, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::addValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal )
{
  if ( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->addValue( col, idStat, semVal );
}


template <typename ValueType>
inline void Cube<ValueType>::addValue( UINT32 plane, UINT32 col, ValueType semVal )
{
  if ( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->addValue( col, semVal );
}


template <typename ValueType>
inline ValueType Cube<ValueType>::getCurrentValue( UINT32 plane, UINT32 col, UINT16 idStat ) const
{
  if ( planes[ plane ] != NULL )
    return planes[ plane ]->getCurrentValue( col, idStat );

  ValueType tmp;

  memset( ( void * ) &tmp, 0, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
inline UINT32 Cube<ValueType>::getCurrentRow( UINT32 plane, UINT32 col ) const
{
  if ( planes[ plane ] != NULL )
    return planes[ plane ]->getCurrentRow( col );
  return -1;
}


template <typename ValueType>
inline UINT32 Cube<ValueType>::getCurrentRow( ) const
{
  return crow;
}

template <typename ValueType>
inline bool Cube<ValueType>::currentCellModified( UINT32 plane, UINT32 col ) const
{
  if ( planes[ plane ] != NULL )
    return planes[ plane ]->currentCellModified( col );
  return false;
}


template <typename ValueType>
inline void Cube<ValueType>::newRow( )
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      if ( planes[ ii ] != NULL )
        planes[ ii ]->newRow( );
    }
  }
  crow++;
}


template <typename ValueType>
inline void Cube<ValueType>::newRow( UINT32 plane, UINT32 col, UINT32 row )
{
  if ( nplanes > 0 )
  {
    if ( planes[ plane ] != NULL )
      planes[ plane ]->newRow( col, row );
  }

  crow = row;
}


template <typename ValueType>
inline void Cube<ValueType>::finish( )
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      if ( planes[ ii ] != NULL )
        planes[ ii ]->finish();
    }
  }
}


template <typename ValueType>
inline void Cube<ValueType>::setNextCell( UINT32 plane, UINT32 col )
{
  if ( nplanes == 0 )
    return;

  if ( planes[ plane ] != NULL )
    planes[ plane ]->setNextCell( col );
}


template <typename ValueType>
inline void Cube<ValueType>::setFirstCell( UINT32 plane, UINT32 col )
{
  if ( nplanes == 0 )
    return;

  if ( planes[ plane ] != NULL )
    planes[ plane ]->setFirstCell( col );
}


template <typename ValueType>
inline bool Cube<ValueType>::endCell( UINT32 plane, UINT32 col )
{
  if ( nplanes == 0 )
    return true;

  if ( planes[ plane ] != NULL )
    return planes[ plane ]->endCell( col );
  return true;
}


template <typename ValueType>
inline bool Cube<ValueType>::planeWithValues( UINT32 plane ) const
{
  if ( nplanes == 0 )
    return false;

  return ( planes[ plane ] != NULL );
}


template <typename ValueType>
inline UINT32 Cube<ValueType>::getPlanes( ) const
{
  if ( nplanes > 0 )
    return planes.size();

  return nplanes;
}


template <typename ValueType>
inline void Cube<ValueType>::eraseColumns( UINT32 ini_col, UINT32 fin_col )
{
  if ( fin_col < ini_col )
    return;

  if ( ini_col < 0 )
    return;

  if ( fin_col >= ncols )
    return;

  for ( UINT32 ii = 0; ii < planes.size(); ii++ )
  {
    if ( planes[ ii ] != NULL )
      planes[ ii ]->eraseColumns( ini_col, fin_col );
  }

  ncols = ncols - ( fin_col - ini_col );
}


template <typename ValueType>
inline void Cube<ValueType>::erasePlanes( UINT32 ini_plane, UINT32 fin_plane )
{
  if ( fin_plane < ini_plane )
    return;

  if ( ini_plane < 0 )
    return;

  if ( fin_plane >= ( UINT32 ) planes.size() )
    return;

  typename vector<Matrix<ValueType> *>::iterator it_ini, it_fin;
  UINT32 i;

  it_ini = planes.begin();
  for ( i = 0; i < ini_plane; i++, it_ini++ );

  for ( it_fin = it_ini; i < fin_plane; i++, it_fin++ )
  {
    if ( planes[ i ] != NULL )
    {
      delete planes[ i ];
      nplanes--;
    }
  }

  planes.erase( it_ini, it_fin );
}


template <typename ValueType>
inline void Cube<ValueType>::print() const
{
  if ( nplanes > 0 )
  {
    for ( UINT32 ii = 0; ii < planes.size(); ii++ )
    {
      cout << endl;
      cout << "******************************************************" << endl;
      cout << "----------Plane " << ii << "----------" << endl;
      if ( planes[ ii ] != NULL )
        planes[ ii ]->print();
    }
  }
}
