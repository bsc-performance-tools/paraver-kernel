#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class LocalKernel: public KernelConnection
{
  public:
    static void init();

    LocalKernel();
    virtual ~LocalKernel();

    virtual Trace *newTrace( const string& whichFile, ProgressController *progress ) const;
    virtual string getPCFFileLocation( const string& traceFile ) const;
    virtual string getROWFileLocation( const string& traceFile ) const;
    virtual Window *newSingleWindow() const;
    virtual Window *newSingleWindow( Trace *whichTrace ) const;
    virtual Window *newDerivedWindow() const;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const;
    virtual Histogram *newHistogram() const;
//    virtual RecordList *newRecordList() const;
    virtual ProgressController *newProgressController() const;
    virtual Filter *newFilter( Filter *concreteFilter ) const;

    virtual void getAllStatistics( vector<string>& onVector ) const;
    virtual void getAllFilterFunctions( vector<string>& onVector ) const;
    virtual void getAllSemanticFunctions( TSemanticGroup whichGroup,
                                          vector<string>& onVector ) const;


  protected:

  private:

};


#endif // LOCALKERNEL_H_INCLUDED
