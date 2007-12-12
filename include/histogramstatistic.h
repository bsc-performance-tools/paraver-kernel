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

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
