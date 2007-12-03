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

    virtual string getName()
    {
      return Adding::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Adding( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;
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

    virtual string getName()
    {
      return AddingSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new AddingSign( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return Average::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Average( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return Maximum::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Maximum( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return Minimum::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Minimum( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return Activity::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Activity( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return InActivity::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InActivity( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return NotThreadMode::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NotThreadMode( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return ObjectI::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ObjectI( *this );
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
      else if ( whichParam == OBJECT )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return AddObjectsI::name;
    }

    virtual SemanticFunction *clone()
    {
      return new AddObjectsI( *this );
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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }

  private:
    static const bool initFromBegin = false;
    static string name;

};

#endif // SEMANTICNOTTHREADFUNCTIONS_H_INCLUDED
