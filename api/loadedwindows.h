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


#pragma once


#include <map>
#include <vector>
#include "paraverkerneltypes.h"

class Timeline;
class Histogram;
class Trace;

typedef PRV_UINT32 TWindowID;

class LoadedWindows
{
  public:
    ~LoadedWindows();

    static LoadedWindows *getInstance();
    static bool validDataWindow( Timeline *dataWindow, Timeline *controlWindow );
    static bool validLevelDataWindow( Timeline *dataWindow, Timeline *controlWindow );
    static bool notInParents( Timeline *whichWindow, Timeline *inParents );

    TWindowID add( Timeline *whichWindow );
    TWindowID add( Histogram *whichHisto );
    void eraseWindow( TWindowID id );
    void eraseWindow( Timeline *whichWindow );
    void eraseHisto( TWindowID id );
    void eraseHisto( Histogram *whichHisto );
    Timeline *getWindow( TWindowID id ) const;
    Histogram *getHisto( TWindowID id ) const;
    bool emptyWindows() const;
    bool emptyHistograms() const;
    void getAll( std::vector<Timeline *>& onVector ) const;
    void getAll( std::vector<TWindowID>& onVector ) const;

    void getAll( std::vector<Histogram *>& onVector ) const;
    void getAll( Trace *whichTrace, std::vector< Timeline *>& onVector ) const;
    void getDerivedCompatible( Trace *whichTrace, std::vector< Timeline *>& onVector ) const;
    void getDerivedCompatible( Trace *whichTrace, std::vector<TWindowID>& onVector ) const;
    void getAll( Trace *whichTrace, std::vector< Histogram *>& onVector ) const;

    // Histogram windows selection related methods
    void getValidControlWindow( Timeline *dataWindow, Timeline *controlWindow, std::vector<TWindowID>& onVector ) const;
    void getValidDataWindow( Timeline *controlWindow, Timeline *extraWindow,
                             std::vector<TWindowID>& onVector ) const;
  protected:

  private:
    LoadedWindows();

    static LoadedWindows *instance;

    std::map<TWindowID, Timeline *> windows;
    std::map<TWindowID, Histogram *> histograms;
    TWindowID currentID;
    TWindowID currentHistoID;

};



