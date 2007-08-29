#ifndef SEMANTICNOTTHREADFUNCTIONS_H_INCLUDED
#define SEMANTICNOTTHREADFUNCTIONS_H_INCLUDED

#include "semanticnotthread.h"

class Adding: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    Adding()
    {
      setDefaultParam();
    }

    ~Adding()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class AddingSign: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    AddingSign()
    {
      setDefaultParam();
    }

    ~AddingSign()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class Average: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    Average()
    {
      setDefaultParam();
    }

    ~Average()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class Maximum: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    Maximum()
    {
      setDefaultParam();
    }

    ~Maximum()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class Minimum: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    Minimum()
    {
      setDefaultParam();
    }

    ~Minimum()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class Activity: public SemanticNotThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    Activity()
    {
      setDefaultParam();
    }

    ~Activity()
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

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

};


class InActivity: public SemanticNotThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    InActivity()
    {
      setDefaultParam();
    }

    ~InActivity()
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

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

};


class NotThreadMode: public SemanticNotThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NotThreadMode()
    {
      setDefaultParam();
    }

    ~NotThreadMode()
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
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return (TParamValue) 0;
    }

  private:
    static const bool initFromBegin = false;

};


class ObjectI: public SemanticNotThread
{
  public:
    typedef enum
    {
      OBJECT = 0,
      MAXPARAM
    } TParam;

    ObjectI()
    {
      setDefaultParam();
    }

    ~ObjectI()
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

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == OBJECT )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

};


class AddObjectsI: public SemanticNotThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    AddObjectsI()
    {
      setDefaultParam();
    }

    ~AddObjectsI()
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

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;

};

#endif // SEMANTICNOTTHREADFUNCTIONS_H_INCLUDED
