/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#ifndef TRACEEDITSTATES_H_INCLUDED
#define TRACEEDITSTATES_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"

class TraceEditSequence;
class TraceOptions;
class Window;

class TraceEditState
{
public:
  TraceEditState( TraceEditSequence *whichSequence ) : mySequence( whichSequence )
  {}

  ~TraceEditState()
  {}

  virtual void init() = 0;

protected:
  TraceEditSequence *mySequence;

private:

};

/****************************************************************************
 ********                  TestState                                 ********
 ****************************************************************************/
class TestState: public TraceEditState
{
  public:
    TestState( TraceEditSequence *whichSequence ) : TraceEditState( whichSequence )
    {}

    ~TestState()
    {}

    virtual void init();

    int getData() const;
    void setData( int whichData );

  protected:

  private:
    int myData;
};


/****************************************************************************
 ********                  TraceOptionsState                         ********
 ****************************************************************************/
class TraceOptionsState: public TraceEditState
{
  public:
    TraceOptionsState( TraceEditSequence *whichSequence ) : TraceEditState( whichSequence )
    {
      myData = NULL;
    }

    ~TraceOptionsState();

    virtual void init();

    TraceOptions *getData() const;
    void setData( TraceOptions *whichData );

  protected:

  private:
    TraceOptions *myData;
};


/****************************************************************************
 ********                  CSVWindowState                            ********
 ****************************************************************************/
class CSVWindowState: public TraceEditState
{
  public:
    CSVWindowState( TraceEditSequence *whichSequence ) : TraceEditState( whichSequence )
    {
      myData = NULL;
    }

    ~CSVWindowState();

    virtual void init();

    Window *getData() const;
    void setData( Window *whichData );

  protected:

  private:
    Window *myData;
};


/****************************************************************************
 ********                  CSVFileNameState                          ********
 ****************************************************************************/
class CSVFileNameState: public TraceEditState
{
  public:
    CSVFileNameState( TraceEditSequence *whichSequence ) : TraceEditState( whichSequence )
    {}

    ~CSVFileNameState();

    virtual void init();

    std::string getData() const;
    void setData( std::string whichData );

  protected:

  private:
    std::string myData;
};

#endif // TRACEEDITSTATES_H_INCLUDED
