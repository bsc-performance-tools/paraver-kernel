#ifndef SEMANTICCPUFUNCTIONS_H_INCLUDED
#define SEMANTICCPUFUNCTIONS_H_INCLUDED

#include "semanticcpu.h"

class ActiveThread: public SemanticCPU
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ActiveThread()
    {
      setDefaultParam();
    }

    ~ActiveThread()
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
      return ActiveThread::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ActiveThread( *this );
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

  private:
    static const bool initFromBegin = false;
    static string name;
};


class ActiveThreadSign: public SemanticCPU
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ActiveThreadSign()
    {
      setDefaultParam();
    }

    ~ActiveThreadSign()
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
      return ActiveThreadSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ActiveThreadSign( *this );
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


class ActiveThreadValues: public SemanticCPU
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ActiveThreadValues()
    {
      setDefaultParam();
    }

    ~ActiveThreadValues()
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
      return ActiveThreadValues::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ActiveThreadValues( *this );
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
      if ( whichParam == VALUES )
      {
        tmp.push_back( 1 );
      }
      return tmp;
    }

  private:
    static const bool initFromBegin = false;
    static string name;

};


class ActiveThreadValuesSign: public SemanticCPU
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ActiveThreadValuesSign()
    {
      setDefaultParam();
    }

    ~ActiveThreadValuesSign()
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
      return ActiveThreadValuesSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ActiveThreadValuesSign( *this );
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
      if ( whichParam == VALUES )
      {
        tmp.push_back( 1 );
      }
      return tmp;
    }

  private:
    static const bool initFromBegin = false;
    static string name;

};

#endif // SEMANTICCPUFUNCTIONS_H_INCLUDED
