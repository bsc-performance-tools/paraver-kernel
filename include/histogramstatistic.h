/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef HISTOGRAMSTATISTIC_H_INCLUDED
#define HISTOGRAMSTATISTIC_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"

class KHistogram;
class Window;

struct CalculateData;

using namespace std;

class HistogramStatistic
{
  public:
    HistogramStatistic() {}
    virtual ~HistogramStatistic() {}

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

  protected:
    KHistogram *myHistogram;
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
    Window *controlWin;
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


class Statistics
{
  public:
    Statistics() {}
    ~Statistics() {}

    static int getNumCommStats();
    void initAllComm( KHistogram *whichHistogram );
    void resetAllComm();
    vector<bool> filterAllComm( CalculateData *data );
    vector<TSemanticValue> executeAllComm( CalculateData *data );
    vector<TSemanticValue> finishRowAllComm( vector<TSemanticValue>& cellValue,
        THistogramColumn column,
        THistogramColumn plane = 0 );

    static int getNumStats();
    void initAll( KHistogram *whichHistogram );
    void resetAll();
    vector<bool> filterAll( CalculateData *data );
    vector<TSemanticValue> executeAll( CalculateData *data );
    vector<TSemanticValue> finishRowAll( vector<TSemanticValue>& cellValue,
                                         THistogramColumn column,
                                         THistogramColumn plane = 0 );

  private:
    StatNumSends statNumSends;
    StatNumReceives statNumReceives;
    StatBytesSent statBytesSent;
    StatBytesReceived statBytesReceived;
    StatAvgBytesSent statAvgBytesSent;
    StatAvgBytesReceived statAvgBytesReceived;
    StatMinBytesSent statMinBytesSent;
    StatMinBytesReceived statMinBytesReceived;
    StatMaxBytesSent statMaxBytesSent;
    StatMaxBytesReceived statMaxBytesReceived;

    StatTime statTime;
    StatPercTime statPercTime;
    StatPercTimeNotZero statPercTimeNotZero;
    StatPercTimeWindow statPercTimeWindow;
    StatNumBursts statNumBursts;
    StatPercNumBursts statPercNumBursts;
    StatIntegral statIntegral;
    StatAvgValue statAvgValue;
    StatMaximum statMaximum;
    StatAvgBurstTime statAvgBurstTime;
    StatStdevBurstTime statStdevBurstTime;
    StatAvgPerBurst statAvgPerBurst;
    StatAvgValueNotZero statAvgValueNotZero;
    StatNumBurstsNotZero statNumBurstsNotZero;
    StatSumBursts statSumBursts;

    static int numCommStats;
    static int numStats;

};

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
