#ifndef TRACEBODYIO_H_INCLUDED
#define TRACEBODYIO_H_INCLUDED

#ifdef WIN32
#include <hash_set>
#else
#include <ext/hash_set>
#endif
#include <fstream>
#include "memoryblocks.h"
#include "ktrace.h"
#include "tracestream.h"

using namespace std;
#ifdef WIN32
using namespace stdext;
#else
using namespace __gnu_cxx;
#endif

class TraceBodyIO
{
  public:
    TraceBodyIO();
    virtual ~TraceBodyIO();

    virtual bool ordered() const = 0;
    virtual void read( TraceStream *file, MemoryBlocks& records,
                       hash_set<TEventType>& events ) const = 0;
    virtual void write( fstream& whichStream,
                        const KTrace& whichTrace,
                        MemoryTrace::iterator *record ) const = 0;
    virtual void writeCommInfo( fstream& whichStream,
                                const KTrace& whichTrace ) const = 0;

    static TraceBodyIO *createTraceBody( TraceStream *file );
    static TraceBodyIO *createTraceBody();
  protected:

  private:

};


#endif // TRACEBODYIO_H_INCLUDED
