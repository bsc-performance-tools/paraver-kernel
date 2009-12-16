#ifndef LOCALKERNEL_H_INCLUDED
#define LOCALKERNEL_H_INCLUDED

#include "kernelconnection.h"

class KWindow;

class LocalKernel: public KernelConnection
{
  public:
    static void init();

    LocalKernel( bool (*messageFunction)(string) );
    virtual ~LocalKernel();

    virtual Trace *newTrace( const string& whichFile, bool noLoad, ProgressController *progress = NULL ) const;
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

    virtual bool userMessage( const string& message ) const;
  protected:

  private:
    bool (*myMessageFunction)(string);
};


#endif // LOCALKERNEL_H_INCLUDED
