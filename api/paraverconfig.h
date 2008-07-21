#ifndef PARAVERCONFIG_H_INCLUDED
#define PARAVERCONFIG_H_INCLUDED

#include <map>
#include <sstream>
#include "paraverkerneltypes.h"

class ParaverConfig;

class PropertyFunction
{
  public:
    PropertyFunction()
    {}
    virtual ~PropertyFunction()
    {}
    virtual void parseLine( istringstream& line, ParaverConfig& config ) = 0;
};

class ParaverConfig
{
  public:
    ~ParaverConfig();

    static ParaverConfig *getInstance();

    static void readParaverConfigFile();
    static bool writeDefaultConfig();

    UINT32 getPrecision() const;
    TObjectOrder getHistoNumColumns() const;
    bool getShowUnits() const;
    bool getThousandSep() const;

    void setPrecision( UINT32 prec );
    void setHistoNumColumns( TObjectOrder columns );
    void setShowUnits( bool units );
    void setThousandSep( bool sep );

  private:
    static ParaverConfig *instance;

    ParaverConfig();

    map<string, PropertyFunction *> propertyFunctions;
    void loadMap();
    void unLoadMap();

    UINT32 precision;
    TObjectOrder histoNumColumns;
    bool showUnits;
    bool thousandSep;

};

// WhatWhere.num_decimals
class WWNumDecimals: public PropertyFunction
{
    void parseLine( istringstream& line, ParaverConfig& config );
};

// Analyzer2D.num_columns
class HistoNumColumns: public PropertyFunction
{
    void parseLine( istringstream& line, ParaverConfig& config );
};

// Analyzer2D.units
class HistoUnits: public PropertyFunction
{
    void parseLine( istringstream& line, ParaverConfig& config );
};

// Analyzer2D.thousandsep
class HistoThousanSep: public PropertyFunction
{
    void parseLine( istringstream& line, ParaverConfig& config );
};

#endif // PARAVERCONFIG_H_INCLUDED
