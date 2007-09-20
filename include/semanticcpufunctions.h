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
      if( whichParam == VALUES )
      {
        tmp.push_back( 1 );
      }
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

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
      if( whichParam == VALUES )
      {
        tmp.push_back( 1 );
      }
      return tmp;
    }

  private:
    static const bool initFromBegin = false;

};

#endif // SEMANTICCPUFUNCTIONS_H_INCLUDED
