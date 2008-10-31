#ifndef TRACESTREAM_H_INCLUDED
#define TRACESTREAM_H_INCLUDED

#include <fstream>
#include <string>
#include <zlib.h>
#include "paraverkerneltypes.h"

using namespace std;

class TraceStream
{
  public:
    TraceStream()
    {}

    TraceStream( const string& filename )
    {}

    virtual ~TraceStream()
    {}

    virtual void open( const string& filename ) = 0;
    virtual void close() = 0;
    virtual void getline( string& strLine ) = 0;
    virtual bool eof() = 0;
    virtual void seekbegin() = 0;
    virtual void seekend() = 0;
    virtual double tellg() = 0;
    virtual bool canseekend() = 0;

    static TraceStream *openFile( const string& filename );
};


class NotCompressed: public TraceStream
{
  public:
    NotCompressed()
    {}

    NotCompressed( const string& filename );

    virtual ~NotCompressed()
    {}

    virtual void open( const string& filename );
    virtual void close();
    virtual void getline( string& strLine );
    virtual bool eof();
    virtual void seekbegin();
    virtual void seekend();
    virtual double tellg();
    virtual bool canseekend();

  private:
    fstream file;

};


class Compressed: public TraceStream
{
  public:
    Compressed()
    {}

    Compressed( const string& filename );

    virtual ~Compressed()
    {}

    virtual void open( const string& filename );
    virtual void close();
    virtual void getline( string& strLine );
    virtual bool eof();
    virtual void seekbegin();
    virtual void seekend();
    virtual double tellg();
    virtual bool canseekend();

  private:
    static const UINT32 LINESIZE = 100 * 1024;
    gzFile file;
    char tmpLine[ LINESIZE ];

};

#endif // TRACESTREAM_H_INCLUDED
