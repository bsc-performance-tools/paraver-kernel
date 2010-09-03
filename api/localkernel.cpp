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

#include "ktraceoptions.h"
#include "ktracecutter.h"
#include "ktracefilter.h"
#include "ktracesoftwarecounters.h"
#include "tracestream.h"
#include <string.h>

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
  // FILTERS
  trace_names_table_last = 0;
}

LocalKernel::~LocalKernel()
{}

bool LocalKernel::checkTraceSize( const string& filename, TTraceSize maxSize ) const
{
  TTraceSize size = TraceStream::getTraceFileSize( filename );

  if( size <= maxSize )
    return true;

  return false;
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
                                          TraceOptions *options ) const
{
//  cout << "LocalKernel::newTraceCutter: tracein " << trace_in  << endl;
//  cout << "LocalKernel::newTraceCutter: traceout" << trace_out << endl;

  //TraceOptionsProxy *myOptions = new TraceOptionsProxy( this );
  //myOptions->myTraceOptions = options;
  /*TraceCutterProxy *myCutter = new TraceCutterProxy( this,
                                                     trace_in,
                                                     trace_out,
                                                     options );
*/
  return new KTraceCutter( trace_in, trace_out, options );
//  return ( TraceCutter *) myCutter;

}

TraceFilter *LocalKernel::newTraceFilter( char *trace_in,
                                          char *trace_out,
                                          TraceOptions *options ) const
{
//  TraceOptionsProxy *myOptions = new TraceOptionsProxy( this );
//  myOptions->myTraceOptions = options;
  return new KTraceFilter( trace_in, trace_out, options );
}


TraceSoftwareCounters *LocalKernel::newTraceSoftwareCounters( char *trace_in,
                                                              char *trace_out,
                                                              TraceOptions *options ) const
{
  return new KTraceSoftwareCounters( trace_in, trace_out, options );
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
    return false;

  return myMessageFunction( message );
}

char *LocalKernel::composeName( char *name, char *newExtension )
{
  int zip_filter;
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
  char *pcfIn  = composeName( name, "pcf" );
  char *pcfOut = composeName( traceToLoad, "pcf" );

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
  char *rowIn = composeName( name, "row" );
  char *rowOut = composeName( traceToLoad, "row" );

  copyFile( rowIn, rowOut );

  free( rowIn );
  free( rowOut );
}

void LocalKernel::getNewTraceName( char *name, char *new_trace_name, int action )
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

  traceToLoad[0] = '\0';
  /* De forma provisional agafem var entorn */
 // if (( traces_home = getenv( "TRACES_HOME" )) != NULL)
  if (( traces_home = getenv( "PWD" )) != NULL)
    sprintf(traceToLoad,"%s",traces_home);

//cout << traceToLoad << endl;

  strcpy( path_name_backup, new_trace_name );

  for ( i = 0; i < trace_names_table_last; i++ )
    if ( !strcmp( name, trace_names_table[i].name ) ) break;

  if ( i == trace_names_table_last )
  {
    trace_names_table[trace_names_table_last].name = ( char * )strdup( name );
    trace_names_table[trace_names_table_last].num_chop = 0;
    trace_names_table[trace_names_table_last].num_sc = 0;
    trace_names_table[trace_names_table_last].num_filter = 0;
    trace_names_table_last++;
  }

  /* Generating the new trace name and preventing to erase old chops */
  do
  {
    switch ( action )
    {
      case INC_CHOP_COUNTER:
        trace_names_table[i].num_chop++;
        break;

      case INC_SC_COUNTER:
        trace_names_table[i].num_sc++;
        break;

      case INC_FILTER_COUNTER:
        trace_names_table[i].num_filter++;
        break;

      default:
        break;
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

    switch ( action )
    {
      case INC_CHOP_COUNTER:
        if ( strstr( new_trace_name, "chop" ) != NULL )
          sprintf( new_trace_name, "%s%d.prv", new_trace_name, trace_names_table[i].num_chop );
        else
          sprintf( new_trace_name, "%schop%d.prv", new_trace_name, trace_names_table[i].num_chop );

        break;

      case INC_SC_COUNTER:
        if ( strstr( new_trace_name, ".sc" ) != NULL )
          sprintf( new_trace_name, "%s%d.prv", new_trace_name, trace_names_table[i].num_sc );
        else
          sprintf( new_trace_name, "%ssc%d.prv", new_trace_name, trace_names_table[i].num_sc );

        break;


      case INC_FILTER_COUNTER:
        if ( strstr( new_trace_name, "filter" ) != NULL )
          sprintf( new_trace_name, "%s%d.prv", new_trace_name, trace_names_table[i].num_filter );
        else
          sprintf( new_trace_name, "%sfilter%d.prv", new_trace_name, trace_names_table[i].num_filter );

        break;

      default:
        break;
    }
  }
#ifndef WIN32
  while ( stat( new_trace_name, &file_info ) == 0 );
#else
  while ( _stat64( new_trace_name, &file_info ) == 0 );
#endif
}
