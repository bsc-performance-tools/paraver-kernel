#ifndef GNUPLOTOUTPUT_H_INCLUDED
#define GNUPLOTOUTPUT_H_INCLUDED

#include "output.h"

class GNUPlotOutput: public Output
{
  public:
    GNUPlotOutput() {}
    ~GNUPlotOutput() {}

    virtual void dumpWindow( Window *whichWindow, string& strOutputFile );
    virtual void dumpHistogram( Histogram *whichHisto,
                        string& strOutputFile,
                        bool onlySelectedPlane = false,
                        bool hideEmptyColumns = false );

    virtual bool getMultipleFiles() const;
    virtual void setMultipleFiles( bool newValue );

  protected:

  private:

};


#endif // GNUPLOTOUTPUT_H_INCLUDED
