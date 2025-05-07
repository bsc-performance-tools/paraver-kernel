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

#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <map>

#include "paraverkerneltypes.h"
#include "semanticcolor.h"


/******************************************************************************
 ***                      MetadataAction                                    ***
 ******************************************************************************/
class MetadataAction
{
  public:
    static constexpr int FIELD_COUNT = 4;
    static constexpr char RUNAPP_ORIGIN_ID[] = "RUNAPP";
    static constexpr char MANUAL_ORIGIN_ID[] = "MANUAL";

    MetadataAction() { error = false; }

    MetadataAction( std::string date,
                    std::string action,
                    std::string origin,
                    std::string originalTrace );

    std::string getDate()          { return date; }
    std::string getAction()        { return action; }
    std::string getOrigin()        { return origin; }
    std::string getOriginalTrace() { return originalTrace; }

    bool getError() { return error; }
    std::string getErrorMessage() { return errorMessage; }

    void write( std::ostream& os ) const;

  protected:
    bool error;
    std::string errorMessage;

    std::string date;
    std::string action;
    std::string origin;
    std::string originalTrace;

  private:
    virtual void flushSpecificFields( std::ostream& os ) const = 0;
};

std::ostream& operator<<( std::ostream& os, const MetadataAction& metadataRecord );


/******************************************************************************
 ***                      MetadataApplication                               ***
 ******************************************************************************/
class MetadataApplication
{
  public:
    virtual bool newMetadata( std::vector< std::string > metadataFields ) = 0;
};


/******************************************************************************
 ***                      CutterMetadataAction                              ***
 ******************************************************************************/
class CutterMetadataAction : public MetadataAction
{
  public:
    static constexpr int  FIELD_COUNT = MetadataAction::FIELD_COUNT + 3;
    static constexpr char ACTION_ID[] = "CUTTER";
  
    CutterMetadataAction( std::vector<std::string>& cutterMetadataFields );
  
    CutterMetadataAction( std::string date,
                          std::string origin,
                          std::string originalTrace,
                          PRV_UINT64 offset,
                          PRV_UINT64 beginTime,
                          PRV_UINT64 endTime );
    
    PRV_UINT64 getOffset() const    { return offset; }
    PRV_UINT64 getBeginTime() const { return beginTime; }
    PRV_UINT64 getEndTime() const   { return endTime; }
    
  private:
    PRV_UINT64 beginTime;
    PRV_UINT64 offset;
    PRV_UINT64 endTime;

    void flushSpecificFields( std::ostream& os ) const;
};


/******************************************************************************
 ***                      CSVMetadataAction                                 ***
 ******************************************************************************/
class CSVMetadataAction : public MetadataAction
{
  public:
    static constexpr int  FIELD_COUNT = MetadataAction::FIELD_COUNT + 2;
    static constexpr char ACTION_ID[] = "CSV";
  
    CSVMetadataAction( std::vector<std::string>& csvMetadataFields );

    CSVMetadataAction( std::string    date,
                       std::string    origin,
                       std::string    originalTrace,
                       TTimeUnit      timeUnits,
                       TColorFunction colorMode );

    TTimeUnit      getTimeUnits() const { return timeUnits; }
    TColorFunction getColorMode() const { return colorMode; }

  private:
    TTimeUnit      timeUnits;
    TColorFunction colorMode;

    void flushSpecificFields( std::ostream& os ) const;
};


/******************************************************************************
 ***                      CutterMetadata                                    ***
 ******************************************************************************/
class CutterMetadata : public MetadataApplication
{
  public:
    virtual bool newMetadata( std::vector< std::string > metadataFields );

    const std::vector< CutterMetadataAction >& getMetadata() const;

    PRV_UINT64 getCutterLastOffset() const;
    PRV_UINT64 getCutterLastBeginTime() const;
    PRV_UINT64 getCutterLastEndTime() const;
    PRV_UINT64 getCutterTotalOffset() const;

  private:
    std::vector< CutterMetadataAction > metadataActions;

    PRV_UINT64 totalOffset = 0;
};


/******************************************************************************
 ***                      CSVMetadata                                       ***
 ******************************************************************************/
class CSVMetadata : public MetadataApplication
{
  public:
    virtual bool newMetadata( std::vector< std::string > metadataFields );

    const std::vector< CSVMetadataAction >& getMetadata() const;

    TTimeUnit      getTimeUnits() const;
    TColorFunction getColorMode() const;

  private:
    std::vector< CSVMetadataAction > metadataActions;
};

/******************************************************************************
 ***                      MetadataManager                                   ***
 ******************************************************************************/
class MetadataManager
{
  public:
    enum class TAppID
    {
      CUTTER,
      CSV
    };

    static std::string getCurrentDate();

    bool newMetadata( std::string metadataStr );

    template< TAppID appID, typename T >
    bool getMetadata( T *& whichMetadata ) const;

    bool getError() const;
    std::string getErrorMessage() const;

  private:
    bool error = false;
    std::string errorMessage{};

    std::map< TAppID, MetadataApplication* > metadataContainer;

    void populateRecord( std::vector<std::string>& record,
                         const std::string&        line,
                         char                      delimiter );
};


template< MetadataManager::TAppID appID, typename T >
bool MetadataManager::getMetadata( T *& whichMetadata ) const
{
  auto it = metadataContainer.find( appID );
  if( it == metadataContainer.end() )
    return false;

  whichMetadata = static_cast< T * >( it->second );

  return true;
}


