#ifndef TRACEBODYIO_H_INCLUDED
#define TRACEBODYIO_H_INCLUDED

#include <iostream>
#include "memoryblocks.h"


class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO() = 0;

    static void read( iostream& whichStream, MemoryBlocks& whichMemory );
  protected:

  private:

};


#endif // TRACEBODYIO_H_INCLUDED
