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

#ifndef KPROGRESSCONTROLLER_H_INCLUDED
#define KPROGRESSCONTROLLER_H_INCLUDED

#include "progresscontroller.h"

class KProgressController: public ProgressController
{
  public:
    KProgressController();
    ~KProgressController();

    void setHandler( ProgressHandler whichHandler );
    void callHandler( ProgressController *not_used );
    double getEndLimit() const;
    void setEndLimit( double limit );
    double getCurrentProgress() const;
    void setCurrentProgress( double progress );
    void setPartner( ProgressController* partner );
    void setStop( bool value );
    bool getStop() const;
    void setMessage( std::string whichMessage );
    std::string getMessage() const;
    void clearMessageChanged();
    bool getMessageChanged() const;

  private:
    ProgressController *myPartner;

    ProgressHandler handler;
    double endLimit;
    double currentProgress;
    bool stop;
};


#endif // KPROGRESSCONTROLLER_H_INCLUDED
