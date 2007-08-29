#ifndef SEMANTICDERIVEDFUNCTIONS_H_INCLUDED
#define SEMANTICDERIVEDFUNCTIONS_H_INCLUDED

#include "semanticderived.h"

class DerivedAdd:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class DerivedProduct:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class DerivedSubstract:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class DerivedDivide:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class DerivedMaximum:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class DerivedMinimum:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = false;
};


class ControlDerivedClearBy:public SemanticDerived
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

  private:
    static const bool initFromBegin = true;
    static const bool controlDerived = false;

    vector<TSemanticValue> prevValue;
    vector<TSemanticValue> state;
    vector<TSemanticValue> prevResult;

};


class ControlDerivedMaximum:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = true;
};


class ControlDerivedAdd:public SemanticDerived
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

  private:
    static const bool initFromBegin = false;
    static const bool controlDerived = true;
};

#endif // SEMANTICDERIVEDFUNCTIONS_H_INCLUDED
