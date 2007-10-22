#ifndef _CUBE_H
#define _CUBE_H

#include <vector>
#include "matrix.h"

template <typename ValueType>
class Cube
{
  public:
    Cube( UINT32 numPlanes, UINT32 numCols, UINT16 numStats );
    ~Cube();

    void Init( UINT16 idStat );
    void Init( );
    void SetValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal );
    void SetValue( UINT32 plane, UINT32 col, ValueType semVal );
    void AddValue( UINT32 plane, UINT32 col, UINT16 idStat, ValueType semVal );
    void AddValue( UINT32 plane, UINT32 col, ValueType semVal );
    ValueType GetCurrentValue( UINT32 plane, UINT32 col, UINT16 idStat ) const;
    UINT32 GetCurrentRow( UINT32 plane, UINT32 col ) const;
    UINT32 GetCurrentRow( ) const;
    void NewRow( );
    void NewRow( UINT32 plane, UINT32 col, UINT32 row );
    void Finish( );
    void SetNextCell( UINT32 plane, UINT32 col );
    void SetFirstCell( UINT32 plane, UINT32 col );
    bool EndCell( UINT32 plane, UINT32 col );
    bool PlaneWithValues( UINT32 plane ) const;
    UINT32 GetPlanes( ) const;
    void EraseColumns( UINT32 ini_col, UINT32 fin_col );
    void ErasePlanes( UINT32 ini_plane, UINT32 fin_plane );

    void Print() const;

  private:
    vector<Matrix<ValueType> *> planes;
    UINT32 nplanes;
    UINT32 ncols;
    UINT32 nstat;
    UINT32 crow;
};

#include "src/cube.cpp"

#endif // _CUBE_H
