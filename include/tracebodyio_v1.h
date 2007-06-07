#ifndef TRACEBODYIO_V1_H_INCLUDED
#define TRACEBODYIO_V1_H_INCLUDED

#include "tracebodyio.h"

// Paraver trace old format file
class TraceBodyIO_v1 : public TraceBodyIO
{
  public:
    static void read( iostream& whichStream, MemoryBlocks& whichMemory );

  protected:

  private:

};


#endif // TRACEBODYIO_V1_H_INCLUDED
