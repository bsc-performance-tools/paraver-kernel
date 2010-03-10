#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include <string>

using std::string;

class Window;
class Histogram;

class TextOutput
{
  public:
    TextOutput();
    ~TextOutput();

    void dumpWindow( Window *whichWindow, string& strOutputFile );
    void dumpHistogram( Histogram *whichHisto, string& strOutputFile );

    bool getMultipleFiles() const;
    void setMultipleFiles( bool newValue );

  protected:

  private:
    bool multipleFiles;
};

#endif // TEXTOUTPUT_H_INCLUDED
