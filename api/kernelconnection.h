#ifndef KERNELCONNECTION_H_INCLUDED
#define KERNELCONNECTION_H_INCLUDED

class Window;
class Histogram;
class Trace;
class RecordList;

class KernelConnection
{
  public:
    virtual ~KernelConnection() {}

    virtual Window *newKSingleWindow( Trace *whichTrace ) const = 0;
    virtual Window *newKDerivedWindow( Window *window1, Window * window2 ) const = 0;
    virtual Histogram *newKHistogram() const = 0;
    virtual RecordList *newRecordList() const = 0;
  protected:

  private:

};


#endif // KERNELCONNECTION_H_INCLUDED
