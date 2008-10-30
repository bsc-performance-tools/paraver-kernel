#ifndef KERNELCONNECTION_H_INCLUDED
#define KERNELCONNECTION_H_INCLUDED

#include <string>
#include <vector>

class Window;
class Histogram;
class Trace;
class RecordList;
class ProgressController;

using namespace std;

class KernelConnection
{
  public:
    virtual ~KernelConnection() {}

    virtual Trace *newTrace( const string& whichFile, ProgressController *progress ) const = 0;
    virtual Window *newSingleWindow() const = 0;
    virtual Window *newSingleWindow( Trace *whichTrace ) const = 0;
    virtual Window *newDerivedWindow() const = 0;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const = 0;
    virtual Histogram *newHistogram() const = 0;
//    virtual RecordList *newRecordList() const = 0;
    virtual ProgressController *newProgressController() const = 0;

    virtual void getAllStatistics( vector<string>& onVector ) const = 0;

  protected:

  private:

};


#endif // KERNELCONNECTION_H_INCLUDED
