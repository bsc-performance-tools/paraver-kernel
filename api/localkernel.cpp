#include "localkernel.h"
#include "kwindow.h"
#include "khistogram.h"
#include "semanticmanagement.h"
#include "filtermanagement.h"
#include "statisticmanagement.h"
#include "krecordlist.h"

LocalKernel::LocalKernel()
{
  createFilter();
  createSemantic();
  createStatistic();
}

LocalKernel::~LocalKernel()
{}

Window *LocalKernel::newKSingleWindow( Trace *whichTrace ) const
{
  return new KSingleWindow( whichTrace );
}

Window *LocalKernel::newKDerivedWindow( Window *window1, Window * window2 ) const
{
  return new KDerivedWindow( window1, window2 );
}

Histogram *LocalKernel::newKHistogram() const
{
  return new KHistogram();
}

RecordList *LocalKernel::newRecordList() const
{
  return new KRecordList();
}
