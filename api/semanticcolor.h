#ifndef SEMANTICCOLOR_H_INCLUDED
#define SEMANTICCOLOR_H_INCLUDED

#include "paravertypes.h"
#include "paraverkerneltypes.h"


class Trace;
class Window;

class SemanticColor
{
  public:
    static UINT32 getNumColors();
    static rgb* getCodeColors();
    static rgb getBeginGradientColor();
    static rgb getEndGradientColor();
    static rgb getAboveOutlierColor();
    static rgb getBelowOutlierColor();

    virtual ~SemanticColor() {};

    virtual rgb calcColor( TSemanticValue whichValue, Window& whichWindow ) const = 0;

  private:
    static UINT32 numColors;
    static rgb codeColor[];

    static rgb beginGradientColor;
    static rgb endGradientColor;
    static rgb aboveOutlierColor;
    static rgb belowOutlierColor;
};

class CodeColor: public SemanticColor
{
  public:
    CodeColor( Trace& whichTrace );
    ~CodeColor();

    UINT32 getNumColors() const;
    rgb getColor( UINT32 pos ) const;
    void setColor( UINT32 pos, rgb color );
    void addColor( rgb color );
    rgb calcColor( TSemanticValue whichValue, Window& whichWindow  ) const;

  private:
    vector<rgb> colors;
};


#endif // SEMANTICCOLOR_H_INCLUDED
