#ifndef LOADEDWINDOWS_H_INCLUDED
#define LOADEDWINDOWS_H_INCLUDED

#include <map>
#include "paraverkerneltypes.h"

using namespace std;

class Window;
class Histogram;
class Trace;

typedef UINT32 TWindowID;

class LoadedWindows
{
  public:
    ~LoadedWindows();

    static LoadedWindows *getInstance();

    TWindowID add( Window *whichWindow );
    TWindowID add( Histogram *whichHisto );
    Window *getWindow( TWindowID id ) const;
    Histogram *getHisto( TWindowID id ) const;
    void getAll( vector<Window *>& onVector ) const;
    void getAll( vector<Histogram *>& onVector ) const;
    void getAll( Trace *whichTrace, vector< Window *>& onVector ) const;
    void getAll( Trace *whichTrace, vector< Histogram *>& onVector ) const;

    // Histogram windows selection related methods
    void getValidControlWindow( Window *dataWindow, Window *controlWindow, vector<TWindowID>& onVector ) const;
    void getValidDataWindow( Window *controlWindow, Window *extraWindow,
                             vector<TWindowID>& onVector ) const;
  protected:

  private:
    LoadedWindows();

    static LoadedWindows *instance;

    map<TWindowID, Window *> windows;
    map<TWindowID, Histogram *> histograms;
    TWindowID currentID;
    TWindowID currentHistoID;

    bool validDataWindow( Window *dataWindow, Window *controlWindow ) const;
    bool validLevelDataWindow( Window *dataWindow, Window *controlWindow ) const;
    bool notInParents( Window *whichWindow, Window *inParents ) const;
};


#endif // LOADEDWINDOWS_H_INCLUDED
