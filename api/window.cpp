#include "kwindow.h"

Window *Window::create()
{
  // Hay que cambiarlo por un WindowProxy
  return new KWindow;
}
