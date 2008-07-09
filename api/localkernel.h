#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class LocalKernel: public KernelConnection
{
  public:
    static void init();

    LocalKernel();
    virtual ~LocalKernel();

    virtual Trace *newTrace( const string& whichFile ) const;
    virtual Window *newSingleWindow() const;
    virtual Window *newSingleWindow( Trace *whichTrace ) const;
    virtual Window *newDerivedWindow() const;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newHistogram() const;
//    virtual RecordList *newRecordList() const;

    virtual void getAllStatistics( vector<string>& onVector ) const;

  protected:

  private:

};


#endif // LOCALKERNEL_H_INCLUDED
