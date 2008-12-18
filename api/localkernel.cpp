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
  createFilter();
  createSemantic();
  createStatistic();
}

LocalKernel::LocalKernel()
{}

LocalKernel::~LocalKernel()
{}

Trace *LocalKernel::newTrace( const string& whichFile, ProgressController *progress ) const
{
  return new KTrace( whichFile, ( KProgressController * ) progress->getConcrete() );
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
  FilterProxy *tmpFilter = new FilterProxy();
  tmpFilter->myFilter = concreteFilter;
  return (Filter *) tmpFilter;
}

void LocalKernel::getAllStatistics( vector<string>& onVector ) const
{
  FunctionManagement<HistogramStatistic>::getInstance()->getAll( onVector );
}
