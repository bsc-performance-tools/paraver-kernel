#ifndef SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
#define SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED

#include <stack>
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }

  private:
    static const bool initFromBegin = true;

    vector<stack<TSemanticValue> > myStack;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUE )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = true;

    vector<stack<TSemanticValue> > myStack;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }

  private:
    static const bool initFromBegin = true;

    vector<TSemanticValue> myStack;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

    vector<TSemanticValue> prevValue;
    vector<TSemanticValue> semPrevValue;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;
};

#endif // SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
