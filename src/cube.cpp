#include <string>
#include <iostream>

using namespace std;

//#include "cube.h"

template <typename ValueType>
Cube<ValueType>::Cube( int numPlanes, int numCols, short numStats )
{
  Matrix2D<ValueType> *tmp_mat = NULL;

  nplanes = 0;
  ncols = numCols;
  nstat = numStats;
  crow = 0;

  for( int ii = 0; ii < numPlanes; ii++ )
  {
    planes.push_back( tmp_mat );
  }
}


template <typename ValueType>
Cube<ValueType>::~Cube()
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      if( planes[ ii ] != NULL )
        delete planes[ ii ];
    }
  }
  planes.clear();
}


template <typename ValueType>
void Cube<ValueType>::Init( short idStat )
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      if( planes[ ii ] != NULL )
        planes[ ii ]->Init( idStat );
    }
  }
}


template <typename ValueType>
void Cube<ValueType>::Init( )
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      if( planes[ ii ] != NULL )
        planes[ ii ]->Init( );
    }
  }
}


template <typename ValueType>
void Cube<ValueType>::SetValue( int plane, int col, short idStat, ValueType semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->SetValue( col, idStat, semVal );
}


template <typename ValueType>
void Cube<ValueType>::SetValue( int plane, int col, ValueType semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->SetValue( col, semVal );
}


template <typename ValueType>
void Cube<ValueType>::AddValue( int plane, int col, short idStat, ValueType semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->AddValue( col, idStat, semVal );
}


template <typename ValueType>
void Cube<ValueType>::AddValue( int plane, int col, ValueType semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<ValueType>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->AddValue( col, semVal );
}


template <typename ValueType>
ValueType Cube<ValueType>::GetCurrentValue( int plane, int col, short idStat ) const
{
  if( planes[ plane ] != NULL )
    return planes[ plane ]->GetCurrentValue( col, idStat );

  ValueType tmp;

  bzero( (void *) &tmp, sizeof( ValueType ) );
  return tmp;
}


template <typename ValueType>
int Cube<ValueType>::GetCurrentRow( int plane, int col ) const
{
  if( planes[ plane ] != NULL )
    return planes[ plane ]->GetCurrentRow( col );
  return -1;
}


template <typename ValueType>
int Cube<ValueType>::GetCurrentRow( ) const
{
  return crow;
}


template <typename ValueType>
void Cube<ValueType>::NewRow( )
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      if( planes[ ii ] != NULL )
        planes[ ii ]->NewRow( );
    }
  }
  crow++;
}


template <typename ValueType>
void Cube<ValueType>::NewRow( int plane, int col, int row )
{
  if( nplanes > 0 )
  {
    if( planes[ plane ] != NULL )
      planes[ plane ]->NewRow( col, row );
  }

  crow = row;
}


template <typename ValueType>
void Cube<ValueType>::Finish( )
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      if( planes[ ii ] != NULL )
        planes[ ii ]->Finish();
    }
  }
}


template <typename ValueType>
void Cube<ValueType>::SetNextCell( int plane, int col )
{
  if( nplanes == 0 )
    return;

  if( planes[ plane ] != NULL )
    planes[ plane ]->SetNextCell( col );
}


template <typename ValueType>
void Cube<ValueType>::SetFirstCell( int plane, int col )
{
  if( nplanes == 0 )
    return;

  if( planes[ plane ] != NULL )
    planes[ plane ]->SetFirstCell( col );
}


template <typename ValueType>
bool Cube<ValueType>::EndCell( int plane, int col )
{
  if( nplanes == 0 )
    return true;

  if( planes[ plane ] != NULL )
    return planes[ plane ]->EndCell( col );
  return true;
}


template <typename ValueType>
bool Cube<ValueType>::PlaneWithValues( int plane ) const
{
  if( nplanes == 0 )
    return false;

  return ( planes[ plane ] != NULL );
}


template <typename ValueType>
int Cube<ValueType>::GetPlanes( ) const
{
  if( nplanes > 0 )
    return planes.size();

  return nplanes;
}


template <typename ValueType>
void Cube<ValueType>::EraseColumns( int ini_col, int fin_col )
{
  if( fin_col < ini_col )
    return;

  if( ini_col < 0 )
    return;

  if( fin_col >= ncols )
    return;

  for( unsigned int ii = 0; ii < planes.size(); ii++ )
  {
    if( planes[ ii ] != NULL )
      planes[ ii ]->EraseColumns( ini_col, fin_col );
  }

  ncols = ncols - ( fin_col - ini_col );
}


template <typename ValueType>
void Cube<ValueType>::ErasePlanes( int ini_plane, int fin_plane )
{
  if( fin_plane < ini_plane )
    return;

  if( ini_plane < 0 )
    return;

  if( fin_plane >= (int) planes.size() )
    return;

  typename vector<Matrix2D<ValueType> *>::iterator it_ini, it_fin;
  int i;

  it_ini = planes.begin();
  for( i = 0; i < ini_plane; i++, it_ini++ );

  for( it_fin = it_ini; i < fin_plane; i++, it_fin++ )
  {
    if( planes[ i ] != NULL )
    {
      delete planes[ i ];
      nplanes--;
    }
  }

  planes.erase( it_ini, it_fin );
}


template <typename ValueType>
void Cube<ValueType>::Print() const
{
  if( nplanes > 0 )
  {
    for( unsigned int ii = 0; ii < planes.size(); ii++ )
    {
      cout << endl;
      cout << "******************************************************" << endl;
      cout << "----------Plane " << ii << "----------" << endl;
      if( planes[ ii ] != NULL )
        planes[ ii ]->Print();
    }
  }
}
