/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef WORKSPACE_H_INCLUDED
#define WORKSPACE_H_INCLUDED

#include <string>
#include <vector>

// SERIALIZATION INCLUDES
#include <fstream>
#include <iostream>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "paraverkerneltypes.h"

class Workspace
{
  public:
    Workspace();
    Workspace( std::string whichName ) : name( whichName ) {}
    ~Workspace();

    virtual std::string getName() const;
    virtual std::vector<TEventType> getAutoTypes() const;
    virtual std::vector<std::pair<std::string,std::string> > getHintCFGs() const;
    virtual std::pair<std::string,std::string> getHintCFG( size_t whichHint ) const;

    virtual void setName( std::string& whichName );
    virtual void setAutoTypes( std::vector<TEventType>& whichAutoTypes );
    virtual void addHintCFG( std::pair<std::string,std::string>& whichCFG );
    virtual void addHintCFG( size_t position, std::pair<std::string,std::string>& whichCFG );
    virtual void removeHintCFG( size_t whichHint );
    virtual void modifyHintCFG( size_t position, std::pair<std::string,std::string>& whichCFG );
    virtual void clearHintCFGs();

    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
      ar & boost::serialization::make_nvp( "name", name );
      if( version >= 1 )
        ar & boost::serialization::make_nvp( "autoTypes", autoTypes );
      ar & boost::serialization::make_nvp( "hintCFGs", hintCFGs );
    }

  protected:
    std::string name;
    std::vector<TEventType> autoTypes;
    std::vector<std::pair<std::string,std::string> > hintCFGs; // path, description

  private:

};

BOOST_CLASS_VERSION( Workspace, 1)


#endif // WORKSPACE_H_INCLUDED
