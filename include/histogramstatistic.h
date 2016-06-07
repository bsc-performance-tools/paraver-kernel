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
#include <map>
#include "paraverkerneltypes.h"

class KHistogram;
class Window;
#ifdef PARALLEL_ENABLED
class CubeBuffer;
#endif

struct CalculateData;

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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 ) = 0;

    virtual std::string getName() const = 0;
    virtual std::string getUnits( const KHistogram *whichHisto ) const = 0;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numComms;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > numComms;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numComms;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > numComms;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *min;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > min;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *min;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > min;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *max;
#else
    std::vector<std::map<TObjectOrder, TSemanticValue> > max;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *max;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
#ifdef PARALLEL_ENABLED
    CubeBuffer *rowTotal;
#else
    std::vector<TSemanticValue> rowTotal;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *max;
#else
    std::vector<std::vector<TSemanticValue> > max;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *controlWin;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
    CubeBuffer *qValues;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
};


class StatAvgPerBurstNotZero: public HistogramStatistic
{
  public:
    StatAvgPerBurstNotZero() {};
    ~StatAvgPerBurstNotZero() {};

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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
    Window *dataWin;
#ifdef PARALLEL_ENABLED
    CubeBuffer *numValues;
#else
    std::vector<std::vector<TSemanticValue> > numValues;
#endif
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();
  protected:

  private:
    static std::string name;
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
                                      TObjectOrder row,
                                      THistogramColumn plane = 0 );

    virtual std::string getName() const;
    virtual std::string getUnits( const KHistogram *whichHisto ) const;
    virtual HistogramStatistic *clone();

  protected:

  private:
    static std::string name;
    Window *dataWin;
};





class Statistics
{
  public:
    Statistics() {}
    ~Statistics() {}

#ifndef PARALLEL_ENABLED
    static std::vector<TSemanticValue> zeroVector;
    static std::vector<std::vector<TSemanticValue> > zeroMatrix;
    static std::vector<std::map<TObjectOrder, TSemanticValue> > zeroCommMatrix;
#endif

    static int getNumCommStats();

    void initAllComm( KHistogram *whichHistogram );
    void resetAllComm();
    std::vector<bool> filterAllComm( CalculateData *data );
    std::vector<TSemanticValue> executeAllComm( CalculateData *data );
    std::vector<TSemanticValue> finishRowAllComm( const std::vector<TSemanticValue>& cellValue,
        THistogramColumn column,
        TObjectOrder row,
        THistogramColumn plane = 0 );

    static int getNumStats();
    void initAll( KHistogram *whichHistogram );
    void resetAll();
    std::vector<bool> filterAll( CalculateData *data );
    std::vector<TSemanticValue> executeAll( CalculateData *data );
    std::vector<TSemanticValue> finishRowAll( const std::vector<TSemanticValue>& cellValue,
                                         THistogramColumn column,
                                         TObjectOrder row,
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
    StatAvgPerBurstNotZero statAvgPerBurstNotZero;
    StatNumBurstsNotZero statNumBurstsNotZero;
    StatSumBursts statSumBursts;

    static int numCommStats;
    static int numStats;

};

#endif // HISTOGRAMSTATISTIC_H_INCLUDED
