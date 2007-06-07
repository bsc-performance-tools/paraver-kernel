#ifndef TRACEBODYIO_H_INCLUDED
#define TRACEBODYIO_H_INCLUDED

#include <fstream>
#include "memoryblocks.h"

using namespace std;

class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO() = 0;

    static void read( fstream& whichStream, MemoryBlocks& whichMemory );
  protected:

  private:

};


#endif // TRACEBODYIO_H_INCLUDED
