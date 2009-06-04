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


    virtual string getName()
    {
      return ComposeAsIs::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeAsIs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static string name;
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

    virtual string getName()
    {
      return ComposeSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSign( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeUnsign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeUnsign( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeMod::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeMod( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeModPlus1::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeModPlus1( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeDivide::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeDivide( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeProduct::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeProduct( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Factor";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeAdding::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeAdding( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeSubstract::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSubstract( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeSelectRange::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSelectRange( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return ComposeSelectRangeOpen::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSelectRangeOpen( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;
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

    virtual string getName()
    {
      return ComposeIsInRange::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsInRange( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return ComposeIsInRangeOpen::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsInRangeOpen( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;
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

    virtual string getName()
    {
      return ComposeIsEqual::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsEqual( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeIsEqualSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsEqualSign( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeStackedValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeStackedValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

    virtual vector<vector<TSemanticValue> > *getStack()
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static string name;

    vector<vector<TSemanticValue> > myStack;
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

    virtual string getName()
    {
      return ComposeInStackedValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeInStackedValue( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUE )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

    virtual vector<vector<TSemanticValue> > *getStack()
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static string name;

    vector<vector<TSemanticValue> > myStack;
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

    virtual string getName()
    {
      return ComposeNestingLevel::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeNestingLevel( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static string name;

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

    virtual string getName()
    {
      return ComposeDelta::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeDelta( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeBurstTime::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeBurstTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ComposeJoinBursts::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeJoinBursts( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static string name;

};

#endif // SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
