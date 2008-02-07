#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class LocalKernel: public KernelConnection
{
  public:
    LocalKernel();
    virtual ~LocalKernel();

    virtual Window *newSingleWindow( Trace *whichTrace ) const;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newHistogram() const;

  protected:

  private:

};


#endif // LOCALKERNEL_H_INCLUDED
