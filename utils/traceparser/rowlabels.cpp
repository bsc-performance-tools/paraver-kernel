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


#include <fstream>
#include <sstream>
#include <algorithm>

#include "paraverlabels.h"

template<typename dummy>
RowLabels<dummy>::RowLabels( const std::string& filename )
{
  std::string strLine;
  std::string strLevel;
  std::string strSize;
  std::istringstream auxStream;
  std::istringstream sizeStream;

  globalMaxLength = 0;

  std::ifstream rowFile( filename.c_str() );
  if ( !rowFile )
    return;

  while ( !rowFile.eof() )
  {
    std::vector<std::string> *tmpvector = nullptr;

    getline( rowFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' )
      continue;

    auxStream.clear();
    auxStream.str( strLine );
    getline( auxStream, strLevel, ' ' ); // 'LEVEL'
    getline( auxStream, strLevel, ' ' );

    TWindowLevel level;
    if ( strLevel == LEVEL_WORKLOAD )
      level = WORKLOAD;
    else if ( strLevel == LEVEL_APPLICATION )
      level = APPLICATION;
    else if ( strLevel == LEVEL_TASK )
      level = TASK;
    else if ( strLevel == LEVEL_THREAD )
      level = THREAD;
    else if ( strLevel == LEVEL_SYSTEM )
      level = SYSTEM;
    else if ( strLevel == LEVEL_NODE )
      level = NODE;
    else if ( strLevel == LEVEL_CPU )
      level = CPU;
    else
      continue;

    std::map<TWindowLevel, std::tuple< std::string, size_t, std::vector<std::string> > >::iterator currentLevelLabels;
    std::tuple< std::string, size_t, std::vector<std::string> > tmpEmptyLevel( strLevel, 0, std::vector<std::string>() );
    std::tie( currentLevelLabels, std::ignore ) = levelLabels.insert( std::make_pair( level, tmpEmptyLevel ) );

    getline( auxStream, strSize, 'S' ); // 'SIZE'
    getline( auxStream, strSize, ' ' ); // 'SIZE'
    getline( auxStream, strSize );

    sizeStream.clear();
    sizeStream.str( strSize );
    int size;

    if ( !( sizeStream >> size ) )
      continue;

    size_t currentLength;
    int i = 0;
    while ( !rowFile.eof() && i < size )
    {
      getline( rowFile, strLine );
      if( strLine[ strLine.length() - 1 ] == '\r' )
        strLine.resize( strLine.length() - 1 );
      std::get<2>( currentLevelLabels->second ).push_back( strLine );

      currentLength = strLine.length();

      // By level
      if ( std::get<1>( currentLevelLabels->second ) < currentLength )
        std::get<1>( currentLevelLabels->second ) = currentLength;

      // Global
      if ( globalMaxLength < currentLength )
        globalMaxLength = currentLength;

      ++i;
    }
  }

  rowFile.close();
}


template<typename dummy>
void RowLabels<dummy>::dumpToFile( const std::string& filename ) const
{
  std::ofstream rowFile( filename.c_str() );
  if ( !rowFile )
    return;

  for( int i = CPU; i >= WORKLOAD; --i )
  {
    std::map<TWindowLevel, std::tuple< std::string, size_t, std::vector<std::string> > >::const_iterator currentLevelLabels;
    if( ( currentLevelLabels = levelLabels.find( static_cast<TWindowLevel>( i ) ) ) != levelLabels.end() )
      dumpLevel( currentLevelLabels->second, rowFile );
  }

  rowFile.close();
}


template<typename dummy>
void RowLabels<dummy>::dumpLevel( const std::tuple< std::string, size_t, std::vector<std::string> >& whichLevel, std::ofstream& whichFile ) const
{
  if( !std::get<2>( whichLevel ).empty() )
  {
    whichFile << "LEVEL " << std::get<0>( whichLevel ) << " SIZE " << std::get<2>( whichLevel ).size() << std::endl;

    std::copy( std::get<2>( whichLevel ).begin(), std::get<2>( whichLevel ).end(), std::ostream_iterator<std::string>( whichFile, "\n" ) );

    whichFile << std::endl;
  }
}


template<typename dummy>
std::string RowLabels<dummy>::getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
{
  std::map<TWindowLevel, std::tuple< std::string, size_t, std::vector<std::string> > >::const_iterator currentLevelLabels;
  if( ( currentLevelLabels = levelLabels.find( whichLevel ) ) == levelLabels.end() )
    return "";

  const std::vector<std::string>& vectorLabels = std::get<2>( currentLevelLabels->second );
  if( vectorLabels.empty() || vectorLabels.size() <= whichRow )
    return "";

  return vectorLabels[ whichLevel ];
}


template<typename dummy>
void RowLabels<dummy>::pushBack( TWindowLevel whichLevel, const std::string& rowLabel )
{
  std::map<TWindowLevel, std::tuple< std::string, size_t, std::vector<std::string> > >::iterator currentLevelLabels;
  if( ( currentLevelLabels = levelLabels.find( whichLevel ) ) == levelLabels.end() )
  {
    std::tuple< std::string, size_t, std::vector<std::string> > tmpCurrentLevel( LABEL_LEVELS[ whichLevel ], 0, std::vector<std::string>() );
    std::tie( currentLevelLabels, std::ignore ) = levelLabels.insert( std::make_pair( whichLevel, tmpCurrentLevel ) );
  }

  std::get<2>( currentLevelLabels->second ).push_back( rowLabel );

  if( std::get<1>( currentLevelLabels->second ) < rowLabel.length() )
    std::get<1>( currentLevelLabels->second ) = rowLabel.length();

  if( globalMaxLength < rowLabel.length() )
    globalMaxLength = rowLabel.length();
}

// whichLevel == NONE (by default) ==> all levels MaxLength
template<typename dummy>
size_t RowLabels<dummy>::getMaxLength( TWindowLevel whichLevel ) const
{
  if( whichLevel == NONE )
    return globalMaxLength;

  std::map<TWindowLevel, std::tuple< std::string, size_t, std::vector<std::string> > >::const_iterator currentLevelLabels;
  if( ( currentLevelLabels = levelLabels.find( whichLevel ) ) == levelLabels.end() )
    return 0;

  return std::get<1>( currentLevelLabels->second );
}
