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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef SEMANTICCOLOR_H_INCLUDED
#define SEMANTICCOLOR_H_INCLUDED

#include "paravertypes.h"
#include "paraverkerneltypes.h"

class Window;

class SemanticColor
{
  public:
    enum TColorFunction
    {
      COLOR = 0,
      GRADIENT,
      NOT_NULL_GRADIENT
    };

    static rgb BACKGROUND;
    static rgb FOREGROUND;
    static rgb DEFAULT_LOGICAL_COMMUNICATIONS;
    static rgb DEFAULT_PHYSICAL_COMMUNICATIONS;
    static rgb DEFAULT_BEGIN_GRADIENT_COLOR;
    static rgb DEFAULT_END_GRADIENT_COLOR;
    static rgb DEFAULT_ABOVE_OUTLIER_COLOR;
    static rgb DEFAULT_BELOW_OUTLIER_COLOR;

    static PRV_UINT32 getNumColors();
    static rgb* getCodeColors();

    static rgb getBeginGradientColor();
    static rgb getEndGradientColor();
    static rgb getAboveOutlierColor();
    static rgb getBelowOutlierColor();

    virtual ~SemanticColor()
    {};

    virtual rgb calcColor( TSemanticValue whichValue,
                           TSemanticValue minimum,
                           TSemanticValue maximum ) const = 0;

  private:
    static PRV_UINT32 numColors;
    static rgb codeColor[];

    static rgb beginGradientColor;
    static rgb endGradientColor;
    static rgb aboveOutlierColor;
    static rgb belowOutlierColor;
};

class CodeColor: public SemanticColor
{
  public:
    CodeColor();
    ~CodeColor();

    PRV_UINT32 getNumColors() const;
    void setColor( PRV_UINT32 pos, rgb color );
    rgb calcColor( TSemanticValue whichValue,
                   TSemanticValue minimum,
                   TSemanticValue maximum ) const;

  private:
    vector<rgb> colors;

    inline rgb getColor( PRV_UINT32 pos ) const;
};


class GradientColor: public SemanticColor
{
  public:
    enum TGradientFunction
    {
      LINEAR = 0,
      STEPS,
      LOGARITHMIC,
      EXPONENTIAL
    };

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
    bool getAllowOutliers() const;
    void allowOutOfScale( bool activate );
    bool getAllowOutOfScale() const;

    TGradientFunction getGradientFunction() const;
    void setGradientFunction( TGradientFunction whichFunction );
    PRV_INT16 getNumSteps() const;
    void setNumSteps( PRV_INT16 steps );

    rgb calcColor( TSemanticValue whichValue,
                   TSemanticValue minimum,
                   TSemanticValue maximum ) const;

    void copy( GradientColor &destiny );

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

    TGradientFunction function;
    PRV_INT16 numSteps;

    void recalcSteps();

    inline rgb functionLinear( TSemanticValue whichValue,
                               TSemanticValue minimum,
                               TSemanticValue maximum ) const;
    inline rgb functionSteps( TSemanticValue whichValue,
                              TSemanticValue minimum,
                              TSemanticValue maximum ) const;
    inline rgb functionLog( TSemanticValue whichValue,
                            TSemanticValue minimum,
                            TSemanticValue maximum ) const;
    inline rgb functionExp( TSemanticValue whichValue,
                            TSemanticValue minimum,
                            TSemanticValue maximum ) const;

};

#endif // SEMANTICCOLOR_H_INCLUDED
