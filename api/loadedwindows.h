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

    // Histogram windows selection related methods
    void getValidControlWindow( Window *dataWindow, vector<Window *>& onVector ) const;
    void getValidDatalWindow( Window *controlWindow, Window *extraWindow,
                              vector<Window *>& onVector ) const;
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
