#include "localkernel.h"
#include "kwindow.h"
#include "khistogram.h"

LocalKernel::LocalKernel()
{}

LocalKernel::~LocalKernel()
{}

Window *LocalKernel::newSingleWindow( Trace *whichTrace ) const
{
  return new KSingleWindow( whichTrace );
}

Window *LocalKernel::newDerivedWindow( Window *window1, Window * window2 ) const
{
  return new KDerivedWindow( window1, window2 );
}

Histogram *LocalKernel::newHistogram() const
{
  return new KHistogram();
}

