#ifndef TRACEBODYIO_H_INCLUDED
#define TRACEBODYIO_H_INCLUDED

#include <fstream>
#include "memoryblocks.h"
#include "trace.h"


using namespace std;

class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO() = 0;

    static void read( fstream& whichStream, MemoryBlocks& whichMemory );
    static void write( fstream& whichStream,
                       const Trace& whichTrace,
                       const MemoryTrace::iterator& record );
  protected:

  private:

};


#endif // TRACEBODYIO_H_INCLUDED
