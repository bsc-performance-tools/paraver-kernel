#ifndef SEMANTICDERIVEDFUNCTIONS_H_INCLUDED
#define SEMANTICDERIVEDFUNCTIONS_H_INCLUDED

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedAdd::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedAdd( *this );
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
    static const bool controlDerived = false;
    static string name;
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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedProduct::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedProduct( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedSubstract::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedSubstract( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedDivide::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedDivide( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedMaximum::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedMaximum( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedMinimum::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedMinimum( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return DerivedDifferent::name;
    }

    virtual SemanticFunction *clone()
    {
      return new DerivedDifferent( *this );
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
    static const bool controlDerived = false;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow );

    virtual string getName()
    {
      return ControlDerivedClearBy::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ControlDerivedClearBy( *this );
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
    static const bool initFromBegin = true;
    static const bool controlDerived = false;
    static string name;

    vector<TSemanticValue> prevValue;
    vector<TSemanticValue> state;
    vector<TSemanticValue> prevResult;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return ControlDerivedMaximum::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ControlDerivedMaximum( *this );
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
    static const bool controlDerived = true;
    static string name;

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

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual bool isControlDerived()
    {
      return controlDerived;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

    virtual string getName()
    {
      return ControlDerivedAdd::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ControlDerivedAdd( *this );
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
    static const bool controlDerived = true;
    static string name;

};

#endif // SEMANTICDERIVEDFUNCTIONS_H_INCLUDED
