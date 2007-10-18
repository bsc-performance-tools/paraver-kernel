#include <string>
#include <iostream>

using namespace std;

#include "cube.h"


template <typename TipusBasic>
Cube<TipusBasic>::Cube( int numPlanes, int numCols, short numStats )
{
  Matrix2D<TipusBasic> *tmp_mat = NULL;
  
  nplanes = 0;
  ncols = numCols;
  nstat = numStats;
  crow = 0;
  
  for( int ii = 0; ii < numPlanes; ii++ )
  {
    planes.push_back( tmp_mat );
  }
}


template <typename TipusBasic>
Cube<TipusBasic>::~Cube()
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


template <typename TipusBasic>
void Cube<TipusBasic>::Init( short idStat )
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


template <typename TipusBasic>
void Cube<TipusBasic>::Init( )
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


template <typename TipusBasic>
void Cube<TipusBasic>::SetValue( int plane, int col, short idStat, TipusBasic semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<TipusBasic>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->SetValue( col, idStat, semVal );
}


template <typename TipusBasic>
void Cube<TipusBasic>::SetValue( int plane, int col, TipusBasic semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<TipusBasic>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->SetValue( col, semVal );
}


template <typename TipusBasic>
void Cube<TipusBasic>::AddValue( int plane, int col, short idStat, TipusBasic semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<TipusBasic>( crow, ncols, nstat );
    nplanes++;
  }
  
  planes[ plane ]->AddValue( col, idStat, semVal );
}


template <typename TipusBasic>
void Cube<TipusBasic>::AddValue( int plane, int col, TipusBasic semVal )
{
  if( planes[ plane ] == NULL )
  {
    planes[ plane ] = new Matrix2D<TipusBasic>( crow, ncols, nstat );
    nplanes++;
  }

  planes[ plane ]->AddValue( col, semVal );
}


template <typename TipusBasic>
TipusBasic Cube<TipusBasic>::GetCurrentValue( int plane, int col, short idStat ) const
{
  if( planes[ plane ] != NULL )
    return planes[ plane ]->GetCurrentValue( col, idStat );
  
  TipusBasic tmp;
  
  bzero( (void *) &tmp, sizeof( TipusBasic ) );
  return tmp;
}


template <typename TipusBasic>
int Cube<TipusBasic>::GetCurrentRow( int plane, int col ) const
{
  if( planes[ plane ] != NULL )
    return planes[ plane ]->GetCurrentRow( col );
  return -1;
}


template <typename TipusBasic>
int Cube<TipusBasic>::GetCurrentRow( ) const
{
  return crow;
}


template <typename TipusBasic>
void Cube<TipusBasic>::NewRow( )
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


template <typename TipusBasic>
void Cube<TipusBasic>::NewRow( int plane, int col, int row )
{
  if( nplanes > 0 )
  {
    if( planes[ plane ] != NULL )
      planes[ plane ]->NewRow( col, row );
  }
    
  crow = row;
}


template <typename TipusBasic>
void Cube<TipusBasic>::Finish( )
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


template <typename TipusBasic>
void Cube<TipusBasic>::SetNextCell( int plane, int col )
{
  if( nplanes == 0 )
    return;
  
  if( planes[ plane ] != NULL )
    planes[ plane ]->SetNextCell( col );
}


template <typename TipusBasic>
void Cube<TipusBasic>::SetFirstCell( int plane, int col )
{
  if( nplanes == 0 )
    return;
  
  if( planes[ plane ] != NULL )
    planes[ plane ]->SetFirstCell( col );
}


template <typename TipusBasic>
bool Cube<TipusBasic>::EndCell( int plane, int col )
{
  if( nplanes == 0 )
    return true;
  
  if( planes[ plane ] != NULL )
    return planes[ plane ]->EndCell( col );
  return true;
}


template <typename TipusBasic>
bool Cube<TipusBasic>::PlaneWithValues( int plane ) const
{
  if( nplanes == 0 )
    return false;
  
  return ( planes[ plane ] != NULL );
}


template <typename TipusBasic>
int Cube<TipusBasic>::GetPlanes( ) const
{
  if( nplanes > 0 )
    return planes.size();
  
  return nplanes;
}


template <typename TipusBasic>
void Cube<TipusBasic>::EraseColumns( int ini_col, int fin_col )
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


template <typename TipusBasic>
void Cube<TipusBasic>::ErasePlanes( int ini_plane, int fin_plane )
{
  if( fin_plane < ini_plane )
    return;
  
  if( ini_plane < 0 )
    return;
  
  if( fin_plane >= (int) planes.size() )
    return;
  
  typename vector<Matrix2D<TipusBasic> *>::iterator it_ini, it_fin;
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


template <typename TipusBasic>
void Cube<TipusBasic>::Print() const
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
