#include <time.h>
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

void LocalKernel::init()
{
  srand( time( NULL ) );

  createFilter();
  createSemantic();
  createStatistic();
}

LocalKernel::LocalKernel( bool ( *messageFunction )( string ) ) :
    myMessageFunction( messageFunction )
{}

LocalKernel::~LocalKernel()
{}

Trace *LocalKernel::newTrace( const string& whichFile, ProgressController *progress ) const
{
  if ( progress == NULL )
    return new KTrace( whichFile, NULL );

  return new KTrace( whichFile, ( KProgressController * ) progress->getConcrete() );
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

  if ( traceFile.substr( 0, traceFile.length() - 6 ) == "prv.gz" )
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
