#include "localkernel.h"
#include "kwindow.h"
#include "khistogram.h"
#include "semanticmanagement.h"
#include "filtermanagement.h"
#include "statisticmanagement.h"
#include "krecordlist.h"

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

Window *LocalKernel::newSingleWindow() const
{
  return new KSingleWindow();
}

Window *LocalKernel::newSingleWindow( Trace *whichTrace ) const
{
  return new KSingleWindow( whichTrace );
}

Window *LocalKernel::newDerivedWindow( Window *window1, Window * window2 ) const
{
  return new KDerivedWindow( window1, window2 );
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
