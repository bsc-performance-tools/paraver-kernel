#ifndef HISTOGRAMSTATISTIC_H_INCLUDED
#define HISTOGRAMSTATISTIC_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"

class KHistogram;
class Window;

struct CalculateData;

using namespace std;

class StatNumSends;
class StatNumReceives;
class StatBytesSent;
class StatBytesReceived;
class StatAvgBytesSent;
class StatAvgBytesReceived;
class StatMinBytesSent;
class StatMinBytesReceived;
class StatMaxBytesSent;
class StatMaxBytesReceived;
class StatTime;
class StatPercTime;
class StatPercTimeNotZero;
class StatPercTimeWindow;
class StatNumBursts;
class StatPercNumBursts;
class StatIntegral;
class StatAvgValue;
class StatMaximum;
class StatAvgBurstTime;
class StatStdevBurstTime;
class StatAvgPerBurst;
class StatAvgValueNotZero;
class StatNumBurstsNotZero;
class StatSumBursts;

class HistogramStatistic
{
  public:
    HistogramStatistic() {};
    virtual ~HistogramStatistic() {};

    virtual bool createComms() const = 0;
    virtual TObjectOrder getPartner( CalculateData *data ) = 0;

    virtual void init( KHistogram *whichHistogram ) = 0;
    virtual void reset() = 0;
    virtual bool filter( CalculateData *data ) const = 0;
    virtual TSemanticValue execute( CalculateData *data ) = 0;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 ) = 0;

    virtual string getName() const = 0;
    virtual string getUnits( const KHistogram *whichHisto ) const = 0;
    virtual HistogramStatistic *clone() = 0;

    static void initAllComm( KHistogram *whichHistogram );
    static void resetAllComm();
    static vector<bool> filterAllComm( CalculateData *data );
    static vector<TSemanticValue> executeAllComm( CalculateData *data );
    static vector<TSemanticValue> finishRowAllComm( vector<TSemanticValue>& cellValue,
        THistogramColumn column,
        THistogramColumn plane = 0 );

    static void initAll( KHistogram *whichHistogram );
    static void resetAll();
    static vector<bool> filterAll( CalculateData *data );
    static vector<TSemanticValue> executeAll( CalculateData *data );
    static vector<TSemanticValue> finishRowAll( vector<TSemanticValue>& cellValue,
        THistogramColumn column,
        THistogramColumn plane = 0 );

  protected:
    KHistogram *myHistogram;

  private:
    static StatNumSends statNumSends;
    static StatNumReceives statNumReceives;
    static StatBytesSent statBytesSent;
    static StatBytesReceived statBytesReceived;
    static StatAvgBytesSent statAvgBytesSent;
    static StatAvgBytesReceived statAvgBytesReceived;
    static StatMinBytesSent statMinBytesSent;
    static StatMinBytesReceived statMinBytesReceived;
    static StatMaxBytesSent statMaxBytesSent;
    static StatMaxBytesReceived statMaxBytesReceived;

    static StatTime statTime;
    static StatPercTime statPercTime;
    static StatPercTimeNotZero statPercTimeNotZero;
    static StatPercTimeWindow statPercTimeWindow;
    static StatNumBursts statNumBursts;
    static StatPercNumBursts statPercNumBursts;
    static StatIntegral statIntegral;
    static StatAvgValue statAvgValue;
    static StatMaximum statMaximum;
    static StatAvgBurstTime statAvgBurstTime;
    static StatStdevBurstTime statStdevBurstTime;
    static StatAvgPerBurst statAvgPerBurst;
    static StatAvgValueNotZero statAvgValueNotZero;
    static StatNumBurstsNotZero statNumBurstsNotZero;
    static StatSumBursts statSumBursts;

};


//-------------------------------------------------------------------------
// Communication statistics
//-------------------------------------------------------------------------
class StatNumSends: public HistogramStatistic
{
  public:
    StatNumSends() {};
    ~StatNumSends() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatNumReceives: public HistogramStatistic
{
  public:
    StatNumReceives() {};
    ~StatNumReceives() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatBytesSent: public HistogramStatistic
{
  public:
    StatBytesSent() {};
    ~StatBytesSent() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatBytesReceived: public HistogramStatistic
{
  public:
    StatBytesReceived() {};
    ~StatBytesReceived() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatAvgBytesSent: public HistogramStatistic
{
  public:
    StatAvgBytesSent() {};
    ~StatAvgBytesSent() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > numComms;
};


class StatAvgBytesReceived: public HistogramStatistic
{
  public:
    StatAvgBytesReceived() {};
    ~StatAvgBytesReceived() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > numComms;
};


class StatMinBytesSent: public HistogramStatistic
{
  public:
    StatMinBytesSent() {};
    ~StatMinBytesSent() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > min;
};


class StatMinBytesReceived: public HistogramStatistic
{
  public:
    StatMinBytesReceived() {};
    ~StatMinBytesReceived() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > min;
};


class StatMaxBytesSent: public HistogramStatistic
{
  public:
    StatMaxBytesSent() {};
    ~StatMaxBytesSent() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > max;
};


class StatMaxBytesReceived: public HistogramStatistic
{
  public:
    StatMaxBytesReceived() {};
    ~StatMaxBytesReceived() {};

    virtual bool createComms() const
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data );

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<vector<TSemanticValue> > max;
};


//-------------------------------------------------------------------------
// Semantic statistics
//-------------------------------------------------------------------------
class StatTime: public HistogramStatistic
{
  public:
    StatTime() {};
    ~StatTime() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatPercTime: public HistogramStatistic
{
  public:
    StatPercTime() {};
    ~StatPercTime() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<TSemanticValue> rowTotal;
};


class StatPercTimeNotZero: public HistogramStatistic
{
  public:
    StatPercTimeNotZero() {};
    ~StatPercTimeNotZero() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
    vector<TSemanticValue> rowTotal;
};


class StatPercTimeWindow: public HistogramStatistic
{
  public:
    StatPercTimeWindow() {};
    ~StatPercTimeWindow() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *controlWin;
};


class StatNumBursts: public HistogramStatistic
{
  public:
    StatNumBursts() {};
    ~StatNumBursts() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
};


class StatPercNumBursts: public HistogramStatistic
{
  public:
    StatPercNumBursts() {};
    ~StatPercNumBursts() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    vector<TSemanticValue> rowTotal;
};


class StatIntegral: public HistogramStatistic
{
  public:
    StatIntegral() {};
    ~StatIntegral() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
};


class StatAvgValue: public HistogramStatistic
{
  public:
    StatAvgValue() {};
    ~StatAvgValue() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatMaximum: public HistogramStatistic
{
  public:
    StatMaximum() {};
    ~StatMaximum() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > max;
};


class StatAvgBurstTime: public HistogramStatistic
{
  public:
    StatAvgBurstTime() {};
    ~StatAvgBurstTime() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatStdevBurstTime: public HistogramStatistic
{
  public:
    StatStdevBurstTime() {};
    ~StatStdevBurstTime() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > numValues;
    vector<vector<TSemanticValue> > qValues;
};


class StatAvgPerBurst: public HistogramStatistic
{
  public:
    StatAvgPerBurst() {};
    ~StatAvgPerBurst() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatAvgValueNotZero: public HistogramStatistic
{
  public:
    StatAvgValueNotZero() {};
    ~StatAvgValueNotZero() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
    vector<vector<TSemanticValue> > numValues;
};


class StatNumBurstsNotZero: public HistogramStatistic
{
  public:
    StatNumBurstsNotZero() {};
    ~StatNumBurstsNotZero() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static string name;
    Window *dataWin;
};


class StatSumBursts: public HistogramStatistic
{
  public:
    StatSumBursts() {};
    ~StatSumBursts() {};

    virtual bool createComms() const
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data )
    {
      return 0;
    }

    virtual void init( KHistogram *whichHistogram );
    virtual void reset();
    virtual bool filter( CalculateData *data ) const;
    virtual TSemanticValue execute( CalculateData *data );
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      THistogramColumn plane = 0 );

    virtual string getName() const;
    virtual string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();

  protected:

  private:
    static string name;
    Window *dataWin;
};

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
