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

#ifndef GNUPLOTOUTPUT_H_INCLUDED
#define GNUPLOTOUTPUT_H_INCLUDED

#include "output.h"

class GNUPlotOutput: public Output
{
  public:
    GNUPlotOutput() {}
    ~GNUPlotOutput() {}

    virtual void dumpWindow( Window *whichWindow,
                             std::string& strOutputFile,
                             ProgressController *progress = NULL );
    virtual void dumpHistogram( Histogram *whichHisto,
                                std::string& strOutputFile,
                                bool onlySelectedPlane = false,
                                bool hideEmptyColumns = false,
                                bool withLabels = true,
                                bool withPreferencesPrecision = true,
                                bool recalcHisto = true,
                                ProgressController *progress = NULL );

    virtual bool getMultipleFiles() const;
    virtual void setMultipleFiles( bool newValue );

  protected:

  private:

};


#endif // GNUPLOTOUTPUT_H_INCLUDED
