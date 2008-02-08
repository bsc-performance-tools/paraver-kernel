#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class LocalKernel: public KernelConnection
{
  public:
    LocalKernel();
    virtual ~LocalKernel();

    virtual Window *newKSingleWindow( Trace *whichTrace ) const;
    virtual Window *newKDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newKHistogram() const;

  protected:

  private:

};


#endif // LOCALKERNEL_H_INCLUDED
