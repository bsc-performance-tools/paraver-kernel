/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef TRACESTREAM_H_INCLUDED
#define TRACESTREAM_H_INCLUDED

#include <fstream>
#include <string>
#include <zlib.h>
#include "paraverkerneltypes.h"

class TraceStream
{
  public:
    TraceStream()
    {}

    TraceStream( const std::string& filename )
    {}

    virtual ~TraceStream()
    {}

    virtual void open( const std::string& filename ) = 0;
    virtual void close() = 0;
    virtual void getline( std::string& strLine ) = 0;
    virtual bool read( char *const buffer, const std::streamsize size ) = 0;
    virtual bool eof() = 0;
    virtual void seekbegin() = 0;
    virtual void seekend() = 0;
    virtual void seekg( std::streampos pos ) = 0;
    virtual std::streampos tellg() = 0;
    virtual bool canseekend() = 0;
    virtual bool good() const = 0;
    virtual void clear() = 0;
    virtual int peek() = 0;

    static TraceStream *openFile( const std::string& filename );

    static TTraceSize getTraceFileSize( const std::string& filename );

    static const double GZIP_COMPRESSION_RATIO;
};


class NotCompressed: public TraceStream
{
  public:
    NotCompressed()
    {}

    NotCompressed( const std::string& filename );

    virtual ~NotCompressed()
    {}

    virtual void open( const std::string& filename );
    virtual void close();
    virtual void getline( std::string& strLine );
    virtual bool read( char *const buffer, const std::streamsize size );
    virtual bool eof();
    virtual void seekbegin();
    virtual void seekend();
    virtual void seekg( std::streampos pos );
    virtual std::streampos tellg();
    virtual bool canseekend();
    virtual bool good() const;
    virtual void clear();
    virtual int peek();

    static TTraceSize getTraceFileSize( const std::string& filename );

private:
    std::ifstream file;

};


class Compressed: public TraceStream
{
  public:
    Compressed()
    {}

    Compressed( const std::string& filename );

    virtual ~Compressed()
    {}

    virtual void open( const std::string& filename );
    virtual void close();
    virtual void getline( std::string& strLine );
    virtual bool read( char *const buffer, const std::streamsize size );
    virtual bool eof();
    virtual void seekbegin();
    virtual void seekend();
    virtual void seekg( std::streampos pos );
    virtual std::streampos tellg();
    virtual bool canseekend();
    virtual bool good() const;
    virtual void clear();
    virtual int peek();

    static TTraceSize getTraceFileSize( const std::string& filename );

  private:
    static const PRV_UINT32 LINESIZE = 1000 * 1024;
    gzFile file;
    char tmpLine[ LINESIZE ];

};

#endif // TRACESTREAM_H_INCLUDED
