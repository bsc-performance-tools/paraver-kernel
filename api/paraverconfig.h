#ifndef PARAVERCONFIG_H_INCLUDED
#define PARAVERCONFIG_H_INCLUDED

#include <map>
#include <sstream>

#include "paraverkerneltypes.h"
#include "drawmode.h"
#include "semanticcolor.h"
#include "paravertypes.h"

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

    enum TImageFormat
    {
      BMP = 0,
      JPG,
      PNG,
      XPM
    };

    enum TTextFormat // << also matrix-row format
    {
      CSV = 0,
      PLAIN
    };

    ParaverConfig();
    ~ParaverConfig();

    static ParaverConfig *getInstance();

    void readParaverConfigFile();
    static void writeParaverConfigFile();
    static bool writeDefaultConfig();

    // GLOBAL XML SECTION
    void setGlobalTracesPath( string whichTracesPath );
    void setGlobalCFGsPath( string whichCfgsPath );
    void setGlobalTmpPath( string whichTmpPath );
    void setGlobalApplyFollowingCFGsToAllTraces( bool whichApplyFollowingCFGsToAllTraces );
    void setGlobalFillStateGaps( bool whichFillStateGaps );

    string getGlobalTracesPath() const;
    string getGlobalCFGsPath() const;
    string getGlobalTmpPath() const;
    bool getGlobalApplyFollowingCFGsToAllTraces() const;
    bool getGlobalFillStateGaps() const;

    // TIMELINES XML SECTION
    void setTimelineDefaultName( string whichDefaultName );
    void setTimelineNameFormat( string whichNameFormat );
    void setTimelineDefaultCFG( string whichDefaultCFG );
    void setTimelinePrecision( UINT32 whichPrecision );
    void setTimelineViewEventsLines( bool whichViewEventLines );
    void setTimelineViewCommunicationsLines( bool whichViewCommunicationsLines );
    void setTimelineViewFunctionAsColor( bool whichViewFunctionAsColor );
    void setTimelineColor( SemanticColor::TColorFunction whichColor );
    void setTimelineDrawmodeTime( DrawModeMethod whichDrawmodeTime );
    void setTimelineDrawmodeObjects( DrawModeMethod whichDrawmodeObjects );
    void setTimelineDrawmodeBoth( DrawModeMethod whichDrawmodeBoth );
    void setTimelineGradientFunction( GradientColor::TGradientFunction whichGradientFunction );
    void setTimelinePixelSize( UINT32 whichPixelSize );
    void setTimelineWhatWhereSemantic( bool whichWhatWhereSemantic );
    void setTimelineWhatWhereEvents( bool whichWhatWhereEvents );
    void setTimelineWhatWhereCommunications( bool whichWhatWhereCommunications );
    void setTimelineWhatWherePreviousNext( bool whichWhatWherePreviousNext );
    void setTimelineWhatWhereText( bool whichWhatWhereText );
    void setTimelineSaveTextFormat( TTextFormat whichSaveTextFormat );
    void setTimelineSaveImageFormat( TImageFormat whichSaveImageFormat );

    string getTimelineDefaultName() const;
    string getTimelineNameFormat() const;
    string getTimelineDefaultCFG() const;
    UINT32 getTimelinePrecision() const;
    bool getTimelineViewEventsLines() const;
    bool getTimelineViewCommunicationsLines() const;
    bool getTimelineViewFunctionAsColor() const;
    SemanticColor::TColorFunction getTimelineColor() const;
    DrawModeMethod getTimelineDrawmodeTime() const;
    DrawModeMethod getTimelineDrawmodeObjects() const;
    DrawModeMethod getTimelineDrawmodeBoth() const;
    GradientColor::TGradientFunction getTimelineGradientFunction() const;
    UINT32 getTimelinePixelSize() const;
    bool getTimelineWhatWhereSemantic() const;
    bool getTimelineWhatWhereEvents() const;
    bool getTimelineWhatWhereCommunications() const;
    bool getTimelineWhatWherePreviousNext() const;
    bool getTimelineWhatWhereText() const;
    TTextFormat getTimelineSaveTextFormat() const;
    TImageFormat getTimelineSaveImageFormat() const;

    // HISTOGRAM
    void setHistogramViewZoom( bool whichViewZoom );
    void setHistogramviewGradientZoom( bool whichViewGradientZoom );
    void setHistogramviewHorizontal( bool whichViewHorizontal );
    void setHistogramviewEmptyColumns( bool whichViewEmptyColumns );
    void setHistogramscientificNotation( bool whichScientificNotation );
    void setHistogramThousandSep( bool whichThousandSep );
    void setHistogramPrecision( UINT32 whichPrecision );
    void setHistogramShowUnits( bool whichShowUnits );
    void setHistogramNumColumns( TObjectOrder whichNumColumns );
    void setHistogramAutofitControlScale( bool whichAutofitControlScale );
    void setHistogramAutofitDataGradient( bool whichAutofitDataGradient );
    void setHistogramAutofitThirdDimensionScale( bool whichAutofitThirdDimensionScale );
    void setHistogramGradientFunction( GradientColor::TGradientFunction whichGradientFunction );
    void setHistogramDrawmodeSemantic( DrawModeMethod whichDrawmodeSemantic );
    void setHistogramDrawmodeObjects( DrawModeMethod whichDrawmodeObjects );
    void setHistogramDrawmodeBoth( DrawModeMethod whichDrawmodeBoth );
    void setHistogramSaveTextAsMatrix( bool whichSaveTextAsMatrix );
    void setHistogramSaveTextFormat( TTextFormat whichSaveTextFormat );
    void setHistogramSaveImageFormat( TImageFormat whichSaveImageFormat );

    bool getHistogramViewZoom() const;
    bool getHistogramViewGradientZoom() const;
    bool getHistogramViewHorizontal() const;
    bool getHistogramViewEmptyColumns() const;
    bool getHistogramScientificNotation() const;
    bool getHistogramThousandSep() const;
    UINT32 getHistogramPrecision() const;
    bool getHistogramShowUnits() const;
    TObjectOrder getHistogramNumColumns() const;
    bool getHistogramAutofitControlScale() const;
    bool getHistogramAutofitDataGradient() const;
    bool getHistogramAutofitThirdDimensionScale() const;
    GradientColor::TGradientFunction getHistogramGradientFunction() const;
    DrawModeMethod getHistogramDrawmodeSemantic() const;
    DrawModeMethod getHistogramDrawmodeObjects() const;
    DrawModeMethod getHistogramDrawmodeBoth() const;
    bool getHistogramSaveTextAsMatrix() const;
    TTextFormat getHistogramSaveTextFormat() const;
    TImageFormat getHistogramSaveImageFormat() const;

    // FILTERS XML SECTION
    void setFiltersFilterTraceUpToMB( float whichFilterTraceUpToMB );
    void setFiltersXMLPath( string whichXMLPath );

    float getFiltersFilterTraceUpToMB() const;
    string getFiltersXMLPath() const;

    // COLORS XML SECTION
    void setColorsTimelineBackground( rgb whichTimelineBackground );
    void setColorsTimelineAxis( rgb whichTimelineAxis );
    void setColorsTimelineLogicalCommunications( rgb whichTimelineLogicalCommunications );
    void setColorsTimelinePhysicalCommunications( rgb whichTimelinePhysicalCommunications );
    void setColorsTopGradient( rgb whichTopGradient );
    void setColorsLowGradient( rgb whichLowGradient );
    void setColorsBeginGradient( rgb whichBeginGradient );
    void setColorsEndGradient( rgb whichEndGradient );

    rgb getColorsTimelineBackground() const;
    rgb getColorsTimelineAxis() const;
    rgb getColorsTimelineLogicalCommunications() const;
    rgb getColorsTimelinePhysicalCommunications() const;
    rgb getColorsTopGradient() const;
    rgb getColorsLowGradient() const;
    rgb getColorsBeginGradient() const;
    rgb getColorsEndGradient() const;

    void saveXML( const string &filename );
    void loadXML( const string &filename );

  private:
    friend class boost::serialization::access;

    static ParaverConfig *instance;
    map<string, PropertyFunction *> propertyFunctions;

    void loadMap();
    void unLoadMap();

    struct XMLPreferencesGlobal
    {
      template< class Archive >
      void serialize( Archive & ar, const unsigned int version )
      {
        ar & boost::serialization::make_nvp( "traces_path", tracesPath );
        ar & boost::serialization::make_nvp( "cfgs_path", cfgsPath );
        ar & boost::serialization::make_nvp( "tmp_path", tmpPath );
        ar & boost::serialization::make_nvp( "apply_following_cfgs_to_all_traces", applyFollowingCFGsToAllTraces );
        ar & boost::serialization::make_nvp( "fill_state_gaps", fillStateGaps );
      }

      string tracesPath; // also for paraload.sig!
      string cfgsPath;
      string tmpPath;    // errors, logs, working dir
      bool applyFollowingCFGsToAllTraces;
      bool fillStateGaps;

    } xmlGlobal;


    struct XMLPreferencesTimeline
    {
      template< class Archive >
      void serialize( Archive & ar, const unsigned int version )
      {
        ar & boost::serialization::make_nvp( "default_name", defaultName );
        ar & boost::serialization::make_nvp( "name_format", nameFormat );
        ar & boost::serialization::make_nvp( "default_applied_cfg", defaultCFG );
        ar & boost::serialization::make_nvp( "decimal_precision", precision );
        ar & boost::serialization::make_nvp( "view_events_lines", viewEventsLines );
        ar & boost::serialization::make_nvp( "view_communications_lines", viewCommunicationsLines );
        ar & boost::serialization::make_nvp( "view_function_as_color", viewFunctionAsColor );
        ar & boost::serialization::make_nvp( "color", color );
        ar & boost::serialization::make_nvp( "drawmode_time", drawmodeTime );
        ar & boost::serialization::make_nvp( "drawmode_objects", drawmodeObjects );
        ar & boost::serialization::make_nvp( "drawmode_both", drawmodeBoth );
        ar & boost::serialization::make_nvp( "gradientFunction", gradientFunction );
        ar & boost::serialization::make_nvp( "pixel_size", pixelSize );
        ar & boost::serialization::make_nvp( "what_where_semantic", whatWhereSemantic );
        ar & boost::serialization::make_nvp( "what_where_events", whatWhereEvents );
        ar & boost::serialization::make_nvp( "what_where_communications", whatWhereCommunications );
        ar & boost::serialization::make_nvp( "what_where_previous_next", whatWherePreviousNext );
        ar & boost::serialization::make_nvp( "what_where_text", whatWhereText );
        ar & boost::serialization::make_nvp( "save_text_format", saveTextFormat );
        ar & boost::serialization::make_nvp( "save_image_format", saveImageFormat );
      }

      string defaultName;
      string nameFormat;
      string defaultCFG;
      UINT32 precision;
      bool viewEventsLines;
      bool viewCommunicationsLines;
      bool viewFunctionAsColor;
      SemanticColor::TColorFunction color;
      DrawModeMethod drawmodeTime;
      DrawModeMethod drawmodeObjects;
      DrawModeMethod drawmodeBoth;
      GradientColor::TGradientFunction gradientFunction;
      UINT32 pixelSize;
      bool whatWhereSemantic;
      bool whatWhereEvents;
      bool whatWhereCommunications;
      bool whatWherePreviousNext;
      bool whatWhereText;
      TTextFormat saveTextFormat;
      TImageFormat saveImageFormat;

    } xmlTimeline;

    struct XMLPreferencesHistogram
    {
      template< class Archive >
      void serialize( Archive & ar, const unsigned int version )
      {
        ar & boost::serialization::make_nvp( "view_zoom", viewZoom );
        ar & boost::serialization::make_nvp( "view_gradient_zoom", viewGradientZoom );
        ar & boost::serialization::make_nvp( "view_horizontal", viewHorizontal );
        ar & boost::serialization::make_nvp( "view_empty_columns", viewEmptyColumns );
        ar & boost::serialization::make_nvp( "cell_scientific_notation", scientificNotation );
        ar & boost::serialization::make_nvp( "cell_thousands_separator", thousandSep );
        ar & boost::serialization::make_nvp( "cell_decimal_precision", precision );
        ar & boost::serialization::make_nvp( "cell_show_units", showUnits );
        ar & boost::serialization::make_nvp( "number_of_columns", histoNumColumns );
        ar & boost::serialization::make_nvp( "autofit_control_scale", autofitControlScale );
        ar & boost::serialization::make_nvp( "autofit_data_gradient", autofitDataGradient );
        ar & boost::serialization::make_nvp( "autofit_third_dimension_scale", autofitThirdDimensionScale );
        ar & boost::serialization::make_nvp( "gradientFunction", gradientFunction );
        ar & boost::serialization::make_nvp( "drawmode_semantic", drawmodeSemantic );
        ar & boost::serialization::make_nvp( "drawmode_objects", drawmodeObjects );
        ar & boost::serialization::make_nvp( "drawmode_both", drawmodeBoth );
        ar & boost::serialization::make_nvp( "save_text_as_matrix", saveTextAsMatrix );
        ar & boost::serialization::make_nvp( "save_text_format", saveTextFormat );
        ar & boost::serialization::make_nvp( "save_image_format", saveImageFormat );
      }

      bool viewZoom;
      bool viewGradientZoom;
      bool viewHorizontal;
      bool viewEmptyColumns;
      bool scientificNotation;
      bool thousandSep;
      UINT32 precision;
      bool showUnits;
      TObjectOrder histoNumColumns;
      bool autofitControlScale;
      bool autofitDataGradient;
      bool autofitThirdDimensionScale;
      GradientColor::TGradientFunction gradientFunction;
      DrawModeMethod drawmodeSemantic;
      DrawModeMethod drawmodeObjects;
      DrawModeMethod drawmodeBoth;
      bool saveTextAsMatrix;
      TTextFormat saveTextFormat;
      TImageFormat saveImageFormat;

    } xmlHistogram;


    struct XMLPreferencesFilters
    {
      template< class Archive >
      void serialize( Archive & ar, const unsigned int version )
      {
        ar & boost::serialization::make_nvp( "filter_trace_up_to_MB", filterTraceUpToMB );
        ar & boost::serialization::make_nvp( "xml_path", xmlPath );
      }

      float filterTraceUpToMB;
      string xmlPath;

    } xmlFilters;

    struct XMLPreferencesColor
    {
      template< class Archive >
      void serialize( Archive & ar, const unsigned int version )
      {
        ar & boost::serialization::make_nvp( "timeline_background", timelineBackground );
        ar & boost::serialization::make_nvp( "timeline_axis", timelineAxis );
        ar & boost::serialization::make_nvp( "timeline_logical_communications", timelineLogicalCommunications );
        ar & boost::serialization::make_nvp( "timeline_physical_communications", timelinePhysicalCommunications );
        ar & boost::serialization::make_nvp( "top_gradient", topGradient );
        ar & boost::serialization::make_nvp( "low_gradient", lowGradient );
        ar & boost::serialization::make_nvp( "begin_gradient", beginGradient );
        ar & boost::serialization::make_nvp( "end_gradient", endGradient );
      }

      rgb timelineBackground;
      rgb timelineAxis;
      rgb timelineLogicalCommunications;
      rgb timelinePhysicalCommunications;
      rgb topGradient;
      rgb lowGradient;
      rgb beginGradient;
      rgb endGradient;

    } xmlColor;


    template< class Archive >
    void serialize( Archive & ar, const unsigned int version )
    {
      if ( version == 0)
      {
        UINT32 prec;
        TObjectOrder columns;
        bool units;
        bool thousSep;
        bool fillGaps;
        ar & boost::serialization::make_nvp( "precision", prec );
        ar & boost::serialization::make_nvp( "histoNumColumns", columns );
        ar & boost::serialization::make_nvp( "showUnits", units );
        ar & boost::serialization::make_nvp( "thousandSep", thousSep );
        ar & boost::serialization::make_nvp( "fillStateGaps", fillGaps );
        return;
      }

      ar & boost::serialization::make_nvp( "global", xmlGlobal );
      ar & boost::serialization::make_nvp( "timeline", xmlTimeline );
      ar & boost::serialization::make_nvp( "histogram", xmlHistogram );
      ar & boost::serialization::make_nvp( "filters", xmlFilters );
      ar & boost::serialization::make_nvp( "color", xmlColor );
    }
};

// First version: non structured, five xml tags
// BOOST_CLASS_VERSION( ParaverConfig, 0)

// Second version: introducing some structure
BOOST_CLASS_VERSION( ParaverConfig, 1)
BOOST_CLASS_VERSION( ParaverConfig::XMLPreferencesGlobal, 0)
BOOST_CLASS_VERSION( ParaverConfig::XMLPreferencesTimeline, 0)
BOOST_CLASS_VERSION( ParaverConfig::XMLPreferencesHistogram, 0)
BOOST_CLASS_VERSION( ParaverConfig::XMLPreferencesFilters, 0)
BOOST_CLASS_VERSION( ParaverConfig::XMLPreferencesColor, 0)

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
