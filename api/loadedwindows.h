#ifndef LOADEDWINDOWS_H_INCLUDED
#define LOADEDWINDOWS_H_INCLUDED

#include <map>
#include "paraverkerneltypes.h"

using namespace std;

class Window;

typedef UINT32 TWindowID;

class LoadedWindows
{
  public:
    ~LoadedWindows();

    static LoadedWindows *getInstance();

    TWindowID add( Window *whichWindow );
    Window *getWindow( TWindowID id ) const;
    void getAll( vector<Window *>& onVector ) const;

    // Histogram windows selection related methods
    void getValidControlWindow( Window *dataWindow, vector<TWindowID>& onVector ) const;
    void getValidDataWindow( Window *controlWindow, Window *extraWindow,
                             vector<TWindowID>& onVector ) const;
  protected:

  private:
    LoadedWindows();

    static LoadedWindows *instance;

    map<TWindowID, Window *> windows;
    TWindowID currentID;

    bool validDataWindow( Window *dataWindow, Window *controlWindow ) const;
    bool validLevelDataWindow( Window *dataWindow, Window *controlWindow ) const;
};


#endif // LOADEDWINDOWS_H_INCLUDED
