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


#ifndef SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
#define SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED

#include <stack>
#include <list>
#include <cmath>
#include "semanticcompose.h"

class ComposeAsIs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAsIs()
    {
      setDefaultParam();
    }

    ~ComposeAsIs()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeAsIs::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeAsIs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
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
    static std::string name;
};


class ComposeSign: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSign()
    {
      setDefaultParam();
    }

    ~ComposeSign()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeSign::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSign( *this );
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
    static std::string name;

};


class ComposeUnsign: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeUnsign()
    {
      setDefaultParam();
    }

    ~ComposeUnsign()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeUnsign::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeUnsign( *this );
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
    static std::string name;

};


class ComposeMod: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeMod()
    {
      setDefaultParam();
    }

    ~ComposeMod()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeMod::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeMod( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeModPlus1: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeModPlus1()
    {
      setDefaultParam();
    }

    ~ComposeModPlus1()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeModPlus1::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeModPlus1( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeDivide: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeDivide()
    {
      setDefaultParam();
    }

    ~ComposeDivide()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeDivide::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeDivide( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeProduct: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeProduct()
    {
      setDefaultParam();
    }

    ~ComposeProduct()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeProduct::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeProduct( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Factor";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeAdding: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeAdding()
    {
      setDefaultParam();
    }

    ~ComposeAdding()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeAdding::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeAdding( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSubstract: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeSubstract()
    {
      setDefaultParam();
    }

    ~ComposeSubstract()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeSubstract::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSubstract( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeComplement: public SemanticCompose
{
  public:
    typedef enum
    {
      MINUEND = 0,
      MAXPARAM
    } TParam;

    ComposeComplement()
    {
      setDefaultParam();
    }

    ~ComposeComplement()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeComplement::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeComplement( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == MINUEND )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Minuend";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};



class ComposeInverseDiv: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeInverseDiv()
    {
      setDefaultParam();
    }

    ~ComposeInverseDiv()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeInverseDiv::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeInverseDiv( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Factor";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSelectRange: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeSelectRange()
    {
      setDefaultParam();
    }

    ~ComposeSelectRange()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeSelectRange::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSelectRange( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeSelectRangeOpen: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeSelectRangeOpen()
    {
      setDefaultParam();
    }

    ~ComposeSelectRangeOpen()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeSelectRangeOpen::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSelectRangeOpen( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeIsInRange: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeIsInRange()
    {
      setDefaultParam();
    }

    ~ComposeIsInRange()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeIsInRange::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeIsInRange( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeIsInRangeOpen: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeIsInRangeOpen()
    {
      setDefaultParam();
    }

    ~ComposeIsInRangeOpen()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeIsInRangeOpen::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeIsInRangeOpen( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeIsEqual: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ComposeIsEqual()
    {
      setDefaultParam();
    }

    ~ComposeIsEqual()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeIsEqual::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeIsEqual( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeIsEqualSign: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ComposeIsEqualSign()
    {
      setDefaultParam();
    }

    ~ComposeIsEqualSign()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeIsEqualSign::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeIsEqualSign( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }

    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }

    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeFloor: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeFloor()
    {
      setDefaultParam();
    }

    ~ComposeFloor()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeFloor::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeFloor( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
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
    static std::string name;
};


class ComposeCeil: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeCeil()
    {
      setDefaultParam();
    }

    ~ComposeCeil()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeCeil::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeCeil( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
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
    static std::string name;
};


class ComposeRound: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeRound()
    {
      setDefaultParam();
    }

    ~ComposeRound()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeRound::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeRound( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
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
    static std::string name;
};


class ComposeAbs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAbs()
    {
      setDefaultParam();
    }

    ~ComposeAbs()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeAbs::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeAbs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
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
    static std::string name;
};


class ComposeStackedValue: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeStackedValue()
    {
      setDefaultParam();
    }

    ~ComposeStackedValue()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeStackedValue::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeStackedValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

    virtual std::vector<std::vector<TSemanticValue> > *getStack() override
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<std::vector<TSemanticValue> > myStack;
};


class ComposeInStackedValue: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUE = 0,
      MAXPARAM
    } TParam;

    ComposeInStackedValue()
    {
      setDefaultParam();
    }

    ~ComposeInStackedValue()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeInStackedValue::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeInStackedValue( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == VALUE )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Value";
    }

    virtual std::vector<std::vector<TSemanticValue> > *getStack() override
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<std::vector<TSemanticValue> > myStack;
};


class ComposeNestingLevel: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeNestingLevel()
    {
      setDefaultParam();
    }

    ~ComposeNestingLevel()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeNestingLevel::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeNestingLevel( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<TSemanticValue> myStack;
};

class ComposeLRUDepth: public SemanticCompose
{
  public:
    typedef enum
    {
      STACK_SIZE = 0,
      MAXPARAM
    } TParam;

    ComposeLRUDepth()
    {
      setDefaultParam();
    }

    ~ComposeLRUDepth()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeLRUDepth::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeLRUDepth( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }

    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == STACK_SIZE )
        tmp.push_back( 256 );

      return tmp;
    }

    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Stack size";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector< std::list< TSemanticValue > > LRUStack;
};


class ComposeEnumerate: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeEnumerate()
    {
      setDefaultParam();
    }

    ~ComposeEnumerate()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeEnumerate::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeEnumerate( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;
};



class ComposeAccumulate: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAccumulate()
    {
      setDefaultParam();
    }

    ~ComposeAccumulate()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeAccumulate::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeAccumulate( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;
};


class ComposeDelta: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeDelta()
    {
      setDefaultParam();
    }

    ~ComposeDelta()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override;

    virtual std::string getName() override
    {
      return ComposeDelta::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeDelta( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<TSemanticValue> semPrevValue;
};


class ComposeBurstTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeBurstTime()
    {
      setDefaultParam();
    }

    ~ComposeBurstTime()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeBurstTime::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeBurstTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};

class ComposeDivideByBurstTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeDivideByBurstTime()
    {
      setDefaultParam();
    }

    ~ComposeDivideByBurstTime()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeDivideByBurstTime::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeDivideByBurstTime( *this );
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
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};

class ComposeJoinBursts: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeJoinBursts()
    {
      setDefaultParam();
    }

    ~ComposeJoinBursts()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeJoinBursts::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeJoinBursts( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

};

class ComposeBeginTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeBeginTime()
    {
      setDefaultParam();
    }

    ~ComposeBeginTime()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeBeginTime::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeBeginTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};

class ComposeEndTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeEndTime()
    {
      setDefaultParam();
    }

    ~ComposeEndTime()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeEndTime::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeEndTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeTranslate: public SemanticCompose
{
  public:
    typedef enum
    {
      TRANSLATION_LIST = 0,
      MAXPARAM
    } TParam;

    ComposeTranslate()
    {
      setDefaultParam();
    }

    ~ComposeTranslate()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeTranslate::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeTranslate( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const override
    {
      return SAME_TYPE; // Some doubts
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Translation List";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSine: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSine()
    {
      setDefaultParam();
    }

    ~ComposeSine()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}


    virtual std::string getName() override
    {
      return ComposeSine::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSine( *this );
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
    static std::string name;
};


class ComposeCosine: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeCosine()
    {
      setDefaultParam();
    }

    ~ComposeCosine()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeCosine::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeCosine( *this );
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
    static std::string name;
};


class ComposeSqrtAbs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSqrtAbs()
    {
      setDefaultParam();
    }

    ~ComposeSqrtAbs()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeSqrtAbs::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeSqrtAbs( *this );
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
    static std::string name;
};


class ComposeArcTan: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeArcTan()
    {
      setDefaultParam();
    }

    ~ComposeArcTan()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeArcTan::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeArcTan( *this );
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
    static std::string name;
};




class ComposeLogN: public SemanticCompose
{
  public:
    typedef enum
    {
      BASE = 0,
      MAXVALUE
    } TParam;

    ComposeLogN()
    {
      setDefaultParam();
    }

    ~ComposeLogN()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXVALUE;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeLogN::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeLogN( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      else if ( whichParam == BASE )
        tmp.push_back( exp( 1 ) );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "Base";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};




class ComposeExponential: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeExponential()
    {
      setDefaultParam();
    }

    ~ComposeExponential()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KWindow *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return ComposeExponential::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new ComposeExponential( *this );
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
    static std::string name;
};

#endif // SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
