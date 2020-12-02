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


#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#ifdef WIN32
  #include <shlobj.h>
  #include <Shlwapi.h>
  #define MAX_LEN_PATH 2048
#else
  #include <sys/stat.h>
  #include <pwd.h>
  #include <sys/types.h>
#endif
#ifdef __APPLE__
  #include "CoreFoundation/CoreFoundation.h"
#endif

#include "paraverconfig.h"
#include "paraverkernelexception.h"


using namespace std;

ParaverConfig *ParaverConfig::instance = NULL;

ParaverConfig *ParaverConfig::getInstance()
{
  if ( ParaverConfig::instance == NULL )
    ParaverConfig::instance = new ParaverConfig();
  return ParaverConfig::instance;
}

ParaverConfig::ParaverConfig() : isModified( false )
{
  string homedir;
  string paraverHomeDir;
  string paraverCFGsDir;
  string paraverXMLDir;
  string paraverTutorialsDir;

#ifdef WIN32

  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );

  WCHAR myPath[ MAX_LEN_PATH ];
  HMODULE hModule = GetModuleHandle( NULL );
  if ( hModule != NULL )
  {
    GetModuleFileName( NULL, myPath, ( sizeof( myPath ) ));
    PathRemoveFileSpec( myPath );
    char tmpMyPath[ MAX_LEN_PATH ];
    size_t tmpSize;
    wcstombs_s( &tmpSize, tmpMyPath, MAX_LEN_PATH, myPath, MAX_LEN_PATH );
    paraverHomeDir = tmpMyPath;

    paraverCFGsDir = paraverHomeDir;
    paraverCFGsDir.append( "\\cfgs" );

    paraverXMLDir = paraverHomeDir;
    paraverXMLDir.append( "\\share\\filters-config" );

    paraverTutorialsDir = paraverHomeDir;
    paraverTutorialsDir.append( "\\tutorials" );
  }
  else
  {
    paraverHomeDir      = homedir;
    paraverCFGsDir      = homedir;
    paraverXMLDir       = homedir;
    paraverTutorialsDir = homedir;
  }

#else
  homedir = getenv( "HOME" );
  if( homedir.empty() )
  {
    struct passwd *pwd = getpwuid( getuid() );
    if( pwd != NULL )
    {
      homedir = string( pwd->pw_dir );
    }
    else
    {
      homedir = string( "/tmp" );
    }
  }

#ifdef __APPLE__
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
  char tmpPath[PATH_MAX];
  if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)tmpPath, PATH_MAX))
  {
      throw ParaverKernelException();
  }
  CFRelease(resourcesURL);

  paraverHomeDir      = homedir;
  paraverCFGsDir      = std::string( tmpPath ) + std::string( "/cfgs" );
  paraverXMLDir       = homedir;
  paraverTutorialsDir = homedir + std::string( "/paraver-tutorials" );
#else // not __APPLE__
  if ( getenv( "PARAVER_HOME" ) == NULL )
  {
    paraverHomeDir       = homedir;
    paraverCFGsDir       = homedir;
    paraverXMLDir        = homedir;
  }
  else
  {
    paraverHomeDir      = getenv( "PARAVER_HOME" );
    paraverCFGsDir      = paraverHomeDir + "/cfgs";
    paraverXMLDir       = paraverHomeDir + "/share/filters-config";
  }
  paraverTutorialsDir = homedir + "/paraver-tutorials";

#endif
#endif


  xmlGlobal.tracesPath = homedir; // also for paraload.sig!
  xmlGlobal.cfgsPath = paraverCFGsDir;
  xmlGlobal.tutorialsPath = paraverTutorialsDir;
  xmlGlobal.tmpPath = homedir; // errors, logs, working dir
  xmlGlobal.applyFollowingCFGsToAllTraces = false;
  xmlGlobal.fillStateGaps = true;
  xmlGlobal.fullTracePath = false;
  xmlGlobal.singleInstance = true;
  xmlGlobal.mainWindowWidth = 300;
  xmlGlobal.mainWindowHeight = 600;
  string strFile( homedir );
#ifdef WIN32
  strFile.append( "\\paraver\\" );
#else
  strFile.append( "/.paraver/" );
#endif

  paraverConfigDir = strFile;

  xmlGlobal.sessionPath = strFile;
  xmlGlobal.sessionSaveTime = 1;
  xmlGlobal.prevSessionLoad = true;
  xmlGlobal.helpContentsUsesBrowser = false;
  xmlGlobal.helpContentsQuestionAnswered = false;
  xmlGlobal.disableTimelineZoomMouseWheel = false;
  xmlGlobal.appsChecked = false;

  xmlTimeline.defaultName = "New window # %N";
  xmlTimeline.nameFormat = "%W @ %T";
  xmlTimeline.defaultCFG = "";
  xmlTimeline.precision = 2;
  xmlTimeline.viewEventsLines = false;
  xmlTimeline.viewCommunicationsLines = true;
  //DEPRECATED
  xmlTimeline.viewFunctionAsColor = true;
  xmlTimeline.color = SemanticColor::COLOR;
  xmlTimeline.drawmodeTime = DRAW_MAXIMUM;
  xmlTimeline.drawmodeObjects = DRAW_MAXIMUM;
  xmlTimeline.gradientFunction = GradientColor::LINEAR;
  xmlTimeline.semanticScaleMinAtZero = false;
  xmlTimeline.pixelSize = 0;
  xmlTimeline.objectLabels = Window::SPACED_LABELS;
  xmlTimeline.objectAxisSize = Window::CURRENT_LEVEL;
  xmlTimeline.whatWhereSemantic = true;
  xmlTimeline.whatWhereEvents = false;
  xmlTimeline.whatWhereCommunications = false;
  xmlTimeline.whatWherePreviousNext = false;
  xmlTimeline.whatWhereText = true;
  xmlTimeline.whatWhereEventPixels = 5;
  xmlTimeline.saveTextFormat = CSV;
  xmlTimeline.saveImageFormat = PNG;

  xmlHistogram.viewZoom = true;
  xmlHistogram.viewFirstRowColored = false;
  xmlHistogram.viewGradientColors = true;
  xmlHistogram.viewHorizontal = true;
  xmlHistogram.viewEmptyColumns = true;
  xmlHistogram.scientificNotation = false;
  xmlHistogram.thousandSep = true;
  xmlHistogram.precision = 2;
  xmlHistogram.showUnits = true;
  xmlHistogram.histoNumColumns = 200;
  xmlHistogram.autofitControlScale = true;
  xmlHistogram.autofitControlScaleZero = false;
  xmlHistogram.autofitDataGradient = true;
  xmlHistogram.autofitThirdDimensionScale = true;
  xmlHistogram.gradientFunction = GradientColor::LINEAR;
  xmlHistogram.drawmodeSemantic = DRAW_MAXIMUM;
  xmlHistogram.drawmodeObjects = DRAW_MAXIMUM;
  xmlHistogram.saveTextAsMatrix = true;
  xmlHistogram.saveTextFormat = CSV;
  xmlHistogram.saveImageFormat = PNG;
  xmlHistogram.pixelSize = 0;
  xmlHistogram.skipCreateDialog = false;
  xmlHistogram.onlyTotals = false;
  xmlHistogram.shortLabels = true;

  // Filter Globals
  xmlFilters.filterTraceUpToMB = 500.0;
  xmlFilters.xmlPath = paraverXMLDir;

  xmlFilters.xmlCutterInstance.byTime = false;
  xmlFilters.xmlCutterInstance.minimumTime = 0;
  xmlFilters.xmlCutterInstance.maximumTime = 0; // proposal: 0 == maximum trace time
  xmlFilters.xmlCutterInstance.minimumTimePercentage = 0;
  xmlFilters.xmlCutterInstance.maximumTimePercentage = 100;
  xmlFilters.xmlCutterInstance.originalTime = true;
  xmlFilters.xmlCutterInstance.breakStates = true;
  xmlFilters.xmlCutterInstance.removeFirstStates = false;
  xmlFilters.xmlCutterInstance.removeLastStates = false;
  xmlFilters.xmlCutterInstance.keepEvents = false;

  xmlFilters.xmlFilterInstance.discardStates = false;
  xmlFilters.xmlFilterInstance.discardEvents = false;
  xmlFilters.xmlFilterInstance.discardCommunications = false;
  xmlFilters.xmlFilterInstance.communicationsMinimumSize = 10000.0; // Random!

  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.intervalsOrStates = true;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.samplingInterval = 1000000.0; // 1ms
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.minimumBurstTime = 1000.0; // 1us
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.types = string("");
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.countEventsOrAcummulateValues = true;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.removeStates = false;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.summarizeStates = false;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.globalCounters = false;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.onlyInBursts = false;
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.typesKept = string("");

  xmlColor.timelineBackground = SemanticColor::BACKGROUND;
  xmlColor.timelineAxis = SemanticColor::FOREGROUND;
  xmlColor.timelineZeroDashLine = SemanticColor::ZERO_AXIS;
  xmlColor.useColorZero = true;
  xmlColor.timelineColorZero = SemanticColor::BACKGROUND;
  xmlColor.timelineColorPunctual = SemanticColor::DEFAULT_PUNCTUAL;
  xmlColor.timelineLogicalCommunications = SemanticColor::DEFAULT_LOGICAL_COMMUNICATIONS;
  xmlColor.timelinePhysicalCommunications = SemanticColor::DEFAULT_PHYSICAL_COMMUNICATIONS;
  xmlColor.topGradient = SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR;
  xmlColor.lowGradient = SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR;
  xmlColor.beginGradient = SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR;
  xmlColor.endGradient = SemanticColor::DEFAULT_END_GRADIENT_COLOR;
  xmlColor.beginNegativeGradient = SemanticColor::DEFAULT_NEGATIVE_BEGIN_GRADIENT_COLOR;
  xmlColor.endNegativeGradient = SemanticColor::DEFAULT_NEGATIVE_END_GRADIENT_COLOR;


#ifdef WIN32
  xmlExternalApplications.myTextEditors.push_back( "start" );
#elif defined(__APPLE__)
  xmlExternalApplications.myTextEditors.push_back( "open" );
#else
  xmlExternalApplications.myTextEditors.push_back( "xdg-open" );
#endif
  xmlExternalApplications.myTextEditors.push_back( "gvim" );
  xmlExternalApplications.myTextEditors.push_back( "nedit" );
  xmlExternalApplications.myTextEditors.push_back( "gedit" );
  xmlExternalApplications.myTextEditors.push_back( "xed" );
  xmlExternalApplications.myTextEditors.push_back( "kate" );
  xmlExternalApplications.myTextEditors.push_back( "textedit" );
  xmlExternalApplications.myTextEditors.push_back( "Notepad++.exe" );
  xmlExternalApplications.myTextEditors.push_back( "wordpad.exe" );
  xmlExternalApplications.myTextEditors.push_back( "notepad.exe" );
  xmlExternalApplications.myTextEditors.push_back( "textedit" );

#ifdef WIN32
  xmlExternalApplications.myPDFReaders.push_back( "start" );
#elif defined(__APPLE__)
  xmlExternalApplications.myPDFReaders.push_back( "open" );
#else
  xmlExternalApplications.myPDFReaders.push_back( "xdg-open" );
#endif
  xmlExternalApplications.myPDFReaders.push_back( "evince" );
  xmlExternalApplications.myPDFReaders.push_back( "okular" );
  xmlExternalApplications.myPDFReaders.push_back( "xreader" );
  xmlExternalApplications.myPDFReaders.push_back( "firefox" );
  xmlExternalApplications.myPDFReaders.push_back( "brave-browser-stable" );
  xmlExternalApplications.myPDFReaders.push_back( "mupdf" ); 
  xmlExternalApplications.myPDFReaders.push_back( "atril" ); 
  xmlExternalApplications.myPDFReaders.push_back( "Acrobat.exe" ); 
  xmlExternalApplications.myPDFReaders.push_back( "MicrosoftEdge.exe" );
  xmlExternalApplications.myTextEditors.push_back( "safari" );

  loadMap();
}

ParaverConfig::~ParaverConfig()
{
  unloadMap();
}

std::string ParaverConfig::getParaverConfigDir()
{
  return paraverConfigDir;
}


// GLOBAL XML SECTION
void ParaverConfig::setGlobalTracesPath( string whichTracesPath )
{
  isModified = isModified || ( xmlGlobal.tracesPath != whichTracesPath );
  xmlGlobal.tracesPath = whichTracesPath;
}

void ParaverConfig::setGlobalCFGsPath( string whichCfgsPath )
{
  isModified = isModified || ( xmlGlobal.cfgsPath != whichCfgsPath );
  xmlGlobal.cfgsPath = whichCfgsPath;
}

void ParaverConfig::setGlobalTutorialsPath( string whichTutorialsPath )
{
  isModified = isModified || ( xmlGlobal.tutorialsPath != whichTutorialsPath );
  xmlGlobal.tutorialsPath = whichTutorialsPath;
}

void ParaverConfig::setGlobalTmpPath( string whichTmpPath )
{
  isModified = isModified || ( xmlGlobal.tmpPath != whichTmpPath );
  xmlGlobal.tmpPath = whichTmpPath;
}

void ParaverConfig::setGlobalApplyFollowingCFGsToAllTraces( bool whichApplyFollowingCFGsToAllTraces )
{
  isModified = isModified || ( xmlGlobal.applyFollowingCFGsToAllTraces != whichApplyFollowingCFGsToAllTraces );
  xmlGlobal.applyFollowingCFGsToAllTraces = whichApplyFollowingCFGsToAllTraces;
}

void ParaverConfig::setGlobalFillStateGaps( bool fill )
{
  isModified = isModified || ( xmlGlobal.fillStateGaps != fill );
  xmlGlobal.fillStateGaps = fill;
}

void ParaverConfig::setGlobalFullTracePath( bool fullPath )
{
  isModified = isModified || ( xmlGlobal.fullTracePath != fullPath );
  xmlGlobal.fullTracePath = fullPath;
}

void ParaverConfig::setGlobalSingleInstance( bool whichSingleInstance )
{
  isModified = isModified || ( xmlGlobal.singleInstance != whichSingleInstance );
  xmlGlobal.singleInstance = whichSingleInstance;
}

void ParaverConfig::setMainWindowWidth( unsigned int whichWidth )
{
  isModified = isModified || ( xmlGlobal.mainWindowWidth != whichWidth );
  xmlGlobal.mainWindowWidth = whichWidth;
}

void ParaverConfig::setMainWindowHeight( unsigned int whichHeight )
{
  isModified = isModified || ( xmlGlobal.mainWindowHeight != whichHeight );
  xmlGlobal.mainWindowHeight = whichHeight;
}

void ParaverConfig::setGlobalSessionPath( string whichSessionPath )
{
  isModified = isModified || ( xmlGlobal.sessionPath != whichSessionPath );
  xmlGlobal.sessionPath = whichSessionPath;
}

void ParaverConfig::setGlobalSessionSaveTime( PRV_UINT16 whichSessionSaveTime )
{
  isModified = isModified || ( xmlGlobal.sessionSaveTime != whichSessionSaveTime );
  xmlGlobal.sessionSaveTime = whichSessionSaveTime;
}

void ParaverConfig::setGlobalPrevSessionLoad( bool isPrevSessionLoaded )
{
  isModified = isModified || ( xmlGlobal.prevSessionLoad != isPrevSessionLoaded );
  xmlGlobal.prevSessionLoad = isPrevSessionLoaded;
}

void ParaverConfig::setGlobalHelpContentsUsesBrowser( bool isHelpContentsUsesBrowser )
{
  isModified = isModified || ( xmlGlobal.helpContentsUsesBrowser != isHelpContentsUsesBrowser );
  xmlGlobal.helpContentsUsesBrowser = isHelpContentsUsesBrowser;
}

void ParaverConfig::setGlobalHelpContentsQuestionAnswered( bool isHelpContentsQuestionAnswered )
{
  isModified = isModified || ( xmlGlobal.helpContentsQuestionAnswered != isHelpContentsQuestionAnswered );
  xmlGlobal.helpContentsQuestionAnswered = isHelpContentsQuestionAnswered;
}

void ParaverConfig::setDisableTimelineZoomMouseWheel( bool disable )
{
  isModified = isModified || ( xmlGlobal.disableTimelineZoomMouseWheel != disable );
  xmlGlobal.disableTimelineZoomMouseWheel = disable;
}


void ParaverConfig::setAppsChecked() // will always set to True
{
  xmlGlobal.appsChecked = true;
}

string ParaverConfig::getGlobalTracesPath() const
{
  return xmlGlobal.tracesPath;
}

string ParaverConfig::getGlobalCFGsPath() const
{
  return xmlGlobal.cfgsPath ;
}

string ParaverConfig::getGlobalTutorialsPath() const
{
  return xmlGlobal.tutorialsPath ;
}

string ParaverConfig::getGlobalTmpPath() const
{
  return xmlGlobal.tmpPath;
}

bool ParaverConfig::getGlobalApplyFollowingCFGsToAllTraces() const
{
  return xmlGlobal.applyFollowingCFGsToAllTraces;
}

bool ParaverConfig::getGlobalFillStateGaps() const
{
  return xmlGlobal.fillStateGaps;
}

bool ParaverConfig::getGlobalFullTracePath() const
{
  return xmlGlobal.fullTracePath;
}

bool ParaverConfig::getGlobalSingleInstance() const
{
  return xmlGlobal.singleInstance;
}

unsigned int ParaverConfig::getMainWindowWidth() const
{
  return xmlGlobal.mainWindowWidth;
}

unsigned int ParaverConfig::getMainWindowHeight() const
{
  return xmlGlobal.mainWindowHeight;
}

string ParaverConfig::getGlobalSessionPath() const
{
  return xmlGlobal.sessionPath;
}


PRV_UINT16 ParaverConfig::getGlobalSessionSaveTime() const
{
  return xmlGlobal.sessionSaveTime;
}

bool ParaverConfig::getGlobalPrevSessionLoad() const
{
  return xmlGlobal.prevSessionLoad;
}

bool ParaverConfig::getGlobalHelpContentsUsesBrowser() const
{
  return xmlGlobal.helpContentsUsesBrowser;
}

bool ParaverConfig::getGlobalHelpContentsQuestionAnswered() const
{
  return xmlGlobal.helpContentsQuestionAnswered;
}

bool ParaverConfig::getDisableTimelineZoomMouseWheel() const
{
  return xmlGlobal.disableTimelineZoomMouseWheel;
}


// TIMELINES XML SECTION
void ParaverConfig::setTimelineDefaultName( string whichDefaultName )
{
  isModified = isModified || ( xmlTimeline.defaultName != whichDefaultName );
  xmlTimeline.defaultName = whichDefaultName;
}

void ParaverConfig::setTimelineNameFormat( string whichNameFormat )
{
  isModified = isModified || ( xmlTimeline.nameFormat != whichNameFormat );
  xmlTimeline.nameFormat = whichNameFormat;
}

void ParaverConfig::setTimelineDefaultCFG( string whichDefaultCFG )
{
  isModified = isModified || ( xmlTimeline.defaultCFG != whichDefaultCFG );
  xmlTimeline.defaultCFG = whichDefaultCFG;
}

void ParaverConfig::setTimelinePrecision( PRV_UINT32 whichPrecision )
{
  isModified = isModified || ( xmlTimeline.precision != whichPrecision );
  xmlTimeline.precision = whichPrecision;
}

void ParaverConfig::setTimelineViewEventsLines( bool whichViewEventsLines )
{
  isModified = isModified || ( xmlTimeline.viewEventsLines != whichViewEventsLines );
  xmlTimeline.viewEventsLines = whichViewEventsLines;
}

void ParaverConfig::setTimelineViewCommunicationsLines( bool whichViewCommunicationsLines )
{
  isModified = isModified || ( xmlTimeline.viewCommunicationsLines != whichViewCommunicationsLines );
  xmlTimeline.viewCommunicationsLines = whichViewCommunicationsLines;
}

//DEPRECATED
void ParaverConfig::setTimelineViewFunctionAsColor( bool whichViewFunctionAsColor )
{
  isModified = isModified || ( xmlTimeline.viewFunctionAsColor != whichViewFunctionAsColor );
  xmlTimeline.viewFunctionAsColor = whichViewFunctionAsColor;
}

void ParaverConfig::setTimelineColor( SemanticColor::TColorFunction whichColor )
{
  isModified = isModified || ( xmlTimeline.color != whichColor );
  xmlTimeline.color = whichColor;
}

void ParaverConfig::setTimelineDrawmodeTime( DrawModeMethod whichDrawmodeTime )
{
  isModified = isModified || ( xmlTimeline.drawmodeTime != whichDrawmodeTime );
  xmlTimeline.drawmodeTime = whichDrawmodeTime;
}

void ParaverConfig::setTimelineDrawmodeObjects( DrawModeMethod whichDrawmodeObjects )
{
  isModified = isModified || ( xmlTimeline.drawmodeObjects != whichDrawmodeObjects );
  xmlTimeline.drawmodeObjects = whichDrawmodeObjects;
}

void ParaverConfig::setTimelineGradientFunction( GradientColor::TGradientFunction whichGradientFunction )
{
  isModified = isModified || ( xmlTimeline.gradientFunction != whichGradientFunction );
  xmlTimeline.gradientFunction = whichGradientFunction;
}

void ParaverConfig::setTimelineSemanticScaleMinAtZero( bool whichSemanticScaleMinAtZero )
{
  isModified = isModified || ( xmlTimeline.semanticScaleMinAtZero != whichSemanticScaleMinAtZero );
  xmlTimeline.semanticScaleMinAtZero = whichSemanticScaleMinAtZero;
}

void ParaverConfig::setTimelinePixelSize( PRV_UINT32 whichPixelSize )
{
  isModified = isModified || ( xmlTimeline.pixelSize != whichPixelSize );
  xmlTimeline.pixelSize = whichPixelSize;
}

void ParaverConfig::setTimelineLabels( Window::TObjectLabels whichLabels )
{
  isModified = isModified || ( xmlTimeline.objectLabels != whichLabels );
  xmlTimeline.objectLabels = whichLabels;
}

void ParaverConfig::setTimelineObjectAxisSize( Window::TObjectAxisSize whichSize )
{
  isModified = isModified || ( xmlTimeline.objectAxisSize != whichSize );
  xmlTimeline.objectAxisSize = whichSize;
}

void ParaverConfig::setTimelineWhatWhereSemantic( bool whichWhatWhereSemantic )
{
  isModified = isModified || ( xmlTimeline.whatWhereSemantic != whichWhatWhereSemantic );
  xmlTimeline.whatWhereSemantic = whichWhatWhereSemantic;
}

void ParaverConfig::setTimelineWhatWhereEvents( bool whichWhatWhereEvents )
{
  isModified = isModified || ( xmlTimeline.whatWhereEvents != whichWhatWhereEvents );
  xmlTimeline.whatWhereEvents = whichWhatWhereEvents;
}

void ParaverConfig::setTimelineWhatWhereCommunications( bool whichWhatWhereCommunications )
{
  isModified = isModified || ( xmlTimeline.whatWhereCommunications != whichWhatWhereCommunications );
  xmlTimeline.whatWhereCommunications = whichWhatWhereCommunications;
}

void ParaverConfig::setTimelineWhatWherePreviousNext( bool whichWhatWherePreviousNext )
{
  isModified = isModified || ( xmlTimeline.whatWherePreviousNext != whichWhatWherePreviousNext );
  xmlTimeline.whatWherePreviousNext = whichWhatWherePreviousNext;
}

void ParaverConfig::setTimelineWhatWhereText( bool whichWhatWhereText )
{
  isModified = isModified || ( xmlTimeline.whatWhereText != whichWhatWhereText );
  xmlTimeline.whatWhereText = whichWhatWhereText;
}

void ParaverConfig::setTimelineWhatWhereEventPixels( PRV_INT16 eventPixels )
{
  isModified = isModified || ( xmlTimeline.whatWhereEventPixels != eventPixels );
  xmlTimeline.whatWhereEventPixels = eventPixels;
}

void ParaverConfig::setTimelineSaveTextFormat( TTextFormat whichSaveTextFormat )
{
  isModified = isModified || ( xmlTimeline.saveTextFormat != whichSaveTextFormat );
  xmlTimeline.saveTextFormat = whichSaveTextFormat;
}

void ParaverConfig::setTimelineSaveImageFormat( TImageFormat whichSaveImageFormat )
{
  isModified = isModified || ( xmlTimeline.saveImageFormat != whichSaveImageFormat );
  xmlTimeline.saveImageFormat = whichSaveImageFormat;
}


string ParaverConfig::getTimelineDefaultName() const
{
  return xmlTimeline.defaultName;
}

string ParaverConfig::getTimelineNameFormat() const
{
  return xmlTimeline.nameFormat;
}

string ParaverConfig::getTimelineDefaultCFG() const
{
  return xmlTimeline.defaultCFG;
}

PRV_UINT32 ParaverConfig::getTimelinePrecision() const
{
  return xmlTimeline.precision;
}

bool ParaverConfig::getTimelineViewEventsLines() const
{
  return xmlTimeline.viewEventsLines;
}

bool ParaverConfig::getTimelineViewCommunicationsLines() const
{
  return xmlTimeline.viewCommunicationsLines;
}

//DEPRECATED
bool ParaverConfig::getTimelineViewFunctionAsColor() const
{
  return xmlTimeline.viewFunctionAsColor;
}

SemanticColor::TColorFunction ParaverConfig::getTimelineColor() const
{
  return xmlTimeline.color;
}

DrawModeMethod ParaverConfig::getTimelineDrawmodeTime() const
{
  return xmlTimeline.drawmodeTime;
}

DrawModeMethod ParaverConfig::getTimelineDrawmodeObjects() const
{
  return xmlTimeline.drawmodeObjects;
}

GradientColor::TGradientFunction ParaverConfig::getTimelineGradientFunction() const
{
  return xmlTimeline.gradientFunction;
}

bool ParaverConfig::getTimelineSemanticScaleMinAtZero() const
{
  return xmlTimeline.semanticScaleMinAtZero;
}

PRV_UINT32 ParaverConfig::getTimelinePixelSize() const
{
  return xmlTimeline.pixelSize;
}

Window::TObjectLabels ParaverConfig::getTimelineLabels() const
{
  return xmlTimeline.objectLabels;
}

Window::TObjectAxisSize ParaverConfig::getTimelineObjectAxisSize() const
{
  return xmlTimeline.objectAxisSize;
}

bool ParaverConfig::getTimelineWhatWhereSemantic() const
{
  return xmlTimeline.whatWhereSemantic;
}

bool ParaverConfig::getTimelineWhatWhereEvents() const
{
  return xmlTimeline.whatWhereEvents;
}

bool ParaverConfig::getTimelineWhatWhereCommunications() const
{
  return xmlTimeline.whatWhereCommunications;
}

bool ParaverConfig::getTimelineWhatWherePreviousNext() const
{
  return xmlTimeline.whatWherePreviousNext;
}

bool ParaverConfig::getTimelineWhatWhereText() const
{
  return xmlTimeline.whatWhereText;
}

PRV_INT16 ParaverConfig::getTimelineWhatWhereEventPixels() const
{
  return xmlTimeline.whatWhereEventPixels;
}

ParaverConfig::TTextFormat ParaverConfig::getTimelineSaveTextFormat() const
{
  return xmlTimeline.saveTextFormat;
}

ParaverConfig::TImageFormat ParaverConfig::getTimelineSaveImageFormat() const
{
  return xmlTimeline.saveImageFormat;
}


// HISTOGRAM
void ParaverConfig::setHistogramViewZoom( bool whichViewZoom )
{
  isModified = isModified || ( xmlHistogram.viewZoom != whichViewZoom );
  xmlHistogram.viewZoom = whichViewZoom;
}

void ParaverConfig::setHistogramViewFirstRowColored( bool whichViewFirstRow )
{
  isModified = isModified || ( xmlHistogram.viewFirstRowColored != whichViewFirstRow );
  xmlHistogram.viewFirstRowColored = whichViewFirstRow;
}

void ParaverConfig::setHistogramViewGradientColors( bool whichViewGradientColors )
{
  isModified = isModified || ( xmlHistogram.viewGradientColors != whichViewGradientColors );
  xmlHistogram.viewGradientColors = whichViewGradientColors;
}

void ParaverConfig::setHistogramViewHorizontal( bool whichViewHorizontal )
{
  isModified = isModified || ( xmlHistogram.viewHorizontal != whichViewHorizontal );
  xmlHistogram.viewHorizontal = whichViewHorizontal;
}

void ParaverConfig::setHistogramViewEmptyColumns( bool whichViewEmptyColumns )
{
  isModified = isModified || ( xmlHistogram.viewEmptyColumns != whichViewEmptyColumns );
  xmlHistogram.viewEmptyColumns = whichViewEmptyColumns;
}

void ParaverConfig::setHistogramScientificNotation( bool whichScientificNotation )
{
  isModified = isModified || ( xmlHistogram.scientificNotation != whichScientificNotation );
  xmlHistogram.scientificNotation = whichScientificNotation;
}

void ParaverConfig::setHistogramThousandSep( bool whichThousandSep )
{
  isModified = isModified || ( xmlHistogram.thousandSep != whichThousandSep );
  xmlHistogram.thousandSep = whichThousandSep;
}

void ParaverConfig::setHistogramPrecision( PRV_UINT32 whichPrecision )
{
  isModified = isModified || ( xmlHistogram.precision != whichPrecision );
  xmlHistogram.precision = whichPrecision;
}

void ParaverConfig::setHistogramShowUnits( bool whichShowUnits )
{
  isModified = isModified || ( xmlHistogram.showUnits != whichShowUnits );
  xmlHistogram.showUnits = whichShowUnits;
}

void ParaverConfig::setHistogramNumColumns( TObjectOrder whichNumColumns )
{
  isModified = isModified || ( xmlHistogram.histoNumColumns != whichNumColumns );
  xmlHistogram.histoNumColumns = whichNumColumns;
}

void ParaverConfig::setHistogramAutofitControlScale( bool whichAutofitControlScale )
{
  isModified = isModified || ( xmlHistogram.autofitControlScale != whichAutofitControlScale );
  xmlHistogram.autofitControlScale = whichAutofitControlScale;
}

void ParaverConfig::setHistogramAutofitControlScaleZero( bool whichAutofitControlScaleZero )
{
  isModified = isModified || ( xmlHistogram.autofitControlScaleZero != whichAutofitControlScaleZero );
  xmlHistogram.autofitControlScaleZero = whichAutofitControlScaleZero;
}

void ParaverConfig::setHistogramAutofitDataGradient( bool whichAutofitDataGradient )
{
  isModified = isModified || ( xmlHistogram.autofitDataGradient != whichAutofitDataGradient );
  xmlHistogram.autofitDataGradient = whichAutofitDataGradient;
}

void ParaverConfig::setHistogramAutofitThirdDimensionScale( bool whichAutofitThirdDimensionScale )
{
  isModified = isModified || ( xmlHistogram.autofitThirdDimensionScale != whichAutofitThirdDimensionScale );
  xmlHistogram.autofitThirdDimensionScale = whichAutofitThirdDimensionScale;
}

void ParaverConfig::setHistogramGradientFunction( GradientColor::TGradientFunction whichGradientFunction )
{
  isModified = isModified || ( xmlHistogram.gradientFunction != whichGradientFunction );
  xmlHistogram.gradientFunction = whichGradientFunction;
}

void ParaverConfig::setHistogramDrawmodeSemantic( DrawModeMethod whichDrawmodeSemantic )
{
  isModified = isModified || ( xmlHistogram.drawmodeSemantic != whichDrawmodeSemantic );
  xmlHistogram.drawmodeSemantic = whichDrawmodeSemantic;
}

void ParaverConfig::setHistogramDrawmodeObjects( DrawModeMethod whichDrawmodeObjects )
{
  isModified = isModified || ( xmlHistogram.drawmodeObjects != whichDrawmodeObjects );
  xmlHistogram.drawmodeObjects = whichDrawmodeObjects;
}

void ParaverConfig::setHistogramSaveTextAsMatrix( bool whichSaveTextAsMatrix )
{
  isModified = isModified || ( xmlHistogram.saveTextAsMatrix != whichSaveTextAsMatrix );
  xmlHistogram.saveTextAsMatrix = whichSaveTextAsMatrix;
}

void ParaverConfig::setHistogramSaveTextFormat( TTextFormat whichSaveTextFormat )
{
  isModified = isModified || ( xmlHistogram.saveTextFormat != whichSaveTextFormat );
  xmlHistogram.saveTextFormat = whichSaveTextFormat;
}

void ParaverConfig::setHistogramSaveImageFormat( TImageFormat whichSaveImageFormat )
{
  isModified = isModified || ( xmlHistogram.saveImageFormat != whichSaveImageFormat );
  xmlHistogram.saveImageFormat = whichSaveImageFormat;
}

void ParaverConfig::setHistogramPixelSize( PRV_UINT16 whichPixelSize )
{
  isModified = isModified || ( xmlHistogram.pixelSize != whichPixelSize );
  xmlHistogram.pixelSize = whichPixelSize;
}

void ParaverConfig::setHistogramSkipCreateDialog( bool whichSkipCreateDialog )
{
  isModified = isModified || ( xmlHistogram.skipCreateDialog != whichSkipCreateDialog );
  xmlHistogram.skipCreateDialog = whichSkipCreateDialog;
}

void ParaverConfig::setHistogramOnlyTotals( bool whichOnlyTotals )
{
  isModified = isModified || ( xmlHistogram.onlyTotals != whichOnlyTotals );
  xmlHistogram.onlyTotals = whichOnlyTotals;
}

void ParaverConfig::setHistogramShortLabels( bool whichShortLabels )
{
  isModified = isModified || ( xmlHistogram.shortLabels != whichShortLabels );
  xmlHistogram.shortLabels = whichShortLabels;
}

bool ParaverConfig::getHistogramViewZoom() const
{
  return xmlHistogram.viewZoom;
}

bool ParaverConfig::getHistogramViewFirstRowColored() const
{
  return xmlHistogram.viewFirstRowColored;
}

bool ParaverConfig::getHistogramViewGradientColors() const
{
  return xmlHistogram.viewGradientColors;
}

bool ParaverConfig::getHistogramViewHorizontal() const
{
  return xmlHistogram.viewHorizontal;
}

bool ParaverConfig::getHistogramViewEmptyColumns() const
{
  return xmlHistogram.viewEmptyColumns;
}

bool ParaverConfig::getHistogramScientificNotation() const
{
  return xmlHistogram.scientificNotation;
}

bool ParaverConfig::getHistogramThousandSep() const
{
  return xmlHistogram.thousandSep;
}

PRV_UINT32 ParaverConfig::getHistogramPrecision() const
{
  return xmlHistogram.precision;
}

bool ParaverConfig::getHistogramShowUnits() const
{
  return xmlHistogram.showUnits;
}

TObjectOrder ParaverConfig::getHistogramNumColumns() const
{
  return xmlHistogram.histoNumColumns;
}

bool ParaverConfig::getHistogramAutofitControlScale() const
{
  return xmlHistogram.autofitControlScale;
}

bool ParaverConfig::getHistogramAutofitControlScaleZero() const
{
  return xmlHistogram.autofitControlScaleZero;
}

bool ParaverConfig::getHistogramAutofitDataGradient() const
{
  return xmlHistogram.autofitDataGradient;
}

bool ParaverConfig::getHistogramAutofitThirdDimensionScale() const
{
  return xmlHistogram.autofitThirdDimensionScale;
}

GradientColor::TGradientFunction ParaverConfig::getHistogramGradientFunction() const
{
  return xmlHistogram.gradientFunction;
}

DrawModeMethod ParaverConfig::getHistogramDrawmodeSemantic() const
{
  return xmlHistogram.drawmodeSemantic;
}

DrawModeMethod ParaverConfig::getHistogramDrawmodeObjects() const
{
  return xmlHistogram.drawmodeObjects;
}

bool ParaverConfig::getHistogramSaveTextAsMatrix() const
{
  return xmlHistogram.saveTextAsMatrix;
}

ParaverConfig::TTextFormat ParaverConfig::getHistogramSaveTextFormat() const
{
  return xmlHistogram.saveTextFormat;
}

ParaverConfig::TImageFormat ParaverConfig::getHistogramSaveImageFormat() const
{
  return xmlHistogram.saveImageFormat;
}

PRV_UINT16 ParaverConfig::getHistogramPixelSize() const
{
  return xmlHistogram.pixelSize;
}

bool ParaverConfig::getHistogramSkipCreateDialog() const
{
  return xmlHistogram.skipCreateDialog;
}

bool ParaverConfig::getHistogramOnlyTotals() const
{
  return xmlHistogram.onlyTotals;
}

bool ParaverConfig::getHistogramShortLabels() const
{
  return xmlHistogram.shortLabels;
}

// FILTERS XML SECTION : GLOBAL
void ParaverConfig::setFiltersFilterTraceUpToMB( float whichFilterTraceUpToMB )
{
  isModified = isModified || ( xmlFilters.filterTraceUpToMB != whichFilterTraceUpToMB );
  xmlFilters.filterTraceUpToMB = whichFilterTraceUpToMB;
}

void ParaverConfig::setFiltersXMLPath( string whichXMLPath )
{
  isModified = isModified || ( xmlFilters.xmlPath != whichXMLPath );
  xmlFilters.xmlPath = whichXMLPath;
}

float ParaverConfig::getFiltersFilterTraceUpToMB() const
{
  return xmlFilters.filterTraceUpToMB;
}

string ParaverConfig::getFiltersXMLPath() const
{
  return xmlFilters.xmlPath;
}

// FILTERS XML SECTION : CUTTER
void ParaverConfig::setCutterByTime( bool whichByTime )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.byTime != whichByTime );
  xmlFilters.xmlCutterInstance.byTime = whichByTime;
}

void ParaverConfig::setCutterMinimumTime( TTime whichMinTime )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.minimumTime != whichMinTime );
  xmlFilters.xmlCutterInstance.minimumTime = whichMinTime;
}

void ParaverConfig::setCutterMaximumTime( TTime whichMaxTime )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.maximumTime != whichMaxTime );
  xmlFilters.xmlCutterInstance.maximumTime = whichMaxTime;
}

void ParaverConfig::setCutterMinimumTimePercentage( TTime whichMinTimePercentage )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.minimumTimePercentage != whichMinTimePercentage );
  xmlFilters.xmlCutterInstance.minimumTimePercentage = whichMinTimePercentage;
}

void ParaverConfig::setCutterMaximumTimePercentage( TTime whichMaxTimePercentage )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.maximumTimePercentage != whichMaxTimePercentage );
  xmlFilters.xmlCutterInstance.maximumTimePercentage = whichMaxTimePercentage;
}

void ParaverConfig::setCutterOriginalTime( bool whichOriginalTime )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.originalTime != whichOriginalTime );
  xmlFilters.xmlCutterInstance.originalTime = whichOriginalTime;
}

// void setFiltersTasksList(  );
void ParaverConfig::setCutterBreakStates( bool whichBreakStates )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.breakStates != whichBreakStates );
  xmlFilters.xmlCutterInstance.breakStates = whichBreakStates;
}

void ParaverConfig::setCutterRemoveFirstStates( bool whichRemoveFirstStates )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.removeFirstStates != whichRemoveFirstStates );
  xmlFilters.xmlCutterInstance.removeFirstStates = whichRemoveFirstStates;
}

void ParaverConfig::setCutterRemoveLastStates( bool whichRemoveLastStates )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.removeLastStates != whichRemoveLastStates );
  xmlFilters.xmlCutterInstance.removeLastStates = whichRemoveLastStates;
}

void ParaverConfig::setCutterKeepEvents( bool keepEvents )
{
  isModified = isModified || ( xmlFilters.xmlCutterInstance.keepEvents != keepEvents );
  xmlFilters.xmlCutterInstance.keepEvents = keepEvents;
}

bool ParaverConfig::getCutterByTime()
{
  return xmlFilters.xmlCutterInstance.byTime;
}

TTime ParaverConfig::getCutterMinimumTime()
{
  return xmlFilters.xmlCutterInstance.minimumTime;
}

TTime ParaverConfig::getCutterMaximumTime()
{
  return xmlFilters.xmlCutterInstance.maximumTime;
}

TTime ParaverConfig::getCutterMinimumTimePercentage()
{
  return xmlFilters.xmlCutterInstance.minimumTimePercentage;
}

TTime ParaverConfig::getCutterMaximumTimePercentage()
{
  return xmlFilters.xmlCutterInstance.maximumTimePercentage;
}

bool ParaverConfig::getCutterOriginalTime()
{
  return xmlFilters.xmlCutterInstance.originalTime;
}

//  setFiltersTasksList(  );
bool ParaverConfig::getCutterBreakStates()
{
  return xmlFilters.xmlCutterInstance.breakStates;
}

bool ParaverConfig::getCutterRemoveFirstStates()
{
  return xmlFilters.xmlCutterInstance.removeFirstStates;
}

bool ParaverConfig::getCutterRemoveLastStates()
{
  return xmlFilters.xmlCutterInstance.removeLastStates;
}

bool ParaverConfig::getCutterKeepEvents()
{
  return xmlFilters.xmlCutterInstance.keepEvents;
}

// FILTERS XML SECTION : FILTER

void ParaverConfig::setFilterDiscardStates( bool discard )
{
  isModified = isModified || ( xmlFilters.xmlFilterInstance.discardStates != discard );
  xmlFilters.xmlFilterInstance.discardStates = discard;
}

void ParaverConfig::setFilterDiscardEvents( bool discard )
{
  isModified = isModified || ( xmlFilters.xmlFilterInstance.discardEvents != discard );
  xmlFilters.xmlFilterInstance.discardEvents = discard;
}

void ParaverConfig::setFilterDiscardCommunications( bool discard )
{
  isModified = isModified || ( xmlFilters.xmlFilterInstance.discardCommunications != discard );
  xmlFilters.xmlFilterInstance.discardCommunications = discard;
}

void ParaverConfig::setFilterCommunicationsMinimumSize( TCommSize size )
{
  isModified = isModified || ( xmlFilters.xmlFilterInstance.communicationsMinimumSize != size );
  xmlFilters.xmlFilterInstance.communicationsMinimumSize = size;
}

bool ParaverConfig::getFilterDiscardStates()
{
  return xmlFilters.xmlFilterInstance.discardStates;
}

bool ParaverConfig::getFilterDiscardEvents()
{
  return xmlFilters.xmlFilterInstance.discardEvents;
}

bool ParaverConfig::getFilterDiscardCommunications()
{
  return xmlFilters.xmlFilterInstance.discardCommunications;
}

TCommSize ParaverConfig::getFilterCommunicationsMinimumSize()
{
  return xmlFilters.xmlFilterInstance.communicationsMinimumSize;
}


// FILTERS XML SECTION : SOFTWARE COUNTERS
void ParaverConfig::setSoftwareCountersInvervalsOrStates( bool whichIntervalsOrStates )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.intervalsOrStates != whichIntervalsOrStates );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.intervalsOrStates = whichIntervalsOrStates;
}

void ParaverConfig::setSoftwareCountersSamplingInterval( TTime whichSamplingInterval )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.samplingInterval != whichSamplingInterval );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.samplingInterval = whichSamplingInterval;
}

void ParaverConfig::setSoftwareCountersMinimumBurstTime( TTime whichMinimumBurstTime )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.minimumBurstTime != whichMinimumBurstTime );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.minimumBurstTime = whichMinimumBurstTime;
}

void ParaverConfig::setSoftwareCountersTypes( string whichTypes )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.types != whichTypes );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.types = whichTypes;
}

void ParaverConfig::setSoftwareCountersCountEventsOrAcummulateValues( bool whichCountEventsOrAcummulateValues )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.countEventsOrAcummulateValues != whichCountEventsOrAcummulateValues );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.countEventsOrAcummulateValues = whichCountEventsOrAcummulateValues;
}

void ParaverConfig::setSoftwareCountersRemoveStates( bool whichRemoveStates )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.removeStates != whichRemoveStates );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.removeStates = whichRemoveStates;
}

void ParaverConfig::setSoftwareCountersSummarizeStates( bool whichSummarizeStates )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.summarizeStates != whichSummarizeStates );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.summarizeStates = whichSummarizeStates;
}

void ParaverConfig::setSoftwareCountersGlobalCounters( bool whichGlobalCounters )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.globalCounters != whichGlobalCounters );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.globalCounters = whichGlobalCounters;
}

void ParaverConfig::setSoftwareCountersOnlyInBursts( bool whichOnlyInBursts )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.onlyInBursts != whichOnlyInBursts );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.onlyInBursts = whichOnlyInBursts;
}

void ParaverConfig::setSoftwareCountersTypesKept( string whichTypesKept )
{
  isModified = isModified || ( xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.typesKept != whichTypesKept );
  xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.typesKept = whichTypesKept;
}


bool ParaverConfig::getSoftwareCountersInvervalsOrStates()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.intervalsOrStates;
}

TTime ParaverConfig::getSoftwareCountersSamplingInterval()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.samplingInterval;
}

TTime ParaverConfig::getSoftwareCountersMinimumBurstTime()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.minimumBurstTime;
}

string ParaverConfig::getSoftwareCountersTypes()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCRangeInstance.types;
}

bool ParaverConfig::getSoftwareCountersCountEventsOrAcummulateValues()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.countEventsOrAcummulateValues;
}

bool ParaverConfig::getSoftwareCountersRemoveStates()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.removeStates;
}

bool  ParaverConfig::getSoftwareCountersSummarizeStates()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.summarizeStates;
}

bool  ParaverConfig::getSoftwareCountersGlobalCounters()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.globalCounters;
}

bool  ParaverConfig::getSoftwareCountersOnlyInBursts()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.onlyInBursts;
}

//bool ParaverConfig::getSoftwareCountersFrequency() // not used!!!

string  ParaverConfig::getSoftwareCountersTypesKept()
{
  return xmlFilters.xmlSoftwareCountersInstance.xmlSCAlgorithmInstance.typesKept;
}

// COLORS XML SECTION
void ParaverConfig::setColorsTimelineBackground( rgb whichTimelineBackground )
{
  isModified = isModified || ( xmlColor.timelineBackground != whichTimelineBackground );
  xmlColor.timelineBackground = whichTimelineBackground;
}

void ParaverConfig::setColorsTimelineAxis( rgb whichTimelineAxis )
{
  isModified = isModified || ( xmlColor.timelineAxis != whichTimelineAxis );
  xmlColor.timelineAxis = whichTimelineAxis;
}

void ParaverConfig::setColorsTimelineZeroDashLine( rgb whichTimelineZeroDashLine )
{
  isModified = isModified || ( xmlColor.timelineZeroDashLine != whichTimelineZeroDashLine );
  xmlColor.timelineZeroDashLine = whichTimelineZeroDashLine;
}

void ParaverConfig::setColorsTimelineUseZero( bool useZero )
{
  isModified = isModified || ( xmlColor.useColorZero != useZero );
  xmlColor.useColorZero = useZero;
}

void ParaverConfig::setColorsTimelineColorZero( rgb whichTimelineZero )
{
  isModified = isModified || ( xmlColor.timelineColorZero != whichTimelineZero );
  xmlColor.timelineColorZero = whichTimelineZero;
}

void ParaverConfig::setColorsTimelinePunctual( rgb whichPunctual )
{
  isModified = isModified || ( xmlColor.timelineColorPunctual != whichPunctual );
  xmlColor.timelineColorPunctual = whichPunctual;
}

void ParaverConfig::setColorsTimelineLogicalCommunications( rgb whichTimelineLogicalCommunications )
{
  isModified = isModified || ( xmlColor.timelineLogicalCommunications != whichTimelineLogicalCommunications );
  xmlColor.timelineLogicalCommunications = whichTimelineLogicalCommunications;
}

void ParaverConfig::setColorsTimelinePhysicalCommunications( rgb whichTimelinePhysicalCommunications )
{
  isModified = isModified || ( xmlColor.timelinePhysicalCommunications != whichTimelinePhysicalCommunications );
  xmlColor.timelinePhysicalCommunications = whichTimelinePhysicalCommunications;
}

void ParaverConfig::setColorsTopGradient( rgb whichTopGradient )
{
  isModified = isModified || ( xmlColor.topGradient != whichTopGradient );
  xmlColor.topGradient = whichTopGradient;
}

void ParaverConfig::setColorsLowGradient( rgb whichLowGradient )
{
  isModified = isModified || ( xmlColor.lowGradient != whichLowGradient );
  xmlColor.lowGradient = whichLowGradient;
}

void ParaverConfig::setColorsBeginGradient( rgb whichBeginGradient )
{
  isModified = isModified || ( xmlColor.beginGradient != whichBeginGradient );
  xmlColor.beginGradient = whichBeginGradient;
}

void ParaverConfig::setColorsEndGradient( rgb whichEndGradient )
{
  isModified = isModified || ( xmlColor.endGradient != whichEndGradient );
  xmlColor.endGradient = whichEndGradient;
}

void ParaverConfig::setColorsBeginNegativeGradient( rgb whichBeginGradient )
{
  isModified = isModified || ( xmlColor.beginNegativeGradient != whichBeginGradient );
  xmlColor.beginNegativeGradient = whichBeginGradient;
}

void ParaverConfig::setColorsEndNegativeGradient( rgb whichEndGradient )
{
  isModified = isModified || ( xmlColor.endNegativeGradient != whichEndGradient );
  xmlColor.endNegativeGradient = whichEndGradient;
}

rgb ParaverConfig::getColorsTimelineBackground() const
{
  return xmlColor.timelineBackground;
}

rgb ParaverConfig::getColorsTimelineAxis() const
{
  return xmlColor.timelineAxis;
}

rgb ParaverConfig::getColorsTimelineZeroDashLine() const
{
  return xmlColor.timelineZeroDashLine;
}

bool ParaverConfig::getColorsTimelineUseZero() const
{
  return xmlColor.useColorZero;
}

rgb ParaverConfig::getColorsTimelineColorZero() const
{
  return xmlColor.timelineColorZero;
}

rgb ParaverConfig::getColorsTimelinePunctual() const
{
  return xmlColor.timelineColorPunctual;
}

rgb ParaverConfig::getColorsTimelineLogicalCommunications() const
{
  return xmlColor.timelineLogicalCommunications;
}

rgb ParaverConfig::getColorsTimelinePhysicalCommunications() const
{
  return xmlColor.timelinePhysicalCommunications;
}

rgb ParaverConfig::getColorsTopGradient() const
{
  return xmlColor.topGradient;
}

rgb ParaverConfig::getColorsLowGradient() const
{
  return xmlColor.lowGradient;
}

rgb ParaverConfig::getColorsBeginGradient() const
{
  return xmlColor.beginGradient;
}

rgb ParaverConfig::getColorsEndGradient() const
{
  return xmlColor.endGradient;
}

rgb ParaverConfig::getColorsBeginNegativeGradient() const
{
  return xmlColor.beginNegativeGradient;
}

rgb ParaverConfig::getColorsEndNegativeGradient() const
{
  return xmlColor.endNegativeGradient;
}


void ParaverConfig::setGlobalExternalTextEditors( std::vector< std::string> whichTextEditors )
{
  isModified = isModified || ( xmlExternalApplications.myTextEditors != whichTextEditors );
  xmlExternalApplications.myTextEditors = whichTextEditors;
}

void ParaverConfig::setGlobalExternalPDFReaders( std::vector< std::string> whichPDFReaders )
{
  isModified = isModified || ( xmlExternalApplications.myPDFReaders != whichPDFReaders );
  xmlExternalApplications.myPDFReaders = whichPDFReaders;
}

std::vector< std::string> ParaverConfig::getGlobalExternalTextEditors() const
{
  return xmlExternalApplications.myTextEditors;
}

std::vector< std::string> ParaverConfig::getGlobalExternalPDFReaders() const
{
  return xmlExternalApplications.myPDFReaders;
}


bool ParaverConfig::initCompleteSessionFile()
{
  fstream file;
  string strFile;
  string strLine;
  bool status = false;

#ifdef WIN32
  strFile = getenv( "HOMEDRIVE" );
  strFile.append( getenv( "HOMEPATH" ) );
  strFile.append( "\\paraver\\CompleteSession" );
#else
  strFile = getenv( "HOME" );
  strFile.append( "/.paraver/CompleteSession" );
#endif

  file.open( strFile.c_str(), ios::in );
  if ( !file.fail() )
  {
    string line;
    getline( file, line );
    status = ( line == "Previous session executed successfully" );
    //file.close();
    //file.open( strFile.c_str(), ios::out | ios::trunc);
  }
  else
  {
    //file.open( strFile.c_str(), ios::out );
    status = true;
  }
  file.close();
  return status;
}

void ParaverConfig::cleanCompleteSessionFile()
{
  fstream file;
  string strFile;
  string strLine;

#ifdef WIN32
  strFile = getenv( "HOMEDRIVE" );
  strFile.append( getenv( "HOMEPATH" ) );
  strFile.append( "\\paraver\\CompleteSession" );
#else
  strFile = getenv( "HOME" );
  strFile.append( "/.paraver/CompleteSession" );
#endif

  file.open( strFile.c_str(), ios::in);
  //if ( !file.fail() )
  //{
  file.close();
  file.open( strFile.c_str(), ios::out | ios::trunc);
  /*}
  else
  {
    file.close();
    file.open( strFile.c_str(), ios::out );
  }*/
  file.close();
}


bool ParaverConfig::getAppsChecked() const
{
  return xmlGlobal.appsChecked;
}

bool ParaverConfig::closeCompleteSessionFile()
{
  ofstream file;
  string strFile;
  string strLine;
  bool status = false;

#ifdef WIN32
  strFile = getenv( "HOMEDRIVE" );
  strFile.append( getenv( "HOMEPATH" ) );
  strFile.append( "\\paraver\\CompleteSession" );
#else
  strFile = getenv( "HOME" );
  strFile.append( "/.paraver/CompleteSession" );
#endif

  file.open( strFile.c_str(), ios::in | ios::out);
  if ( !file.fail() )
  {
    file << "Previous session executed successfully" << endl;
    status = true;
    file.close();
  }
  return status;
}


void ParaverConfig::readParaverConfigFile()
{
  ifstream file;
  ifstream fileXML;
  string strLine;
  string strTag;
  string strFile;
  string strFileXML;
  string strBackupFileXML;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );
#ifdef WIN32
  strFile.append( "\\paraver\\paraver" );
#else
  strFile.append( "/.paraver/paraver" );
#endif
  strFileXML = strFile;
  strFileXML.append( ".xml" );
  strBackupFileXML = strFile;
  strBackupFileXML.append( "_backup.xml" );

  fileXML.open( strFileXML.c_str() );

  if ( !fileXML )
  {
    // XML file can't be opened
    fileXML.close();

    // Try to open backup file
    fileXML.open( strBackupFileXML.c_str() );

    if ( !fileXML )
    {
      // Backup XML file can't be opened
      fileXML.close();

      // Does file in old format exists?
      file.open( strFile.c_str() );
      if ( file )
      {
        // Read it!
        while ( !file.eof() )
        {
          getline( file, strLine );
          if ( strLine.length() == 0 )
            continue;
          else if ( strLine[ 0 ] == '#' || strLine[ 0 ] == '<' )
            continue;

          istringstream auxStream( strLine );
          getline( auxStream, strTag, ' ' );

          map<string, PropertyFunction*>::iterator it =
            propertyFunctions.find( strTag );
          if ( it != propertyFunctions.end() )
          {
            it->second->parseLine( auxStream, *this );
          }
        }
        file.close();
      }


      // Try to create/rewrite new XML format
      if ( !ParaverConfig::writeDefaultConfig() )
        return;
      else
      { // Right! And also write old info.
        ParaverConfig::writeParaverConfigFile();
      }
    }
    else
    { 
      // Backup XML file found ! load it!
      fileXML.close();
      loadXML( strBackupFileXML );
    }
  }
  else
  {
    // XML file found ! load it!
    fileXML.close();
    loadXML( strFileXML );
  }
}


void ParaverConfig::writeParaverConfigFile( bool writeBackup )
{
  string homedir;
  string strFile, strBackupFile;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );

#ifdef WIN32
  strFile.append( "\\paraver\\paraver" );
  string tmpPath( homedir + "\\paraver" );

  int len = tmpPath.length() + 1;
  wchar_t *wText = new wchar_t[len];
  memset(wText,0,len);
  ::MultiByteToWideChar( CP_ACP, NULL, tmpPath.c_str(), -1, wText, len );

  SHCreateDirectoryEx( NULL, wText, NULL );
  delete []wText;
#else
  strFile.append( "/.paraver/paraver" );
  mkdir( ( homedir + "/.paraver" ).c_str(), (mode_t)0700 );
#endif

  // copy paraver.xml file into a backup
  strBackupFile = strFile;
  strFile.append( ".xml" );

  if( writeBackup && instance->isModified )
  {
    strBackupFile.append( "_backup.xml" );
    std::ifstream src( strFile.c_str() );
    std::ofstream dst( strBackupFile.c_str() );
    dst << src.rdbuf();
    src.close();
    dst.close();
  }  
  instance->saveXML( strFile );
  
  instance->isModified = false;
}


bool ParaverConfig::writeDefaultConfig()
{
  ofstream file;
  string strFile;
  string homedir;

#ifdef WIN32
  homedir = getenv( "HOMEDRIVE" );
  homedir.append( getenv( "HOMEPATH" ) );
#else
  homedir = getenv( "HOME" );
#endif
  strFile.append( homedir );

#ifdef WIN32
  strFile.append( "\\paraver\\paraver.xml" );
  string tmpPath( homedir + "\\paraver" );

  int len = tmpPath.length() + 1;
  wchar_t *wText = new wchar_t[len];
  memset(wText,0,len);
  ::MultiByteToWideChar( CP_ACP, NULL, tmpPath.c_str(), -1, wText, len );

  SHCreateDirectoryEx( NULL, wText, NULL );
  delete []wText;
#else
  strFile.append( "/.paraver/paraver.xml" );
  mkdir( ( homedir + "/.paraver" ).c_str(), (mode_t)0700 );
#endif

  file.open( strFile.c_str() );

  if ( !file )
    return false;

  file.close();

  return true;
}

void ParaverConfig::saveXML( const string &filename )
{
  std::ofstream ofs( filename.c_str() );
  boost::archive::xml_oarchive oa( ofs );
  oa << boost::serialization::make_nvp( "paraver_configuration", *this );
}

void ParaverConfig::loadXML( const string &filename )
{
  std::ifstream ifs( filename.c_str() );
  boost::archive::xml_iarchive ia( ifs );
  ia >> boost::serialization::make_nvp( "paraver_configuration", *this );
}

void ParaverConfig::loadMap()
{
  propertyFunctions[ "WhatWhere.num_decimals:" ] = new WWNumDecimals();
  propertyFunctions[ "Analyzer2D.num_columns:" ] = new HistoNumColumns();
  propertyFunctions[ "Analyzer2D.units:" ] = new HistoUnits();
  propertyFunctions[ "Analyzer2D.thousandsep:" ] = new HistoThousanSep();
}

void ParaverConfig::unloadMap()
{
  for ( map<string, PropertyFunction*>::iterator it = propertyFunctions.begin();
        it != propertyFunctions.end();
        ++it )
    delete ( *it ).second;
}

void WWNumDecimals::parseLine( istringstream& line, ParaverConfig& config )
{
  string strNumDecimals;
  PRV_UINT32 precision;

  getline( line, strNumDecimals );
  istringstream streamNumDecimals( strNumDecimals );

  if ( streamNumDecimals >> precision )
    config.setHistogramPrecision( precision );
}

void HistoNumColumns::parseLine( istringstream& line, ParaverConfig& config )
{
  string strNumColumns;
  TObjectOrder numColumns;

  getline( line, strNumColumns );
  istringstream streamNumColumns( strNumColumns );

  if ( streamNumColumns >> numColumns )
    config.setHistogramNumColumns( numColumns );
}

void HistoUnits::parseLine( istringstream& line, ParaverConfig& config )
{
  string strUnits;

  getline( line, strUnits );

  if ( strUnits.compare( "True" ) == 0 )
    config.setHistogramShowUnits( true );
  else
    config.setHistogramShowUnits( false );
}

void HistoThousanSep::parseLine( istringstream& line, ParaverConfig& config )
{
  string strSep;

  getline( line, strSep );

  if ( strSep.compare( "True" ) == 0 )
    config.setHistogramThousandSep( true );
  else
    config.setHistogramThousandSep( false );
}
