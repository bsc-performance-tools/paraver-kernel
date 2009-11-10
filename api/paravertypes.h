#ifndef PARAVERTYPES_H_INCLUDED
#define PARAVERTYPES_H_INCLUDED

// SERIALIZATION INCLUDES
#include <fstream>
#include <iostream>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

typedef unsigned char ParaverColor;

enum colorIndex { RED, GREEN, BLUE };

struct rgb
{
  template< class Archive >
  void serialize( Archive & ar, const unsigned int version )
  {
    ar & boost::serialization::make_nvp( "red", red );
    ar & boost::serialization::make_nvp( "green", green );
    ar & boost::serialization::make_nvp( "blue", blue );
  }


  ParaverColor red;
  ParaverColor green;
  ParaverColor blue;
};

#endif // PARAVERTYPES_H_INCLUDED
