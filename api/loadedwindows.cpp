#include "loadedwindows.h"
#include "window.h"

LoadedWindows *LoadedWindows::instance = NULL;

LoadedWindows::LoadedWindows()
{
  currentID = 0;
}

LoadedWindows *LoadedWindows::getInstance()
{
  if ( LoadedWindows::instance == NULL )
    LoadedWindows::instance = new LoadedWindows();
  return LoadedWindows::instance;
}

LoadedWindows::~LoadedWindows()
{
  for ( map<TWindowID, Window *>::iterator it = windows.begin();
        it != windows.end(); it++ )
    delete ( *it ).second;
}

TWindowID LoadedWindows::add( Window *whichWindow )
{
  currentID++;
  windows[ currentID ] = whichWindow;

  return currentID;
}

// Histogram windows selection related methods
void LoadedWindows::getValidControlWindow( Window *dataWindow,
    vector<Window *>& onVector ) const
{
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); it++ )
  {
    if ( ( *it ).second->getChild() == NULL &&
         validDataWindow( dataWindow, ( *it ).second ) )
      onVector.push_back( ( *it ).second );
  }
}

void LoadedWindows::getValidDatalWindow( Window *controlWindow,
    Window *extraWindow,
    vector<Window *>& onVector ) const
{
  Window *cWin = ( controlWindow > extraWindow ) ? controlWindow : extraWindow;
  for ( map<TWindowID, Window *>::const_iterator it = windows.begin();
        it != windows.end(); it++ )
  {
    if ( ( *it ).second->getChild() == NULL &&
         validDataWindow( ( *it ).second, cWin ) )
      onVector.push_back( ( *it ).second );
  }
}

bool LoadedWindows::validDataWindow( Window *dataWindow, Window *controlWindow ) const
{
  if ( dataWindow->getTrace() == controlWindow->getTrace() )
  {
    return validLevelDataWindow( dataWindow, controlWindow );
  }
  else
  {
    // The traces have the same resource and application structure???
  }

  return false;
}

bool LoadedWindows::validLevelDataWindow( Window *dataWindow, Window *controlWindow ) const
{
  if ( dataWindow->getLevel() == controlWindow->getLevel() )
    return true;
  else
  {
    if ( dataWindow->getLevel() >= WORKLOAD && dataWindow->getLevel() <= THREAD
         &&
         controlWindow->getLevel() >= WORKLOAD && controlWindow->getLevel() <= THREAD )
    {
      if ( dataWindow->getLevel() < controlWindow->getLevel() )
        return true;
    }
    else if ( dataWindow->getLevel() >= SYSTEM && dataWindow->getLevel() <= CPU
              &&
              controlWindow->getLevel() >= SYSTEM && controlWindow->getLevel() <= CPU )
    {
      if ( dataWindow->getLevel() < controlWindow->getLevel() )
        return true;
    }
  }
  return false;
}
