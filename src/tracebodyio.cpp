#include "tracebodyio_v1.h"
#include "tracebodyio_v2.h"

TraceBodyIO::TraceBodyIO()
{}

TraceBodyIO::~TraceBodyIO()
{}

TraceBodyIO *TraceBodyIO::createTraceBody( TraceStream *file )
{
  TraceBodyIO *ret;
  string firstLine;

  file->getline( firstLine );
  if( firstLine.compare( "new format" ) == 0 )
  {
    ret = new TraceBodyIO_v2();
  }
  else
  {
    ret = new TraceBodyIO_v1();
    file->seekbegin();
  }
  return ret;
}

TraceBodyIO *TraceBodyIO::createTraceBody()
{
  return new TraceBodyIO_v2();
}
