#ifndef KERNELCONNECTION_H_INCLUDED
#define KERNELCONNECTION_H_INCLUDED

class Window;
class Histogram;
class Trace;

class KernelConnection
{
  public:
    virtual ~KernelConnection() {}

    virtual Window *newSingleWindow( Trace *whichTrace ) const = 0;
    virtual Window *newDerivedWindow( Window *window1, Window * window2 ) const = 0;
    virtual Histogram *newHistogram() const = 0;

  protected:

  private:

};


#endif // KERNELCONNECTION_H_INCLUDED
