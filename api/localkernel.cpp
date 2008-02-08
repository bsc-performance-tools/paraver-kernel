#include "localkernel.h"
#include "kwindow.h"
#include "khistogram.h"

LocalKernel::LocalKernel()
{}

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

