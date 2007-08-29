#ifndef SEMANTICTHREADFUNCTIONS_H_INCLUDED
#define SEMANTICTHREADFUNCTIONS_H_INCLUDED

#include "semanticthread.h"

class KSingleWindow;


// Semantic auxiliar functions
void getNextEvent( MemoryTrace::iterator *it, KSingleWindow *window );

TSemanticValue getTotalCommSize( MemoryTrace::iterator *itBegin,
                                 MemoryTrace::iterator *itEnd,
                                 KSingleWindow *window );


class StateAsIs: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    StateAsIs()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class Useful: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    Useful()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class StateSign: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    StateSign()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class GivenState: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    GivenState()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InState: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    InState()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class NotInState: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    NotInState()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class StateRecordDuration: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    StateRecordDuration()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class LastEventType: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    LastEventType()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class LastEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    LastEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class LastEventValueWOBursts: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    LastEventValueWOBursts()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class NextEventType: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NextEventType()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class NextEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NextEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class AverageNextEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    AverageNextEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class AverageLastEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    AverageLastEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class GivenEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    GivenEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class InEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    InEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class IntervalBetweenEvents: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    IntervalBetweenEvents()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class NotInEventValue: public SemanticThread
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    NotInEventValue()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class InEventRange: public SemanticThread
{
  public:
    typedef enum
    {
      MINVALUE = 0,
      MAXVALUE,
      MAXPARAM
    } TParam;

    InEventRange()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class EventBytes: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    EventBytes()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = EVENT;
    static const bool         initFromBegin = false;
};


class LastTag: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    LastTag()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + PHY + RECV;
    static const bool         initFromBegin = false;
};


class CommSize: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    CommSize()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM;
    static const bool         initFromBegin = false;
};


class CommRecvPartner: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    CommRecvPartner()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + LOG + RECV;
    static const bool         initFromBegin = false;
};


class CommPartner: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    CommPartner()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + LOG + SEND;
    static const bool         initFromBegin = false;
};


class LastSendDuration: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    LastSendDuration()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + LOG + SEND;
    static const bool         initFromBegin = false;
};


class NextRecvDuration: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NextRecvDuration()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + LOG + RECV;
    static const bool         initFromBegin = false;
};


class SendBytesInTransit: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    SendBytesInTransit()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + SEND + RRECV;
    static const bool         initFromBegin = true;
};


class SendMessagesInTransit: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    SendMessagesInTransit()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + SEND + RRECV;
    static const bool         initFromBegin = true;
};


class SendBandWidth: public SemanticThread
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    SendBandWidth()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow );

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == FACTOR )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = COMM + SEND + RRECV;
    static const bool         initFromBegin = true;

    vector<INT64> bandwidth;
};


class RecvBytesInTransit: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    RecvBytesInTransit()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;
};


class RecvMessagesInTransit: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    RecvMessagesInTransit()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;
};


class RecvBandWidth: public SemanticThread
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    RecvBandWidth()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow );

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == FACTOR )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;

    vector<INT64> bandwidth;
};


class RecvNegativeMessages: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    RecvNegativeMessages()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;
};


class RecvNegativeBytes: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    RecvNegativeBytes()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;
};


class NumberReceives: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NumberReceives()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV;
    static const bool         initFromBegin = true;
};


class NumberReceiveBytes: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NumberReceiveBytes()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = COMM + RECV;
    static const bool         initFromBegin = true;
};


class ApplicationID: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ApplicationID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class TaskID: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    TaskID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class ThreadID: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ThreadID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class NodeID: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    NodeID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class CPUID: public SemanticThread
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    CPUID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
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
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InApplicationID: public SemanticThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    InApplicationID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InTaskID: public SemanticThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    InTaskID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InThreadID: public SemanticThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    InThreadID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InNodeID: public SemanticThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    InNodeID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};


class InCPUID: public SemanticThread
{
  public:
    typedef enum
    {
      OBJECTS = 0,
      MAXPARAM
    } TParam;

    InCPUID()
    {
      setDefaultParam();
    }

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }
    virtual TSemanticValue execute( const SemanticInfo *info );
    virtual void init( KWindow *whichWindow )
    {}

  protected:
    virtual const TRecordType getValidateMask()
    {
      return validateMask;
    }
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
};

#endif // SEMANTICTHREADFUNCTIONS_H_INCLUDED
