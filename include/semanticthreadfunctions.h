#ifndef SEMANTICTHREADFUNCTIONS_H_INCLUDED
#define SEMANTICTHREADFUNCTIONS_H_INCLUDED

#include "semanticthread.h"

class KSingleWindow;


// Semantic auxiliar functions
void getNextEvent( MemoryTrace::iterator *it, KSingleWindow *window );

TSemanticValue getTotalCommSize( MemoryTrace::iterator *itBegin,
                                 MemoryTrace::iterator *itEnd,
                                 KSingleWindow *window );


/**************************
** State functions (Thread)
***************************/

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

    virtual string getName()
    {
      return StateAsIs::name;
    }

    virtual SemanticFunction *clone()
    {
      return new StateAsIs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return STATE_TYPE;
    }

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
    static string name;
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

    virtual string getName()
    {
      return Useful::name;
    }

    virtual SemanticFunction *clone()
    {
      return new Useful( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return STATE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return StateSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new StateSign( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return GivenState::name;
    }

    virtual SemanticFunction *clone()
    {
      return new GivenState( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return STATE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return InState::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InState( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return NotInState::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NotInState( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return StateRecordDuration::name;
    }

    virtual SemanticFunction *clone()
    {
      return new StateRecordDuration( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

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
    static string name;

};


/**************************
** Event functions (Thread)
***************************/

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

    virtual string getName()
    {
      return LastEventType::name;
    }

    virtual SemanticFunction *clone()
    {
      return new LastEventType( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTTYPE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return LastEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new LastEventValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTVALUE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return LastEventValueWOBursts::name;
    }

    virtual SemanticFunction *clone()
    {
      return new LastEventValueWOBursts( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTVALUE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NextEventType::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NextEventType( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTTYPE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NextEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NextEventValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTVALUE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return AverageNextEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new AverageNextEventValue( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return AverageLastEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new AverageLastEventValue( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return GivenEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new GivenEventValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return EVENTVALUE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return InEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InEventValue( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return IntervalBetweenEvents::name;
    }

    virtual SemanticFunction *clone()
    {
      return new IntervalBetweenEvents( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NotInEventValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NotInEventValue( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return InEventRange::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InEventRange( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return EventBytes::name;
    }

    virtual SemanticFunction *clone()
    {
      return new EventBytes( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

};


/**************************
** Comm functions (Thread)
***************************/

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

    virtual string getName()
    {
      return LastTag::name;
    }

    virtual SemanticFunction *clone()
    {
      return new LastTag( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMTAG_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return CommSize::name;
    }

    virtual SemanticFunction *clone()
    {
      return new CommSize( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return CommRecvPartner::name;
    }

    virtual SemanticFunction *clone()
    {
      return new CommRecvPartner( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return OBJECT_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return CommPartner::name;
    }

    virtual SemanticFunction *clone()
    {
      return new CommPartner( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return OBJECT_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return LastSendDuration::name;
    }

    virtual SemanticFunction *clone()
    {
      return new LastSendDuration( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NextRecvDuration::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NextRecvDuration( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return SendBytesInTransit::name;
    }

    virtual SemanticFunction *clone()
    {
      return new SendBytesInTransit( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return SendMessagesInTransit::name;
    }

    virtual SemanticFunction *clone()
    {
      return new SendMessagesInTransit( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return SendBandWidth::name;
    }

    virtual SemanticFunction *clone()
    {
      return new SendBandWidth( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return BANDWIDTH_TYPE;
    }

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
    static const TRecordType  validateMask = COMM + SEND + RRECV;
    static const bool         initFromBegin = true;
    static string name;


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

    virtual string getName()
    {
      return RecvBytesInTransit::name;
    }

    virtual SemanticFunction *clone()
    {
      return new RecvBytesInTransit( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return RecvMessagesInTransit::name;
    }

    virtual SemanticFunction *clone()
    {
      return new RecvMessagesInTransit( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return RecvBandWidth::name;
    }

    virtual SemanticFunction *clone()
    {
      return new RecvBandWidth( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return BANDWIDTH_TYPE;
    }

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
    static const TRecordType  validateMask = COMM + RECV + RSEND;
    static const bool         initFromBegin = true;
    static string name;


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

    virtual string getName()
    {
      return RecvNegativeMessages::name;
    }

    virtual SemanticFunction *clone()
    {
      return new RecvNegativeMessages( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return RecvNegativeBytes::name;
    }

    virtual SemanticFunction *clone()
    {
      return new RecvNegativeBytes( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NumberReceives::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NumberReceives( *this );
    }


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
    static string name;

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

    virtual string getName()
    {
      return NumberReceiveBytes::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NumberReceiveBytes( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return COMMSIZE_TYPE;
    }

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
    static string name;

};


/**************************
** Object functions (Thread)
***************************/

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

    virtual string getName()
    {
      return ApplicationID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ApplicationID( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return APPL_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return TaskID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new TaskID( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TASK_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return ThreadID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ThreadID( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return THREAD_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return NodeID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new NodeID( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NODE_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return CPUID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new CPUID( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return CPU_TYPE;
    }

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
    static string name;

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

    virtual string getName()
    {
      return InApplicationID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InApplicationID( *this );
    }


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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return InTaskID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InTaskID( *this );
    }


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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return InThreadID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InThreadID( *this );
    }


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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return InNodeID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InNodeID( *this );
    }


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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
    static string name;

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

    virtual string getName()
    {
      return InCPUID::name;
    }

    virtual SemanticFunction *clone()
    {
      return new InCPUID( *this );
    }


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
      else if ( whichParam == OBJECTS )
        tmp.push_back( 1 );

      return tmp;
    }
  private:
    static const TRecordType  validateMask = STATE + BEGIN;
    static const bool         initFromBegin = false;
    static string name;

};

#endif // SEMANTICTHREADFUNCTIONS_H_INCLUDED
