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


#pragma once


#include "semanticderived.h"

class DerivedAdd: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedAdd()
    {
      setDefaultParam();
    }

    ~DerivedAdd()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedAdd::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedAdd( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;
};


class DerivedProduct: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedProduct()
    {
      setDefaultParam();
    }

    ~DerivedProduct()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedProduct::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedProduct( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class DerivedSubstract: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedSubstract()
    {
      setDefaultParam();
    }

    ~DerivedSubstract()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedSubstract::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedSubstract( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class DerivedDivide: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedDivide()
    {
      setDefaultParam();
    }

    ~DerivedDivide()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedDivide::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedDivide( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class DerivedMaximum: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedMaximum()
    {
      setDefaultParam();
    }

    ~DerivedMaximum()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedMaximum::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedMaximum( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class DerivedMinimum: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedMinimum()
    {
      setDefaultParam();
    }

    ~DerivedMinimum()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedMinimum::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedMinimum( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class DerivedDifferent: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedDifferent()
    {
      setDefaultParam();
    }

    ~DerivedDifferent()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedDifferent::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedDifferent( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
    static std::string name;

};


class ControlDerivedClearBy: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ControlDerivedClearBy()
    {
      setDefaultParam();
    }

    ~ControlDerivedClearBy()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ControlDerivedClearBy::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ControlDerivedClearBy( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static const bool controlDerived = false;
    static std::string name;

    std::vector<TSemanticValue> lastControlValue;
    std::vector<TRecordTime> lastDataBeginTime;

};


class ControlDerivedMaximum: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ControlDerivedMaximum()
    {
      setDefaultParam();
    }

    ~ControlDerivedMaximum()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ControlDerivedMaximum::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ControlDerivedMaximum( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = true;
    static std::string name;

};


class ControlDerivedAdd: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ControlDerivedAdd()
    {
      setDefaultParam();
    }

    ~ControlDerivedAdd()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ControlDerivedAdd::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ControlDerivedAdd( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = true;
    static std::string name;

};


class ControlDerivedEnumerate: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ControlDerivedEnumerate()
    {
      setDefaultParam();
    }

    ~ControlDerivedEnumerate()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ControlDerivedEnumerate::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ControlDerivedEnumerate( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static const bool controlDerived = false;
    static std::string name;

    std::vector<TSemanticValue> prevControlValue;
    std::vector<TRecordTime> prevDataTime;
    std::vector<TSemanticValue> myEnumerate;

};


class ControlDerivedAverage: public SemanticDerived
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ControlDerivedAverage()
    {
      setDefaultParam();
    }

    ~ControlDerivedAverage()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived() override
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;
    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ControlDerivedAverage::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ControlDerivedAverage( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = true;
    static std::string name;

    std::vector<TSemanticValue> totalValue;
    std::vector<TRecordTime> totalTime;

};


