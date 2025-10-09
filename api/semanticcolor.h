/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include <array>
#include <functional>
#include <map>
#include "paravertypes.h"
#include "paraverkerneltypes.h"

class Timeline;

enum class TColorFunction
{
  CODE_COLOR = 0,
  GRADIENT,
  NOT_NULL_GRADIENT,
  FUNCTION_LINE,
  PUNCTUAL,
  FUSED_LINES,
  ALTERNATIVE_GRADIENT
};

enum class TGradientFunction
{
  LINEAR = 0,
  STEPS,
  LOGARITHMIC,
  EXPONENTIAL
};

class SemanticColor
{
  public:
    using TRangeFunctions = std::map< double, std::function< rgb( TSemanticValue )> >;

    static rgb BACKGROUND;
    static rgb FOREGROUND;
    static rgb ZERO_AXIS;

    static rgb DEFAULT_PUNCTUAL;
    static rgb DEFAULT_LOGICAL_COMMUNICATIONS;
    static rgb DEFAULT_PHYSICAL_COMMUNICATIONS;
    static rgb DEFAULT_BEGIN_GRADIENT_COLOR;
    static rgb DEFAULT_END_GRADIENT_COLOR;
    static rgb DEFAULT_NEGATIVE_BEGIN_GRADIENT_COLOR;
    static rgb DEFAULT_NEGATIVE_END_GRADIENT_COLOR;
    static rgb DEFAULT_ABOVE_OUTLIER_COLOR;
    static rgb DEFAULT_BELOW_OUTLIER_COLOR;

    static PRV_UINT32 getFixedNumColors();
    static rgb* getFixedCodeColors();

    template<typename C>
    static int getLuminance( C red, C green, C blue )
    {
      return ( red   * 30 ) / 100 +
             ( green * 59 ) / 100 +
             ( blue  * 11 ) / 100;
    }

    // NO initialization, nor expand of code colors is done.
    SemanticColor() {}

    // All initializations and expand of code colors.
    // For performance reasons (expand code colors), it should be called in few locations, like trace loading
    // and then the SemanticColor object copied to othes, like Timelines.
    SemanticColor( const std::vector< rgb >& whichAlternativeGradientColors );

    rgb calcColor( TSemanticValue whichValue,
                   TSemanticValue minimum,
                   TSemanticValue maximum ) const;

    void setColorMode( TColorFunction whichMode );
    TColorFunction getColorMode() const;

    void setCodeColorMode();
    void setGradientColorMode();
    void setAlternativeGradientColorMode();
    void setNotNullGradientColorMode();
    void setFunctionLineColorMode();
    void setFusedLinesColorMode();
    void setPunctualColorMode();
    bool isCodeColorSet() const;
    bool isGradientColorSet() const;
    bool isNotNullGradientColorSet() const;
    bool isAlternativeGradientColorSet() const;
    bool isFunctionLineColorSet() const;
    bool isFusedLinesColorSet() const;
    bool isPunctualColorSet() const;

    // Code Color methods
    PRV_UINT32 getNumColors() const;
    void setColor( PRV_UINT32 pos, rgb color );
    void setCustomColor( TSemanticValue whichValue, rgb color );
    bool existCustomColors() const;
    const std::map<TSemanticValue, rgb>& getCustomPalette() const;
    void setCustomPalette( const std::map<TSemanticValue, rgb>& whichPalette );
    bool getUseCustomPalette() const;
    void setUseCustomPalette( bool newValue );
    bool isColorSimilarToBackground( rgb whichColor ) const;

    void setCustomBackgroundColor( rgb whichColor );
    void setCustomAxisColor( rgb whichColor );
    void setCustomPunctualColor( rgb whichColor );
    rgb getCustomBackgroundColor() const;
    rgb getCustomAxisColor() const;
    rgb getCustomPunctualColor() const;

    // Gradient Color methods
    void setBeginGradientColor( rgb color );
    rgb getBeginGradientColor() const;

    void setEndGradientColor( rgb color );
    rgb getEndGradientColor() const;

    void setNegativeBeginGradientColor( rgb color );
    rgb getNegativeBeginGradientColor() const;

    void setNegativeEndGradientColor( rgb color );
    rgb getNegativeEndGradientColor() const;

    void setAboveOutlierColor( rgb color );
    rgb getAboveOutlierColor() const;

    void setBelowOutlierColor( rgb color );
    rgb getBelowOutlierColor() const;

    void allowOutliers( bool activate );
    bool getAllowOutliers() const;
    void allowOutOfScale( bool activate );
    bool getAllowOutOfScale() const;

    TGradientFunction getGradientFunction() const;
    void setGradientFunction( TGradientFunction whichFunction );

    bool isColorOutlier( rgb whichColor ) const;

  private:
    static PRV_UINT32 numFixedCodeColors;
    static rgb fixedCodeColor[];

    static const int MAX_COLORS;

    TColorFunction colorMode;

    std::vector<rgb> colors;
    std::map<TSemanticValue, rgb> customPalette;
    bool useCustomPalette = false;
    rgb customBackgroundColor;
    rgb customAxisColor;
    rgb customPunctualColor;

    bool drawOutlier;
    bool drawOutOfScale;
    
    TRangeFunctions rangeFunctions;
    TRangeFunctions negativeRangeFunctions;
    TRangeFunctions alternativeRangeFunctions;
    std::array< rgb, 2 > stopColors;
    std::array< rgb, 2 > negativeStopColors;
    std::vector< rgb > alternativeStopColors;
    rgb aboveOutlierColor;
    rgb belowOutlierColor;

    double redStep;
    double greenStep;
    double blueStep;

    double negativeRedStep;
    double negativeGreenStep;
    double negativeBlueStep;

    TGradientFunction function;

    void initGradient();
    void recalcSteps();

    inline rgb getColor( PRV_UINT32 pos ) const;
    void expandColors();

    rgb calcCodeColor( TSemanticValue whichValue,
                       TSemanticValue minimum,
                       TSemanticValue maximum ) const;

    rgb calcGradientColor( TSemanticValue whichValue,
                           TSemanticValue minimum,
                           TSemanticValue maximum ) const;

};


class Normalizer
{
  public:
    static double calculate( TSemanticValue whichValue,
                             TSemanticValue whichMinimum,
                             TSemanticValue whichMaximum,
                             TGradientFunction whichFunction,
                             bool minimumAsBase );
  private:
    static PRV_INT16 numSteps;

};



