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


#pragma once


#include <string>
#include <map>
#include "paraverkerneltypes.h"

#ifdef PARALLEL_ENABLED
#include "cubebuffer.h"
#endif

class KHistogram;
class Timeline;

struct CalculateData;

constexpr size_t NUM_SEMANTIC_STATS = 17;
constexpr size_t NUM_COMM_STATS = 10;

struct TTimelinesData
{
  TRecordTime    controlBeginTime;
  TRecordTime    controlEndTime;
  TSemanticValue controlValue;

  TRecordTime    dataBeginTime;
  TRecordTime    dataEndTime;
  TSemanticValue dataValue;
};

class HistogramStatistic
{
  public:
    HistogramStatistic() {}
    virtual ~HistogramStatistic() {}

    virtual bool createComms() const = 0;
    virtual TObjectOrder getPartner( CalculateData *data ) = 0;

    virtual void init( const KHistogram& whichHistogram ) = 0;
    virtual void reset() = 0;
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) = 0;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) = 0;

    virtual std::string getName() const = 0;
    virtual std::string getUnits( const KHistogram *whichHisto ) const = 0;
    virtual bool isNotZeroStat() const
    {
      return false;
    }

    virtual HistogramStatistic *clone() = 0;

};


//-------------------------------------------------------------------------
// Communication statistics
//-------------------------------------------------------------------------
class StatNumSends: public HistogramStatistic
{
  public:
    StatNumSends() {};
    ~StatNumSends() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
};


class StatNumReceives: public HistogramStatistic
{
  public:
    StatNumReceives() {};
    ~StatNumReceives() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
};


class StatBytesSent: public HistogramStatistic
{
  public:
    StatBytesSent() {};
    ~StatBytesSent() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
};


class StatBytesReceived: public HistogramStatistic
{
  public:
    StatBytesReceived() {};
    ~StatBytesReceived() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
};


class StatAvgBytesSent: public HistogramStatistic
{
  public:
    StatAvgBytesSent() {};
    ~StatAvgBytesSent() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numComms;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > numComms;
#endif
};


class StatAvgBytesReceived: public HistogramStatistic
{
  public:
    StatAvgBytesReceived() {};
    ~StatAvgBytesReceived() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numComms;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > numComms;
#endif
};


class StatMinBytesSent: public HistogramStatistic
{
  public:
    StatMinBytesSent() {};
    ~StatMinBytesSent() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *min;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > min;
#endif
};


class StatMinBytesReceived: public HistogramStatistic
{
  public:
    StatMinBytesReceived() {};
    ~StatMinBytesReceived() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *min;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > min;
#endif
};


class StatMaxBytesSent: public HistogramStatistic
{
  public:
    StatMaxBytesSent() {};
    ~StatMaxBytesSent() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *max;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > max;
#endif
};


class StatMaxBytesReceived: public HistogramStatistic
{
  public:
    StatMaxBytesReceived() {};
    ~StatMaxBytesReceived() {};

    virtual bool createComms() const override
    {
      return true;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override;

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *max;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > max;
#endif
};


//-------------------------------------------------------------------------
// Semantic statistics
//-------------------------------------------------------------------------
class StatTime: public HistogramStatistic
{
  public:
    StatTime() {};
    ~StatTime() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
};


class StatPercTime: public HistogramStatistic
{
  public:
    StatPercTime() {};
    ~StatPercTime() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
};


class StatPercTimeNotZero: public HistogramStatistic
{
  public:
    StatPercTimeNotZero() {};
    ~StatPercTimeNotZero() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
};


class StatPercTimeWindow: public HistogramStatistic
{
  public:
    StatPercTimeWindow() {};
    ~StatPercTimeWindow() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    TRecordTime histogramTotalDuration;
};


class StatNumBursts: public HistogramStatistic
{
  public:
    StatNumBursts() {};
    ~StatNumBursts() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
};


class StatPercNumBursts: public HistogramStatistic
{
  public:
    StatPercNumBursts() {};
    ~StatPercNumBursts() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
};


class StatIntegral: public HistogramStatistic
{
  public:
    StatIntegral() {};
    ~StatIntegral() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *dataWin;
};


class StatAvgValue: public HistogramStatistic
{
  public:
    StatAvgValue() {};
    ~StatAvgValue() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};


class StatMaximum: public HistogramStatistic
{
  public:
    StatMaximum() {};
    ~StatMaximum() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *max;
#else
    std::vector<std::vector<TSemanticValue> > max;
#endif
};

class StatMinimum: public HistogramStatistic
{
  public:
    StatMinimum() {};
    ~StatMinimum() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *min;
#else
    std::vector<std::vector<TSemanticValue> > min;
#endif
};


class StatAvgBurstTime: public HistogramStatistic
{
  public:
    StatAvgBurstTime() {};
    ~StatAvgBurstTime() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};


class StatStdevBurstTime: public HistogramStatistic
{
  public:
    StatStdevBurstTime() {};
    ~StatStdevBurstTime() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
    Timeline *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
    CubeBuffer<1> *qValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
    std::vector<std::vector<TSemanticValue> > qValues;
#endif
};


class StatAvgPerBurst: public HistogramStatistic
{
  public:
    StatAvgPerBurst() {};
    ~StatAvgPerBurst() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};


class StatAvgValueNotZero: public HistogramStatistic
{
  public:
    StatAvgValueNotZero() {};
    ~StatAvgValueNotZero() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual bool isNotZeroStat() const override
    {
      return true;
    }

    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};


class StatAvgPerBurstNotZero: public HistogramStatistic
{
  public:
    StatAvgPerBurstNotZero() {};
    ~StatAvgPerBurstNotZero() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual bool isNotZeroStat() const override
    {
      return true;
    }

    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer<1> *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};



class StatNumBurstsNotZero: public HistogramStatistic
{
  public:
    StatNumBurstsNotZero() {};
    ~StatNumBurstsNotZero() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual bool isNotZeroStat() const override
    {
      return true;
    }

    virtual HistogramStatistic *clone() override;
  protected:

  private:
    static std::string name;
};


class StatSumBursts: public HistogramStatistic
{
  public:
    StatSumBursts() {};
    ~StatSumBursts() {};

    virtual bool createComms() const override
    {
      return false;
    }
    virtual TObjectOrder getPartner( CalculateData *data ) override
    {
      return 0;
    }

    virtual void init( const KHistogram& whichHistogram ) override;
    virtual void reset() override;
    
    virtual TSemanticValue execute( CalculateData *data, TTimelinesData& timelinesData ) override;
    virtual TSemanticValue finishRow( TSemanticValue cellValue,
                                      THistogramColumn column,
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) override;

    virtual std::string getName() const override;
    virtual std::string getUnits( const KHistogram *whichHisto ) const override;
    virtual HistogramStatistic *clone() override;

  protected:

  private:
    static std::string name;
};


class Statistics
{
  public:
    Statistics( const KHistogram& whichHistogram );

#ifndef PARALLEL_ENABLED
    static std::vector<TSemanticValue> zeroVector;
    static std::vector<std::vector<TSemanticValue> > zeroMatrix;
    static std::vector<std::map<TObjectOrder, TSemanticValue> > zeroCommMatrix;
#endif

    void initAllComm();
    void resetAllComm();
    bool filterAllComm( CalculateData *data );
    void executeAllComm( CalculateData *data, std::array<TSemanticValue, NUM_COMM_STATS>& onValues );
    std::array<TSemanticValue, NUM_COMM_STATS> finishRowAllComm( const std::array<TSemanticValue, NUM_COMM_STATS>& cellValue,
                                                                 THistogramColumn column,
                                                                 TObjectOrder row,
                                                                 THistogramColumn plane = 0 );

    void initAll();
    void resetAll();
    bool filterAll( CalculateData *data );
    void executeAll( CalculateData *data, std::array<TSemanticValue, NUM_SEMANTIC_STATS>& onValues, bool& isNotZeroValue );
    std::array<TSemanticValue, NUM_SEMANTIC_STATS> finishRowAll( const std::array<TSemanticValue, NUM_SEMANTIC_STATS>& cellValue,
                                                                 THistogramColumn column,
                                                                 TObjectOrder row,
                                                                 THistogramColumn plane = 0 );

  private:
    const KHistogram &myHistogram;

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
    StatMinimum statMinimum;
    StatAvgBurstTime statAvgBurstTime;
    StatStdevBurstTime statStdevBurstTime;
    StatAvgPerBurst statAvgPerBurst;
    StatAvgValueNotZero statAvgValueNotZero;
    StatAvgPerBurstNotZero statAvgPerBurstNotZero;
    StatNumBurstsNotZero statNumBurstsNotZero;
    StatSumBursts statSumBursts;

    TTimelinesData fillTimelinesData( CalculateData *data ) const;
};


