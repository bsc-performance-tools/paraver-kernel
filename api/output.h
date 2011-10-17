#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include <string>

class Window;
class Histogram;

using namespace std;

class Output
{
  public:
    enum TOutput
    {
      TEXT = 0,
      GNUPLOT
    };

    static Output *createOutput( TOutput whichOutput );

    Output() {}
    ~Output() {}

    virtual void dumpWindow( Window *whichWindow, string& strOutputFile ) = 0;
    virtual void dumpHistogram( Histogram *whichHisto,
                        string& strOutputFile,
                        bool onlySelectedPlane = false,
                        bool hideEmptyColumns = false ) = 0;

    virtual bool getMultipleFiles() const = 0;
    virtual void setMultipleFiles( bool newValue ) = 0;

  protected:

  private:

};


#endif // OUTPUT_H_INCLUDED
