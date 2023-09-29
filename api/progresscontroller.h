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


#include "paraverkerneltypes.h"

class KernelConnection;

class ProgressController
{
  public:
    typedef void(*ProgressHandler)( void*, ProgressController* );

    static ProgressController *create( KernelConnection *whichKernel );

    ProgressController() {};
    virtual ~ProgressController() {};

    virtual void setHandler( void* whichProgressDialog, ProgressHandler whichHandler ) = 0;
    virtual void callHandler( ProgressController *not_used ) = 0;
    virtual double getEndLimit() const = 0;
    virtual void setEndLimit( double limit ) = 0;
    virtual double getCurrentProgress() const = 0;
    virtual void setCurrentProgress( double progress ) = 0;
    virtual void setPartner( ProgressController* partner ) = 0;
    virtual void setStop( bool value ) = 0;
    virtual bool getStop() const = 0;
    virtual void setMessage( std::string whichMessage ) = 0;
    virtual std::string getMessage() const = 0;
    virtual bool getMessageChanged() const = 0;
    virtual void clearMessageChanged() = 0;
    virtual void setLastUpdate( double whichLastUpdate ) = 0;
    virtual double getLastUpdate() const = 0;

    virtual ProgressController *getConcrete() const
    {
      return nullptr;
    }
};


class ProgressControllerProxy:public ProgressController
{
  public:
    ~ProgressControllerProxy();

    void setHandler( void* whichProgressDialog, ProgressHandler whichHandler ) override;
    void callHandler( ProgressController *not_used ) override;
    double getEndLimit() const override;
    void setEndLimit( double limit ) override;
    double getCurrentProgress() const override;
    void setCurrentProgress( double progress ) override;
    void setPartner( ProgressController* partner ) override;
    void setStop( bool value ) override;
    bool getStop() const override;
    void setMessage( std::string whichMessage ) override;
    std::string getMessage() const override;
    void clearMessageChanged() override;
    bool getMessageChanged() const override;
    void setLastUpdate( double whichLastUpdate ) override;
    double getLastUpdate() const override;

    ProgressController *getConcrete() const override;

  private:
    ProgressControllerProxy( KernelConnection *whichKernel );

    KernelConnection *myKernel;
    ProgressController *myPartner;

    ProgressHandler handler;
    void *progressDialog;
    double endLimit;
    double currentProgress;
    std::string message;
    bool messageChanged;

    double lastUpdate;

    friend ProgressController *ProgressController::create( KernelConnection * );
};


