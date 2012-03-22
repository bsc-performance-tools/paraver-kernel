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

#include <time.h>
#include <sys/stat.h>
#include "localkernel.h"
#include "kwindow.h"
#include "khistogram.h"
#include "histogramstatistic.h"
#include "semanticmanagement.h"
#include "filtermanagement.h"
#include "statisticmanagement.h"
#include "functionmanagement.h"
#include "krecordlist.h"
#include "kprogresscontroller.h"
#include "labelconstructor.h"
#include "previousfiles.h"


#include "ktraceoptions.h"
#include "ktracecutter.h"
#include "ktracefilter.h"
#include "ktracesoftwarecounters.h"
#include "tracestream.h"
#include <string.h>

using namespace std;

void LocalKernel::init()
{
  srand( time( NULL ) );

  createFilter();
  createSemantic();
  createStatistic();

  LabelConstructor::init();
}

LocalKernel::LocalKernel( bool ( *messageFunction )( string ) ) :
    myMessageFunction( messageFunction )
{
  setPathSeparator( string( "/") ); // FIXME

  // FILTERS
  trace_names_table_last = 0;

  PreviousFiles *prevTraceNames = PreviousFiles::createPreviousCutFilteredTraces();
}

LocalKernel::~LocalKernel()
{
//  deleted prevTracesNames;
}

bool LocalKernel::checkTraceSize( const string& filename, TTraceSize maxSize ) const
{
  TTraceSize size = TraceStream::getTraceFileSize( filename );

  if( size <= maxSize )
    return true;

  return false;
}

TTraceSize LocalKernel::getTraceSize( const string& filename ) const
{
  return TraceStream::getTraceFileSize( filename );
}

Trace *LocalKernel::newTrace( const string& whichFile, bool noLoad, ProgressController *progress ) const
{
  if ( progress == NULL )
    return new KTrace( whichFile, NULL, noLoad );

  return new KTrace( whichFile, ( KProgressController * ) progress->getConcrete(), noLoad );
}

string LocalKernel::getPCFFileLocation( const string& traceFile ) const
{
  string pcfFile;

  if ( traceFile.substr( traceFile.length() - 6 ) == "prv.gz" )
    pcfFile = traceFile.substr( 0, traceFile.length() - 6 );
  else
    pcfFile = traceFile.substr( 0, traceFile.length() - 3 );
  pcfFile.append( "pcf" );

  return pcfFile;
}

string LocalKernel::getROWFileLocation( const string& traceFile ) const
{
  string rowFile;

  if ( traceFile.substr( traceFile.length() - 6 ) == "prv.gz" )
    rowFile = traceFile.substr( 0, traceFile.length() - 6 );
  else
    rowFile = traceFile.substr( 0, traceFile.length() - 3 );
  rowFile.append( "row" );

  return rowFile;
}

Window *LocalKernel::newSingleWindow() const
{
  return new KSingleWindow();
}

Window *LocalKernel::newSingleWindow( Trace *whichTrace ) const
{
  return new KSingleWindow( ( KTrace * ) whichTrace->getConcrete() );
}

Window *LocalKernel::newDerivedWindow( Window *window1, Window * window2 ) const
{
  return new KDerivedWindow( ( KWindow * ) window1->getConcrete(),
                             ( KWindow * ) window2->getConcrete() );
}

Window *LocalKernel::newDerivedWindow() const
{
  return new KDerivedWindow();
}

Histogram *LocalKernel::newHistogram() const
{
  return new KHistogram();
}

/*RecordList *LocalKernel::newRecordList() const
{
  return new KRecordList();
}*/

ProgressController *LocalKernel::newProgressController() const
{
  return new KProgressController();
}

Filter *LocalKernel::newFilter( Filter *concreteFilter ) const
{
  FilterProxy *tmpFilter = new FilterProxy( this );
  tmpFilter->myFilter = concreteFilter;
  return ( Filter * ) tmpFilter;
}

TraceOptions *LocalKernel::newTraceOptions() const
{
  return new KTraceOptions( this );
}

TraceCutter *LocalKernel::newTraceCutter( //TraceCutter *concreteTraceCutter,
                                          char *trace_in,
                                          char *trace_out,
                                          TraceOptions *options,
                                          const vector< TEventType > &whichTypesWithValuesZero,
                                          ProgressController *progress ) const
{
//  cout << "LocalKernel::newTraceCutter: tracein " << trace_in  << endl;
//  cout << "LocalKernel::newTraceCutter: traceout" << trace_out << endl;

  //TraceOptionsProxy *myOptions = new TraceOptionsProxy( this );
  //myOptions->myTraceOptions = options;
/*  TraceCutterProxy *myCutter = new TraceCutterProxy( this,
                                                     trace_in,
                                                     trace_out,
                                                     options );
*/
  KProgressController *tmpKProgressControler = NULL;

  if ( progress != NULL )
    tmpKProgressControler = (KProgressController *)progress->getConcrete();

  return new KTraceCutter( trace_in, trace_out, options,  whichTypesWithValuesZero , tmpKProgressControler);
//  return ( TraceCutter *) myCutter;

}


TraceFilter *LocalKernel::newTraceFilter( char *trace_in,
                                          char *trace_out,
                                          TraceOptions *options,
                                          ProgressController *progress,
                                          const std::map< TTypeValuePair, TTypeValuePair > whichTranslationTable ) const
{
//  TraceOptionsProxy *myOptions = new TraceOptionsProxy( this );
//  myOptions->myTraceOptions = options;

  KProgressController *tmpKProgressControler = NULL;

  if ( progress != NULL )
    tmpKProgressControler = (KProgressController *)progress->getConcrete();

  return new KTraceFilter( trace_in, trace_out, options, tmpKProgressControler, whichTranslationTable );

}


TraceSoftwareCounters *LocalKernel::newTraceSoftwareCounters( char *trace_in,
                                                              char *trace_out,
                                                              TraceOptions *options,
                                                              ProgressController *progress ) const
{

  KProgressController *tmpKProgressControler = NULL;

  if ( progress != NULL )
    tmpKProgressControler = (KProgressController *)progress->getConcrete();

  return new KTraceSoftwareCounters( trace_in, trace_out, options, tmpKProgressControler );
}

void LocalKernel::getAllStatistics( vector<string>& onVector ) const
{
  FunctionManagement<HistogramStatistic>::getInstance()->getAll( onVector );
}

void LocalKernel::getAllFilterFunctions( vector<string>& onVector ) const
{
  FunctionManagement<FilterFunction>::getInstance()->getAll( onVector );
}

void LocalKernel::getAllSemanticFunctions( TSemanticGroup whichGroup,
    vector<string>& onVector ) const
{
  FunctionManagement<SemanticFunction>::getInstance()->getAll( onVector,
      whichGroup );
}

bool LocalKernel::userMessage( const string& message ) const
{
  if( myMessageFunction == NULL )
    return true;

  return myMessageFunction( message );
}

char *LocalKernel::composeName( char *name, char *newExtension )
{
  int zip_filter = 0;
  char *fileName = strdup( name );
  char *aux;

  if ( ( aux = strrchr( fileName, '.' ) ) != NULL )
  {
    /* The names finishes with .gz */
    if ( !strcmp( aux, ".gz" ) )
      zip_filter = 7;
    else
      zip_filter = 4;
  }

  fileName[ strlen( fileName ) - zip_filter ] = '\0';
  sprintf( fileName, "%s.%s", fileName, newExtension );

  return fileName;
}

void LocalKernel::copyFile( char *in, char *out )
{
  FILE *fileIn, *fileOut;
  char line[2048];

  if ( ( fileIn = fopen( in, "r" ) ) == NULL )
    return;

  fileOut = fopen( out, "w" );

  while ( fgets( line, sizeof( line ), fileIn ) != NULL )
    fputs( line, fileOut );

  fclose( fileIn );
  fclose( fileOut );
}

void LocalKernel::copyPCF( char *name, char *traceToLoad )
{
/*
  char *pcfIn = strdup( name );
  char *pcfOut = strdup( traceToLoad );
  pcfIn[strlen( pcfIn )-4] = '\0';
  pcfOut[strlen( pcfOut )-4] = '\0';
  sprintf( pcfIn, "%s.pcf", pcfIn );
  sprintf( pcfOut, "%s.pcf", pcfOut );
*/
  char *pcfIn  = composeName( name, (char *)string("pcf").c_str() );
  char *pcfOut = composeName( traceToLoad, (char *)string("pcf").c_str() );

  copyFile( pcfIn, pcfOut );

  free( pcfIn );
  free( pcfOut );
}

void LocalKernel::copyROW( char *name, char *traceToLoad )
{
/*
  char *rowIn = strdup( name );
  char *rowOut = strdup( traceToLoad );
  rowIn[strlen( rowIn )-4] = '\0';
  rowOut[strlen( rowOut )-4] = '\0';
  sprintf( rowIn, "%s.row", rowIn );
  sprintf( rowOut, "%s.row", rowOut );
  copyFile( rowIn, rowOut );
*/
  char *rowIn = composeName( name, (char *)string("row").c_str() );
  char *rowOut = composeName( traceToLoad, (char *)string("row").c_str() );

  copyFile( rowIn, rowOut );

  free( rowIn );
  free( rowOut );
}

void LocalKernel::getNewTraceName( char *name,
                                   char *new_trace_name,
                                   string action,
                                   bool saveNewNameInfo )
{
  /* Putting on table of names in order to generate new trace names */
  int i;
#ifndef WIN32
  struct stat file_info;
#else
  struct _stat64 file_info;
#endif
  char *c;
  char path_name_backup[1024];
  char traceToLoad[1024];
  char *traces_home;

  // Used when
  struct traces_table currentName;

  traceToLoad[0] = '\0';
  /* De forma provisional agafem var entorn */
 // if (( traces_home = getenv( "TRACES_HOME" )) != NULL)
  if (( traces_home = getenv( "PWD" )) != NULL)
    sprintf(traceToLoad,"%s",traces_home);

//cout << traceToLoad << endl;

  strcpy( path_name_backup, new_trace_name );

  // first: look for name in the table
  // precond: trace_names_table_last < MAX_TRACES_HISTORY_LENGTH
  for ( i = 0; i < trace_names_table_last; i++ )
    if ( !strcmp( name, trace_names_table[i].name ) )
      break;

    // second: found? --> ok; not found? --> insert it!
  if ( i == trace_names_table_last )
  {
    if ( saveNewNameInfo )
    {
      // not found!
      if ( trace_names_table_last >= MAX_TRACES_HISTORY_LENGTH )
      {
        // we haven't room; insert as a circular buffer, from the beginning
        trace_names_table_last = 0;
        i = 0;
        free( trace_names_table[ trace_names_table_last ].name );
      }

      // insert it
      trace_names_table[ trace_names_table_last ].name = ( char * )strdup( name );
      trace_names_table[ trace_names_table_last ].num_chop = 0;
      trace_names_table[ trace_names_table_last ].num_sc = 0;
      trace_names_table[ trace_names_table_last ].num_filter = 0;

      // and advance counter
      trace_names_table_last++;
    }
    else
    {
      currentName.name = ( char * )strdup( name );
      currentName.num_chop = 0;
      currentName.num_sc = 0;
      currentName.num_filter = 0;
    }
  }
  else
  {
    currentName = trace_names_table[ i ];
  }

  /* Generating the new trace name and preventing to erase old chops */
  do
  {
    if ( action == TraceCutter::getID() )
    {
      currentName.num_chop++;
    }
    else if ( action == TraceFilter::getID() )
    {
      currentName.num_filter++;
    }
    else if ( action == TraceSoftwareCounters::getID() )
    {
      currentName.num_sc++;
    }
    else
    {
    }

    sprintf( new_trace_name, "%s", path_name_backup );

    if ( strlen( new_trace_name ) > 0 )
    {
      c = strrchr( name, '/' );

      if ( c != NULL )
      {
        c++;
        if ( new_trace_name[strlen( new_trace_name )-1] == '/' )
          sprintf( new_trace_name, "%s%s", new_trace_name, c );
        else
          sprintf( new_trace_name, "%s/%s", new_trace_name, c );
      }
      else
      {
        sprintf( new_trace_name, "%s", name );
      }
    }
    else
      sprintf( new_trace_name, "%s", name );

    c = &new_trace_name[strlen( new_trace_name )-3];

    if ( !strcmp( c, ".gz" ) )
      new_trace_name[strlen( new_trace_name )-6] = '\0';
    else
      new_trace_name[strlen( new_trace_name )-3] = '\0';

    string dotExtension(".");
    string toolInfix( "" );
    int numInfix = 0;
    if ( action == TraceCutter::getID() )
    {
      dotExtension += TraceCutter::getExtension();

      if ( strstr( new_trace_name, dotExtension.c_str() ) == NULL )
        toolInfix = TraceCutter::getExtension();

      numInfix = currentName.num_chop;
    }
    else if ( action == TraceFilter::getID() )
    {
      dotExtension += TraceFilter::getExtension();

      if ( strstr( new_trace_name, dotExtension.c_str() ) == NULL )
        toolInfix = TraceFilter::getExtension();

      numInfix = currentName.num_filter;
    }
    else if ( action == TraceSoftwareCounters::getID() )
    {
      dotExtension += TraceSoftwareCounters::getExtension();

      if ( strstr( new_trace_name, dotExtension.c_str() ) == NULL )
        toolInfix = TraceSoftwareCounters::getExtension();

      numInfix = currentName.num_sc;
    }
    else
    {
    }

    sprintf( new_trace_name, "%s%s%d.prv", new_trace_name, toolInfix.c_str(), numInfix );
  }
#ifndef WIN32
  while ( stat( new_trace_name, &file_info ) == 0 );
#else
  while ( _stat64( new_trace_name, &file_info ) == 0 );
#endif

  if ( saveNewNameInfo )
  {
    trace_names_table[ i ] = currentName;
  }
}


string LocalKernel::getNewTraceName( const string& fullPathTraceName,
                                     const string& traceFilterID ) const
{
  string newTraceName = fullPathTraceName;
  return newTraceName;
}


// precond1: fullPathTraceName is well formed ( ends with *.prv.gz or *.prv, and path exists)
// precond2: traceids belong to available tools and are inserted in order
string LocalKernel::getNewTraceName( const string& fullPathTraceName,
                                     const vector< string >& traceFilterID,
                                     const bool commitName ) const
{
  string newTraceName;
  /*
  string retTraceName;

  map< string, int > toolNumberedSuffix; // ( key = id, value = version )
  map< string, string > toolExtension;   // ( key = id, value = extension )

  string auxTraceName;
  string fullPath = "";
  string auxPath;
  string newTraceNameGzipped;
  char *tracesHome;

#ifndef WIN32
  struct stat file_info;
#else
  struct _stat64 file_info;
#endif

  // Constants
  string gzippedPrvSuffix = string( ".prv.gz" );
  string prvSuffix = string( ".prv" );
  string pathSep = getPathSeparator();

  // Blind build of the new trace name
  size_t uniqueStartingPos = fullPathTraceName.length() - gzippedPrvSuffix.length();
  size_t prvSuffixStartPos = fullPathTraceName.rfind( gzippedPrvSuffix, uniqueStartingPos );
  if ( prvSuffixStartPos == string::npos )
  {
    uniqueStartingPos = fullPathTraceName.length() - prvSuffix.length();
    prvSuffixStartPos = fullPathTraceName.rfind( prvSuffix, uniqueStartingPos );
  }

  retTraceName = fullPathTraceName.substr( 0, prvSuffixStartPos );

  for( vector< string >::iterator id = traceFilterID.begin(); id != traceFilterID.end(); ++id )
  {
    retTraceName = retTraceName +
            string( "." ) + TraceOptionsProxy::getTraceToolExtension( *id ) + string("1");
  }

  newTraceName

  for( vector< string >::iterator id = availableTraceToolsIDs.begin(); id != availableTraceToolsIDs.end(); ++id )
  {
     prvExtension = TraceOptionsProxy::getTraceToolExtension( *id );
      dottedExtension = string( "." ) + extension;
      infixStartPos = tr->rfind( dottedExtension );


  // Initialize variables to describe current state
  vector< string > availableTraceToolsIDs = TraceOptionsProxy::getIDsAvailableTraceTools();
  vector< string > tracesHistory = prevTraceNames->getFiles();

  for( vector< string >::iterator tr = tracesHistory.begin(); tr != tracesHistory.end(); ++tr )
  {
    // Look for fullPathTraceName
    // found --> read suffixes and increment
    // !found --> append ordered suffixes with versions = 1
    // in both cases, add to list

    toolNumberedSuffix[ *tr ] = map< string, int >();
    toolExtension[ *tr ] =  map< string, string >();

    for( vector< string >::iterator id = availableTraceToolsIDs.begin(); id != availableTraceToolsIDs.end(); ++id )
    {
      extension = TraceOptionsProxy::getTraceToolExtension( *id );
      dottedExtension = string( "." ) + extension;
      infixStartPos = tr->rfind( dottedExtension );
      if ( infixStartPos != string::npos )
      {
        // found! => get the number
        infixStartNumberPos = infixStartPos + toolExtension[ *it ].length() + 1;
        infixFinalDotPos = auxTraceName.find( string("."), infixStartNumberPos );
        if ( infixStartPos != string::npos )
        {
          auxStrNumber = auxTraceName.substr( infixStartNumberPos,
                                              infixFinalDotPos - infixStartNumberPos );
          stringstream auxStrStrNumber( auxStrNumber );
          if ( !( auxStrStrNumber >> auxNumber ).fail() )
          {
            toolNumberedSuffix[ *it ] = auxNumber;
      toolNumberedSuffix[ *tr ][ *at ] = 0;
      toolExtension[ *tr ][ *at ] = TraceOptionsProxy::getTraceToolExtension( *it );

          }
        }
      }
    }
  }

  // Does fullPathTraceName contains the numbered infix?

  // Step 1: Forget the path to the base directory
  size_t lastPathSepPos = fullPathTraceName.find_last_of( pathSep );
  if ( lastPathSepPos != string::npos )
  {
    auxTraceName = fullPathTraceName.substr( lastPathSepPos );
    auxPath = fullPathTraceName.substr( 0, lastPathSepPos );
cout << "auxTraceName" << auxTraceName << endl;
    //fullPath =
  }

  // Step 2: Find tools substrings in trace name (i.e, for cutter, ".chop")
  // Must be done for every filter in same order
  string auxExtension, auxStrNumber;
  size_t infixStartPos,infixStartNumberPos,infixFinalDotPos;
  PRV_UINT32 auxNumber;

  for( vector< string >::iterator it = tmpAvailableTraceTools.begin();
          it != tmpAvailableTraceTools.end(); ++it )
  {
    auxExtension = string( "." ) + toolExtension[ *it ];
    infixStartPos = auxTraceName.rfind( auxExtension );
    if ( infixStartPos != string::npos )
    {
      // found! => get the number
      infixStartNumberPos = infixStartPos + toolExtension[ *it ].length() + 1;
      infixFinalDotPos = auxTraceName.find( string("."), infixStartNumberPos );
      if ( infixStartPos != string::npos )
      {
        auxStrNumber = auxTraceName.substr( infixStartNumberPos,
                                            infixFinalDotPos - infixStartNumberPos );
        stringstream auxStrStrNumber( auxStrNumber );
        if ( !( auxStrStrNumber >> auxNumber ).fail() )
        {
          toolNumberedSuffix[ *it ] = auxNumber;
        }
      }
    }
  }

  // Get current directory
  // Today, getenv( "TRACES_HOME" )) ) is almost deprecated
  if (( tracesHome = getenv( "PWD" )) != NULL)
  {
    fullPath = string ( tracesHome );
  }

  // Step 3: Test existance
  do
  {
    newTraceName = auxPath + pathSep;
    for( vector< string >::const_iterator it = traceFilterID.begin(); it != traceFilterID.end(); ++it )
    {
      stringstream auxStrStrNumber;
      auxStrStrNumber << toolNumberedSuffix[ *it ];
      newTraceName = newTraceName +
                     string(".") +
                     toolExtension[ *it ] +
                     auxStrStrNumber.str();
    }

    newTraceName += string( ".prv" );
    newTraceNameGzipped = newTraceName + string( ".prv.gz" );
  }
#ifndef WIN32
  while (( stat( newTraceName.c_str(), &file_info ) == 0 ) ||
         ( stat( newTraceNameGzipped.c_str(), &file_info ) == 0 ));
#else
  while (( _stat64( newTraceName.c_str(), &file_info ) == 0 ) ||
         ( _stat64( newTraceNameGzipped.c_str(), &file_info ) == 0 ));
#endif

cout << "newTraceName: " << newTraceName << endl;

*/
  return newTraceName;
}
