#ifndef HISTOGRAMSTATISTIC_H_INCLUDED
#define HISTOGRAMSTATISTIC_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"

class Histogram;
class KWindow;

struct CalculateData;

using namespace std;

class HistogramStatistic
{
  public:
    HistogramStatistic();
    virtual ~HistogramStatistic();

    virtual bool createComms() const = 0;
    virtual TObjectOrder getPartner( CalculateData *data ) = 0;

    virtual void init( Histogram *whichHistogram ) = 0;
    virtual void reset() = 0;
    virtual TSemanticValue execute( CalculateData *data ) = 0;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 ) = 0;

    virtual string getName() = 0;
    virtual HistogramStatistic *clone() = 0;

  protected:
    Histogram *myHistogram;

  private:

};


//-------------------------------------------------------------------------
// Communication statistics
//-------------------------------------------------------------------------
class StatNumSends: public HistogramStatistic
{
  public:
    StatNumSends();
    ~StatNumSends();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatNumReceives: public HistogramStatistic
{
  public:
    StatNumReceives();
    ~StatNumReceives();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatBytesSent: public HistogramStatistic
{
  public:
    StatBytesSent();
    ~StatBytesSent();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatBytesReceived: public HistogramStatistic
{
  public:
    StatBytesReceived();
    ~StatBytesReceived();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatAvgBytesSent: public HistogramStatistic
{
  public:
    StatAvgBytesSent();
    ~StatAvgBytesSent();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > numComms;
};


class StatAvgBytesReceived: public HistogramStatistic
{
  public:
    StatAvgBytesReceived();
    ~StatAvgBytesReceived();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > numComms;
};


class StatMinBytesSent: public HistogramStatistic
{
  public:
    StatMinBytesSent();
    ~StatMinBytesSent();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > min;
};


class StatMinBytesReceived: public HistogramStatistic
{
  public:
    StatMinBytesReceived();
    ~StatMinBytesReceived();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > min;
};


class StatMaxBytesSent: public HistogramStatistic
{
  public:
    StatMaxBytesSent();
    ~StatMaxBytesSent();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > max;
};


class StatMaxBytesReceived: public HistogramStatistic
{
  public:
    StatMaxBytesReceived();
    ~StatMaxBytesReceived();

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<vector<TSemanticValue> > max;
};


//-------------------------------------------------------------------------
// Semantic statistics
//-------------------------------------------------------------------------
class StatTime: public HistogramStatistic
{
  public:
    StatTime();
    ~StatTime();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatPercTime: public HistogramStatistic
{
  public:
    StatPercTime();
    ~StatPercTime();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<TSemanticValue> rowTotal;
};


class StatPercTimeNotZero: public HistogramStatistic
{
  public:
    StatPercTimeNotZero();
    ~StatPercTimeNotZero();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
    vector<TSemanticValue> rowTotal;
};


class StatPercTimeWindow: public HistogramStatistic
{
  public:
    StatPercTimeWindow();
    ~StatPercTimeWindow();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *controlWin;
};


class StatNumBursts: public HistogramStatistic
{
  public:
    StatNumBursts();
    ~StatNumBursts();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
};


class StatPercNumBursts: public HistogramStatistic
{
  public:
    StatPercNumBursts();
    ~StatPercNumBursts();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    vector<TSemanticValue> rowTotal;
};


class StatIntegral: public HistogramStatistic
{
  public:
    StatIntegral();
    ~StatIntegral();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *dataWin;
};


class StatAvgValue: public HistogramStatistic
{
  public:
    StatAvgValue();
    ~StatAvgValue();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatMaximum: public HistogramStatistic
{
  public:
    StatMaximum();
    ~StatMaximum();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *dataWin;
    vector<vector<TSemanticValue> > max;
};


class StatAvgBurstTime: public HistogramStatistic
{
  public:
    StatAvgBurstTime();
    ~StatAvgBurstTime();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatStdevBurstTime: public HistogramStatistic
{
  public:
    StatStdevBurstTime();
    ~StatStdevBurstTime();

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( Histogram *whichHistogram );
    virtual void reset();
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName();
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    KWindow *dataWin;
    vector<vector<TSemanticValue> > numValues;
    vector<vector<TSemanticValue> > qValues;
};

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
