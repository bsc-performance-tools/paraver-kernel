#ifndef SEMANTICCOLOR_H_INCLUDED
#define SEMANTICCOLOR_H_INCLUDED

#include "paravertypes.h"
#include "paraverkerneltypes.h"


class Window;

class SemanticColor
{
  public:
    static rgb BACKGROUND;

    static UINT32 getNumColors();
    static rgb* getCodeColors();
    static rgb getBeginGradientColor();
    static rgb getEndGradientColor();
    static rgb getAboveOutlierColor();
    static rgb getBelowOutlierColor();

    virtual ~SemanticColor()
    {};

    virtual rgb calcColor( TSemanticValue whichValue, Window& whichWindow ) = 0;

  private:
    static UINT32 numColors;
    static rgb codeColor[];

    static rgb beginGradientColor;
    static rgb endGradientColor;
    static rgb aboveOutlierColor;
    static rgb belowOutlierColor;

    static const double GradientSteps;
};

class CodeColor: public SemanticColor
{
  public:
    CodeColor();
    ~CodeColor();

    UINT32 getNumColors() const;
    rgb getColor( UINT32 pos ) const;
    void setColor( UINT32 pos, rgb color );
    void addColor( rgb color );
    rgb calcColor( TSemanticValue whichValue, Window& whichWindow  );

  private:
    vector<rgb> colors;
};


class GradientColor: public SemanticColor
{
  public:
    GradientColor();
    ~GradientColor();

    void setBeginGradientColor( rgb color );
    rgb getBeginGradientColor() const;

    void setEndGradientColor( rgb color );
    rgb getEndGradientColor() const;

    void setAboveOutlierColor( rgb color );
    rgb getAboveOutlierColor() const;

    void setBelowOutlierColor( rgb color );
    rgb getBelowOutlierColor() const;

    void allowOutliers( bool activate );
    void allowOutOfScale( bool activate );

    rgb calcColor( TSemanticValue whichValue, Window& whichWindow );

  private:
    bool drawOutlier;
    bool drawOutOfScale;

    rgb beginGradientColor;
    rgb endGradientColor;
    rgb aboveOutlierColor;
    rgb belowOutlierColor;

    double redStep;
    double greenStep;
    double blueStep;

    void recalcSteps();
};

#endif // SEMANTICCOLOR_H_INCLUDED
