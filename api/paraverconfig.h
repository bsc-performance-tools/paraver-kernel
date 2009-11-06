#ifndef PARAVERCONFIG_H_INCLUDED
#define PARAVERCONFIG_H_INCLUDED

#include <map>
#include <sstream>
#include "paraverkerneltypes.h"

// SERIALIZATION INCLUDES
#include <fstream>
#include <iostream>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


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
    ParaverConfig();
    ~ParaverConfig();

    static ParaverConfig *getInstance();

    void readParaverConfigFile();
    static void writeParaverConfigFile();
    static bool writeDefaultConfig();

    UINT32 getPrecision() const;
    TObjectOrder getHistoNumColumns() const;
    bool getShowUnits() const;
    bool getThousandSep() const;
    bool getFillStateGaps() const;

    void setPrecision( UINT32 prec );
    void setHistoNumColumns( TObjectOrder columns );
    void setShowUnits( bool units );
    void setThousandSep( bool sep );
    void setFillStateGaps( bool sep );

    void saveXML( const string &filename );
    void loadXML( const string &filename );

  private:
    friend class boost::serialization::access;

    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
//      cout << "serialize begin" << endl;
//      ar & boost::serialization::make_nvp( "2DDecimalPrecision", precision );
      ar & BOOST_SERIALIZATION_NVP( precision );
//      cout << "serialize 1" << endl;
//      ar & boost::serialization::make_nvp( "2DNumberOfColumns", histoNumColumns );
      ar & BOOST_SERIALIZATION_NVP( histoNumColumns );
//      cout << "serialize 2" << endl;
      //ar & boost::serialization::make_nvp( "2DShowUnits", showUnits );
      ar & BOOST_SERIALIZATION_NVP( showUnits );
//      cout << "serialize 3" << endl;
      //ar & boost::serialization::make_nvp( "2DThousandSeparator", thousandSep );
      ar & BOOST_SERIALIZATION_NVP( thousandSep );
//      cout << "serialize 4" << endl;
      //ar & boost::serialization::make_nvp( "GlobalFillStateGaps", fillStateGaps );
      ar & BOOST_SERIALIZATION_NVP( fillStateGaps );
//      cout << "serialize end" << endl;
    }

    static ParaverConfig *instance;


    map<string, PropertyFunction *> propertyFunctions;
    void loadMap();
    void unLoadMap();

    UINT32 precision;
    TObjectOrder histoNumColumns;
    bool showUnits;
    bool thousandSep;
    bool fillStateGaps;
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
