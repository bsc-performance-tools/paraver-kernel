#ifndef TRACEBODYIO_V1_H_INCLUDED
#define TRACEBODYIO_V1_H_INCLUDED

#ifdef WIN32
  #include <hash_set>
#else
  #include <ext/hash_set>
#endif
#include "tracebodyio.h"
#include "tracestream.h"

using namespace std;
#ifdef WIN32
  using namespace stdext;
#else
  using namespace __gnu_cxx;
#endif

// Paraver trace old format file
class TraceBodyIO_v1 : public TraceBodyIO
{
  public:
    static const UINT8 StateRecord = '1';
    static const UINT8 EventRecord = '2';
    static const UINT8 CommRecord = '3';
    static const UINT8 GlobalCommRecord = '4';

    static void read( TraceStream *file, MemoryBlocks& records,
                      hash_set<TEventType>& events );
    static void write( fstream& whichStream,
                       const KTrace& whichTrace,
                       const MemoryTrace::iterator *record );
    static void writeEvents( fstream& whichStream,
                             const KTrace& whichTrace,
                             const vector<MemoryTrace::iterator *>& recordList );
  protected:

  private:
    static void readState( const string& line, MemoryBlocks& records );
    static void readEvent( const string& line, MemoryBlocks& records,
                           hash_set<TEventType>& events );
    static void readComm( const string& line, MemoryBlocks& records );
    static void readGlobalComm( const string& line, MemoryBlocks& records );
    static bool readCommon( istringstream& line,
                            TCPUOrder& CPU,
                            TApplOrder& appl,
                            TTaskOrder& task,
                            TThreadOrder& thread,
                            TRecordTime& time );

    static bool writeState( string& line,
                            const KTrace& whichTrace,
                            const MemoryTrace::iterator *record );
    static bool writeEvent( string& line,
                            const KTrace& whichTrace,
                            const MemoryTrace::iterator *record,
                            bool needCommons = true );
    static bool writeComm( string& line,
                           const KTrace& whichTrace,
                           const MemoryTrace::iterator *record );
    static bool writeGlobalComm( string& line,
                                 const KTrace& whichTrace,
                                 const MemoryTrace::iterator *record );
    static void writeCommon( ostringstream& line,
                             const KTrace& whichTrace,
                             const MemoryTrace::iterator *record );

};


#endif // TRACEBODYIO_V1_H_INCLUDED
