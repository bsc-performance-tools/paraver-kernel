#ifndef _CUBE_H
#define _CUBE_H

#include <vector>
#include "matrix.h"

template <typename ValueType>
class Cube
{
  public:
    Cube( int numPlanes, int numCols, short numStats );
    ~Cube();

    void Init( short idStat );
    void Init( );
    void SetValue( int plane, int col, short idStat, ValueType semVal );
    void SetValue( int plane, int col, ValueType semVal );
    void AddValue( int plane, int col, short idStat, ValueType semVal );
    void AddValue( int plane, int col, ValueType semVal );
    ValueType GetCurrentValue( int plane, int col, short idStat ) const;
    int GetCurrentRow( int plane, int col ) const;
    int GetCurrentRow( ) const;
    void NewRow( );
    void NewRow( int plane, int col, int row );
    void Finish( );
    void SetNextCell( int plane, int col );
    void SetFirstCell( int plane, int col );
    bool EndCell( int plane, int col );
    bool PlaneWithValues( int plane ) const;
    int GetPlanes( ) const;
    void EraseColumns( int ini_col, int fin_col );
    void ErasePlanes( int ini_plane, int fin_plane );

    void Print() const;

  private:
    vector<Matrix2D<ValueType> *> planes;
    int nplanes;
    int ncols;
    int nstat;
    int crow;
};

//#include "src/cube.cpp"

#endif // _CUBE_H
