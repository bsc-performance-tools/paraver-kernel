#ifndef TRACEBODYIO_V2_H_INCLUDED
#define TRACEBODYIO_V2_H_INCLUDED

#include "tracebodyio.h"
#include "tracestream.h"

using namespace std;

// Paraver trace NEW format file
class TraceBodyIO_v2 : public TraceBodyIO
{
  public:
    static const UINT8 StateBeginRecord = '1';
    static const UINT8 StateEndRecord = '2';
    static const UINT8 EventRecord = '3';
    static const UINT8 CommRecord = 'd';
    static const UINT8 LogicalSendRecord = '4';
    static const UINT8 LogicalRecvRecord = '5';
    static const UINT8 PhysicalSendRecord = '6';
    static const UINT8 PhysicalRecvRecord = '7';
    static const UINT8 GlobalCommRecord = '8';
#ifdef BYTHREAD
    static const UINT8 RemoteLogicalSendRecord = 'W';
    static const UINT8 RemoteLogicalRecvRecord = 'X';
    static const UINT8 RemotePhysicalSendRecord = 'Y';
    static const UINT8 RemotePhysicalRecvRecord = 'Z';
#endif

    bool ordered() const;
    void read( TraceStream *file, MemoryBlocks& records,
               hash_set<TEventType>& events ) const;
    void write( fstream& whichStream,
                const KTrace& whichTrace,
                MemoryTrace::iterator *record ) const;
    void writeEvents( fstream& whichStream,
                      const KTrace& whichTrace,
                      vector<MemoryTrace::iterator *>& recordList ) const;
    void writeCommInfo( fstream& whichStream,
                        const KTrace& whichTrace ) const;
  protected:

  private:
    void readState( const string& line, MemoryBlocks& records ) const;
    void readEvent( const string& line, MemoryBlocks& records,
                    hash_set<TEventType>& events ) const;
    void readComm( const string& line, MemoryBlocks& records ) const;
    void readGlobalComm( const string& line, MemoryBlocks& records ) const;
    bool readCommon( istringstream& line,
                     TCPUOrder& CPU,
                     TThreadOrder& thread,
                     TRecordTime& time ) const;

    bool writeState( string& line,
                     const KTrace& whichTrace,
                     MemoryTrace::iterator *record ) const;
    bool writeEvent( string& line,
                     const KTrace& whichTrace,
                     MemoryTrace::iterator *record,
                     bool needCommons = true ) const;
    bool writeCommRecord( string& line,
                          const KTrace& whichTrace,
                          MemoryTrace::iterator *record ) const;
    bool writeGlobalComm( string& line,
                          const KTrace& whichTrace,
                          MemoryTrace::iterator *record ) const;
    void writeCommon( ostringstream& line,
                      const KTrace& whichTrace,
                      MemoryTrace::iterator *record ) const;

};


#endif // TRACEBODYIO_V2_H_INCLUDED
