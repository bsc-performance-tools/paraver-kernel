#ifndef TRACEBODYIO_V1_H_INCLUDED
#define TRACEBODYIO_V1_H_INCLUDED

#include "tracebodyio.h"

using namespace std;

// Paraver trace old format file
class TraceBodyIO_v1 : public TraceBodyIO
{
  public:
    static const UINT8 StateRecord = '1';
    static const UINT8 EventRecord = '2';
    static const UINT8 CommRecord = '3';
    static const UINT8 GlobalCommRecord = '4';

    static void read( fstream& file, MemoryBlocks& records );

  protected:

  private:
    static void readState( const string& line, MemoryBlocks& records );
    static void readEvent( const string& line, MemoryBlocks& records );
    static void readComm( const string& line, MemoryBlocks& records );
    static void readGlobalComm( const string& line, MemoryBlocks& records );
    static bool readCommon( istringstream& line,
                            TCPUOrder& CPU,
                            TApplOrder& appl,
                            TTaskOrder& task,
                            TThreadOrder& thread,
                            TRecordTime& time );
};


#endif // TRACEBODYIO_V1_H_INCLUDED
