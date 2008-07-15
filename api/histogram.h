#ifndef HISTOGRAM_H_INCLUDED
#define HISTOGRAM_H_INCLUDED

#include <limits>
#include "paraverkerneltypes.h"
#include "paraverconfig.h"

class KernelConnection;
class HistogramTotals;
class Window;

class Histogram
{
  public:
    static Histogram *create( KernelConnection *whichKernel );

    Histogram() {};
    Histogram( KernelConnection *whichKernel );
    virtual ~Histogram() {}

    virtual void setWindowBeginTime( TRecordTime whichTime );
    virtual void setWindowEndTime( TRecordTime whichTime );

    virtual bool getThreeDimensions() const = 0;

    virtual TRecordTime getBeginTime() const = 0;
    virtual TRecordTime getEndTime() const = 0;

    virtual Window *getControlWindow() const = 0;
    virtual Window *getDataWindow() const = 0;
    virtual Window *getExtraControlWindow() const = 0;
    virtual void setControlWindow( Window *whichWindow ) = 0;
    virtual void setDataWindow( Window *whichWindow ) = 0;
    virtual void setExtraControlWindow( Window *whichWindow ) = 0;
    virtual void clearControlWindow() = 0;
    virtual void clearDataWindow() = 0;
    virtual void clearExtraControlWindow() = 0;

    virtual void setControlMin( THistogramLimit whichMin ) = 0;
    virtual void setControlMax( THistogramLimit whichMax ) = 0;
    virtual void setControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setExtraControlMin( THistogramLimit whichMin ) = 0;
    virtual void setExtraControlMax( THistogramLimit whichMax ) = 0;
    virtual void setExtraControlDelta( THistogramLimit whichDelta ) = 0;
    virtual void setDataMin( TSemanticValue whichMin ) = 0;
    virtual void setDataMax( TSemanticValue whichMax ) = 0;
    virtual void setBurstMin( TRecordTime whichTime ) = 0;
    virtual void setBurstMax( TRecordTime whichTime ) = 0;
    virtual void setCommSizeMin( TCommSize whichSize ) = 0;
    virtual void setCommSizeMax( TCommSize whichSize ) = 0;
    virtual void setCommTagMin( TCommTag whichTag ) = 0;
    virtual void setCommTagMax( TCommTag whichTag ) = 0;

    virtual THistogramLimit getControlMin() const = 0;
    virtual THistogramLimit getControlMax() const = 0;
    virtual THistogramLimit getControlDelta() const = 0;
    virtual THistogramLimit getExtraControlMin() const = 0;
    virtual THistogramLimit getExtraControlMax() const = 0;
    virtual THistogramLimit getExtraControlDelta() const = 0;
    virtual TSemanticValue getDataMin() const = 0;
    virtual TSemanticValue getDataMax() const = 0;
    virtual TRecordTime getBurstMin() const = 0;
    virtual TRecordTime getBurstMax() const = 0;
    virtual TCommSize getCommSizeMin() const = 0;
    virtual TCommSize getCommSizeMax() const = 0;
    virtual TCommTag getCommTagMin() const = 0;
    virtual TCommTag getCommTagMax() const = 0;

    virtual void setInclusive( bool newValue ) = 0;

    virtual THistogramColumn getNumPlanes() const = 0;
    virtual THistogramColumn getNumColumns() const = 0;
    virtual TObjectOrder getNumRows() const = 0;

    virtual TSemanticValue getCurrentValue( UINT32 col,
                                            UINT16 idStat,
                                            UINT32 plane = 0 ) const = 0;
    virtual UINT32 getCurrentRow( UINT32 col, UINT32 plane = 0 ) const = 0;
    virtual void setNextCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual void setFirstCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool endCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool planeWithValues( UINT32 plane = 0 ) const = 0;

    virtual TSemanticValue getCommCurrentValue( UINT32 col,
        UINT16 idStat,
        UINT32 plane = 0 ) const = 0;
    virtual UINT32 getCommCurrentRow( UINT32 col, UINT32 plane = 0 ) const = 0;
    virtual void setCommNextCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual void setCommFirstCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool endCommCell( UINT32 col, UINT32 plane = 0 ) = 0;
    virtual bool planeCommWithValues( UINT32 plane = 0 ) const = 0;

    virtual HistogramTotals *getColumnTotals() const = 0;
    virtual HistogramTotals *getCommColumnTotals() const = 0;
    virtual HistogramTotals *getRowTotals() const = 0;
    virtual HistogramTotals *getCommRowTotals() const = 0;

    virtual void clearStatistics() = 0;
    virtual void pushbackStatistic( string& whichStatistic ) = 0;

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime ) = 0;

    virtual bool itsCommunicationStat( const string& whichStat ) const = 0;

    virtual string getUnitsLabel( const string& whichStat ) const = 0;

    // Specific methods of HistogramProxy
    virtual HistogramTotals *getTotals( const string& whichStat ) const
    {
      return NULL;
    }
    virtual void setHorizontal( bool newValue ) {}
    virtual bool getHorizontal() const
    {
      return true;
    }
    virtual void setHideColumns( bool newValue ) {}
    virtual bool getHideColumns() const
    {
      return false;
    }
    virtual void setScientificNotation( bool newValue ) {}
    virtual bool getScientificNotation() const
    {
      return false;
    }
    virtual void setNumDecimals( UINT16 newValue ) {}
    virtual UINT16 getNumDecimals() const
    {
      return 2;
    }
    virtual void setThousandSeparator( bool newValue ) {}
    virtual bool getThousandSeparator() const
    {
      return false;
    }
    virtual void setShowUnits( bool newValue ) {}
    virtual bool getShowUnits() const
    {
      return ParaverConfig::getInstance()->getShowUnits();
    }
    virtual void setSortColumns( bool newValue ) {}
    virtual bool getSortColumns() const
    {
      return false;
    }
    virtual void setSortCriteria( THistoTotals whichCriteria ) {}
    virtual THistoTotals getSortCriteria() const
    {
      return AVERAGE;
    }
    virtual void setMinGradient( double whichMin ) {}
    virtual double getMinGradient() const
    {
      return std::numeric_limits<double>::min();
    }
    virtual void setMaxGradient( double whichMax ) {}
    virtual double getMaxGradient() const
    {
      return std::numeric_limits<double>::max();
    }
    virtual void setComputeScale( bool newValue ) {}
    virtual bool getComputeScale() const
    {
      return false;
    }
    virtual void setComputeGradient( bool newValue ) {}
    virtual bool getComputeGradient() const
    {
      return false;
    }
    virtual void setPlaneMinValue( double whichMin ) {}
    virtual INT32 getSelectedPlane() const
    {
      return 0;
    }
    virtual INT32 getCommSelectedPlane() const
    {
      return 0;
    }
    virtual void compute2DScale() {}
    virtual void compute3DScale() {}
    virtual string getRowLabel( TObjectOrder whichRow ) const
    {
      return "Unamed row";
    }
    virtual string getColumnLabel( THistogramColumn whichColumn ) const
    {
      return "Unamed column";
    }
    virtual string getPlaneLabel( THistogramColumn whichPlane ) const
    {
      return "Unamed plane";
    }

    virtual void setName( const string& whichName ) {}
    virtual string getName() const
    {
      return "Unamed histogram";
    }

    virtual void setCalculateAll( bool status ) {}
    virtual bool getCalculateAll() const
    {
      return true;
    }

    virtual bool getIdStat( const string& whichStat, UINT16& idStat ) const
    {
      return false;
    }

    virtual void setCurrentStat( const string& whichStat ) {}
    virtual string getCurrentStat() const
    {
      return "";
    }

    virtual THistogramColumn getNumColumns( const string& whichStat ) const
    {
      return getNumColumns();
    }

  protected:
    KernelConnection *myKernel;

};


class HistogramProxy : public Histogram
{
  public:
    virtual ~HistogramProxy();

    virtual void setWindowBeginTime( TRecordTime whichTime );
    virtual void setWindowEndTime( TRecordTime whichTime );

    virtual bool getThreeDimensions() const;
    virtual TRecordTime getBeginTime() const;
    virtual TRecordTime getEndTime() const;
    virtual Window *getControlWindow() const;
    virtual Window *getDataWindow() const;
    virtual Window *getExtraControlWindow() const;
    virtual void setControlWindow( Window *whichWindow );
    virtual void setDataWindow( Window *whichWindow );
    virtual void setExtraControlWindow( Window *whichWindow );
    virtual void clearControlWindow();
    virtual void clearDataWindow();
    virtual void clearExtraControlWindow();
    virtual void setControlMin( THistogramLimit whichMin );
    virtual void setControlMax( THistogramLimit whichMax );
    virtual void setControlDelta( THistogramLimit whichDelta );
    virtual void setExtraControlMin( THistogramLimit whichMin );
    virtual void setExtraControlMax( THistogramLimit whichMax );
    virtual void setExtraControlDelta( THistogramLimit whichDelta );
    virtual void setDataMin( THistogramLimit whichMin );
    virtual void setDataMax( THistogramLimit whichMax );
    virtual void setBurstMin( TRecordTime whichTime );
    virtual void setBurstMax( TRecordTime whichTime );
    virtual void setCommSizeMin( TCommSize whichSize );
    virtual void setCommSizeMax( TCommSize whichSize );
    virtual void setCommTagMin( TCommTag whichTag );
    virtual void setCommTagMax( TCommTag whichTag );

    virtual THistogramLimit getControlMin() const;
    virtual THistogramLimit getControlMax() const;
    virtual THistogramLimit getControlDelta() const;
    virtual THistogramLimit getExtraControlMin() const;
    virtual THistogramLimit getExtraControlMax() const;
    virtual THistogramLimit getExtraControlDelta() const;
    virtual THistogramLimit getDataMin() const;
    virtual THistogramLimit getDataMax() const;
    virtual TRecordTime getBurstMin() const;
    virtual TRecordTime getBurstMax() const;
    virtual TCommSize getCommSizeMin() const;
    virtual TCommSize getCommSizeMax() const;
    virtual TCommTag getCommTagMin() const;
    virtual TCommTag getCommTagMax() const;

    virtual void setInclusive( bool newValue );
    virtual THistogramColumn getNumPlanes() const;
    virtual THistogramColumn getNumColumns( const string& whichStat ) const;
    virtual TObjectOrder getNumRows() const;
    virtual TSemanticValue getCurrentValue( UINT32 col,
                                            UINT16 idStat,
                                            UINT32 plane = 0 ) const;
    virtual UINT32 getCurrentRow( UINT32 col, UINT32 plane = 0 ) const;
    virtual void setNextCell( UINT32 col, UINT32 plane = 0 );
    virtual void setFirstCell( UINT32 col, UINT32 plane = 0 );
    virtual bool endCell( UINT32 col, UINT32 plane = 0 );
    virtual bool planeWithValues( UINT32 plane = 0 ) const;
    virtual TSemanticValue getCommCurrentValue( UINT32 col,
        UINT16 idStat,
        UINT32 plane = 0 ) const;
    virtual UINT32 getCommCurrentRow( UINT32 col, UINT32 plane = 0 ) const;
    virtual void setCommNextCell( UINT32 col, UINT32 plane = 0 );
    virtual void setCommFirstCell( UINT32 col, UINT32 plane = 0 );
    virtual bool endCommCell( UINT32 col, UINT32 plane = 0 );
    virtual bool planeCommWithValues( UINT32 plane = 0 ) const;

    virtual HistogramTotals *getTotals( const string& whichStat ) const;
    virtual HistogramTotals *getColumnTotals() const;
    virtual HistogramTotals *getCommColumnTotals() const;
    virtual HistogramTotals *getRowTotals() const;
    virtual HistogramTotals *getCommRowTotals() const;

    virtual void clearStatistics();
    virtual void pushbackStatistic( string& whichStatistic );

    virtual void execute( TRecordTime whichBeginTime, TRecordTime whichEndTime );

    virtual void setHorizontal( bool newValue );
    virtual bool getHorizontal() const;
    virtual void setHideColumns( bool newValue );
    virtual bool getHideColumns() const;
    virtual void setShowUnits( bool newValue );
    virtual bool getShowUnits() const;
    virtual void setSortColumns( bool newValue );
    virtual bool getSortColumns() const;
    virtual void setSortCriteria( THistoTotals whichCriteria );
    virtual THistoTotals getSortCriteria() const;
    virtual void setMinGradient( double whichMin );
    virtual double getMinGradient() const;
    virtual void setMaxGradient( double whichMax );
    virtual double getMaxGradient() const;
    virtual void setComputeScale( bool newValue );
    virtual bool getComputeScale() const;
    virtual void setComputeGradient( bool newValue );
    virtual bool getComputeGradient() const;
    virtual void setPlaneMinValue( double whichMin );
    virtual INT32 getSelectedPlane() const;
    virtual INT32 getCommSelectedPlane() const;

    virtual void compute2DScale();
    virtual void compute3DScale();

    virtual string getRowLabel( TObjectOrder whichRow ) const;
    virtual string getColumnLabel( THistogramColumn whichColumn ) const;
    virtual string getPlaneLabel( THistogramColumn whichPlane ) const;

    virtual void setName( const string& whichName );
    virtual string getName() const;

    virtual void setCalculateAll( bool status );
    virtual bool getCalculateAll() const;

    virtual bool getIdStat( const string& whichStat, UINT16& idStat ) const;

    virtual void setCurrentStat( const string& whichStat );
    virtual string getCurrentStat() const;

    bool itsCommunicationStat( const string& whichStat ) const;

    string getUnitsLabel( const string& whichStat ) const;

  private:
    string name;

    bool horizontal;
    bool hideColumns;
    bool showUnits;
    bool sortColumns;
    THistoTotals sortCriteria;
    double minGradient;
    double maxGradient;
    bool computeScale;
    bool computeGradient;
    bool futurePlane;
    double planeMinValue;
    INT32 selectedPlane;
    INT32 commSelectedPlane;

    TRecordTime winBeginTime;
    TRecordTime winEndTime;

    // Must store the associated proxies
    Window *controlWindow;
    Window *dataWindow;
    Window *extraControlWindow;

    bool calculateAll;
    string currentStat;
    vector<string> calcStat;
    vector<string> commCalcStat;

    Histogram *myHisto;

    HistogramProxy( KernelConnection *whichKernel );

    THistogramColumn getNumColumns() const;

    friend Histogram *Histogram::create( KernelConnection * );
};


#endif // HISTOGRAM_H_INCLUDED
