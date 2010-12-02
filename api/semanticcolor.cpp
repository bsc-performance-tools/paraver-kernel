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

#include <math.h>
#include <iostream>
#include "semanticcolor.h"
#include "window.h"
#include "paraverconfig.h"

#ifdef WIN32
#define lround floor
#endif

rgb SemanticColor::BACKGROUND = { 0, 0, 0 };
rgb SemanticColor::FOREGROUND = { 255, 255, 255 };
rgb SemanticColor::DEFAULT_LOGICAL_COMMUNICATIONS = { 255, 255, 0 };
rgb SemanticColor::DEFAULT_PHYSICAL_COMMUNICATIONS = { 255, 0, 0 };

PRV_UINT32 SemanticColor::numColors = 49;
rgb SemanticColor::codeColor[ ] =
{
  { 117, 195, 255 }, //  0 - Idle
  {   0,   0, 255 }, //  1 - Running
  { 255, 255, 255 }, //  2 - Not created
  {   0, 170,   0 }, //  3 - Waiting a message
  { 255,   0, 174 }, //  4 - Blocked
  { 179,   0,   0 }, //  5 - Thread Synchronization
  { 0,   255,   0 }, //  6 - Test/Probe
  { 255, 255,   0 }, //  7 - Scheduled and Fork/Join
  { 235,   0,   0 }, //  8 - Wait/Wait all
  {   0, 162,   0 }, //  9 - Blocked
  { 255,   0, 255 }, // 10 - Immediate Send
  { 100, 100, 177 }, // 11 - Immediate Receive
  { 172, 174,  41 }, // 12 - I/O
  { 255, 144,  26 }, // 13 - Group communication
  {   2, 255, 177 }, // 14 - Tracing Disabled
  { 192, 224,   0 }, // 15 - Overhead
  {  66,  66,  66 }, // 16 - Not used
  { 189, 168, 100 }, // 17 - Not used
  {  95, 200,   0 }, // 18 - Not used
  { 203,  60,  69 }, // 19 - Not used
  {   0, 109, 255 }, // 20 - Not used
  { 200,  61,  68 }, // 21 - Not used
  { 200,  66,   0 }, // 22 - Not used
  {   0,  41,   0 }, // 23 - Not used
  { 139, 121, 177 }, // 24 - Not used
  { 116, 116, 116 }, // 25 - Not used
  { 200,  50,  89 }, // 26 - Not used
  { 255, 171,  98 }, // 27 - Not used
  {   0,  68, 189 }, // 28 - Not used
  {  52,  43,   0 }, // 29 - Not used
  { 255,  46,   0 }, // 30 - Not used
  { 100, 216,  32 }, // 31 - Not used
  {   0,   0, 112 }, // 32 - Not used
  { 105, 105,   0 }, // 33 - Not used
  { 132,  75, 255 }, // 34 - Not used
  { 184, 232,   0 }, // 35 - Not used
  {   0, 109, 112 }, // 36 - Not used
  { 189, 168, 100 }, // 37 - Not used
  { 132,  75,  75 }, // 38 - Not used
  { 255,  75,  75 }, // 39 - Not used
  { 255,  20,   0 }, // 40 - Not used
  {  52,   0,   0 }, // 41 - Not used
  {   0,  66,   0 }, // 42 - Not used
  { 184, 132,   0 }, // 43 - Not used
  { 100,  16,  32 }, // 44 - Not used
  { 146, 255, 255 }, // 45 - Not used
  {   0,  23,  37 }, // 46 - Not used
  { 146,   0, 255 }, // 47 - Not used
  {   0, 138, 119 }  // 48 - Not used
};

// Program defaults
rgb SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR = {   0, 255,   0 };
rgb SemanticColor::DEFAULT_END_GRADIENT_COLOR   = {   0,   0, 255 };
rgb SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR  = { 255, 146,  24 };
rgb SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR  = { 207, 207,  68 };

// Preferences may change these
rgb SemanticColor::beginGradientColor = SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR;
rgb SemanticColor::endGradientColor   = SemanticColor::DEFAULT_END_GRADIENT_COLOR;
rgb SemanticColor::aboveOutlierColor  = SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR;
rgb SemanticColor::belowOutlierColor  = SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR;

PRV_UINT32 SemanticColor::getNumColors()
{
  return numColors;
}

rgb* SemanticColor::getCodeColors()
{
  return codeColor;
}

rgb SemanticColor::getBeginGradientColor()
{
  return ParaverConfig::getInstance()->getColorsBeginGradient();
}

rgb SemanticColor::getEndGradientColor()
{
  return ParaverConfig::getInstance()->getColorsEndGradient();
}

rgb SemanticColor::getAboveOutlierColor()
{
  return ParaverConfig::getInstance()->getColorsTopGradient();
}

rgb SemanticColor::getBelowOutlierColor()
{
  return ParaverConfig::getInstance()->getColorsLowGradient();
}

// CODECOLOR METHODS
CodeColor::CodeColor( )
{
  rgb* codeColor = SemanticColor::getCodeColors();
  for ( PRV_UINT32 i = 0; i < SemanticColor::getNumColors(); i++ )
    colors.push_back( codeColor[ i ] );
}

CodeColor::~CodeColor()
{}

PRV_UINT32 CodeColor::getNumColors() const
{
  return colors.size();
}

rgb CodeColor::getColor( PRV_UINT32 pos ) const
{
  if( pos == 0 && ParaverConfig::getInstance()->getColorsTimelineUseZero() )
    return ParaverConfig::getInstance()->getColorsTimelineColorZero();
  pos = pos % colors.size();
  return colors[ pos ];
}

void CodeColor::setColor( PRV_UINT32 pos, rgb color )
{
  if ( pos >= colors.size() )
  {
    while( pos >= colors.size() )
    {
      vector<rgb>::iterator pos = colors.end(); --pos;
      colors.insert( pos, colors.begin(), colors.end() );
    }
  }
  colors[ pos ] = color;
}

rgb CodeColor::calcColor( TSemanticValue whichValue,
                          TSemanticValue minimum,
                          TSemanticValue maximum ) const
{
  if ( whichValue < minimum ||
       whichValue > maximum )
    return getColor( 0 ); // IDLE!

  return getColor( static_cast< PRV_UINT32 >( whichValue ) );
}

bool CodeColor::calcValue( rgb whichColor, TSemanticValue& returnValue ) const
{
  bool found = false;
  returnValue = 0;

  for( PRV_UINT32 i = 0; i < colors.size(); ++i )
  {
    if( whichColor == colors[ i ] )
    {
      returnValue = i;
      found = true;
      break;
    }
  }

  return found;
}

// GRADIENTCOLOR METHODS
GradientColor::GradientColor( )
{
  bool blackNotNull = ParaverConfig::getInstance()->getTimelineColor() != SemanticColor::NOT_NULL_GRADIENT;

  drawOutlier = true;
  drawOutOfScale = blackNotNull;

  beginGradientColor = SemanticColor::getBeginGradientColor();
  endGradientColor   = SemanticColor::getEndGradientColor();
  aboveOutlierColor  = SemanticColor::getAboveOutlierColor();
  belowOutlierColor  = SemanticColor::getBelowOutlierColor();

  function = GradientColor::STEPS;
  numSteps = 10;

  recalcSteps();
}

GradientColor::~GradientColor()
{}

void GradientColor::setBeginGradientColor( rgb color )
{
  beginGradientColor = color;
  recalcSteps();
}

rgb GradientColor::getBeginGradientColor() const
{
  return beginGradientColor;
}

void GradientColor::setEndGradientColor( rgb color )
{
  endGradientColor = color;
  recalcSteps();
}

inline rgb GradientColor::getEndGradientColor() const
{
  return endGradientColor;
}

inline void GradientColor::setAboveOutlierColor( rgb color )
{
  aboveOutlierColor = color;
}

inline rgb GradientColor::getAboveOutlierColor() const
{
  return aboveOutlierColor;
}

inline void GradientColor::setBelowOutlierColor( rgb color )
{
  belowOutlierColor = color;
}

inline rgb GradientColor::getBelowOutlierColor() const
{
  return belowOutlierColor;
}

void GradientColor::allowOutliers( bool activate )
{
  drawOutlier = activate;
}

bool GradientColor::getAllowOutliers() const
{
  return drawOutlier;
}

void GradientColor::allowOutOfScale( bool activate )
{
  drawOutOfScale = activate;
}

bool GradientColor::getAllowOutOfScale() const
{
  return drawOutOfScale;
}

GradientColor::TGradientFunction GradientColor::getGradientFunction() const
{
  return function;
}

void GradientColor::setGradientFunction( TGradientFunction whichFunction )
{
  function = whichFunction;
}

PRV_INT16 GradientColor::getNumSteps() const
{
  return numSteps;
}

void GradientColor::setNumSteps( PRV_INT16 steps )
{
  numSteps = steps;
}

rgb GradientColor::calcColor( TSemanticValue whichValue,
                              TSemanticValue minimum,
                              TSemanticValue maximum ) const
{
  if ( whichValue == 0 && !drawOutOfScale )
  {
    if( ParaverConfig::getInstance()->getColorsTimelineUseZero() )
      return ParaverConfig::getInstance()->getColorsTimelineColorZero();
    else
      return SemanticColor::BACKGROUND;
  }

  if ( whichValue < minimum )
  {
    if ( drawOutlier )
      return belowOutlierColor;
    if ( drawOutOfScale )
      return beginGradientColor;
    return SemanticColor::BACKGROUND;
  }

  if ( whichValue > maximum )
  {
    if ( drawOutlier )
      return aboveOutlierColor;
    if ( drawOutOfScale )
      return endGradientColor;
    return SemanticColor::BACKGROUND;
  }

  TSemanticValue norm = ( whichValue - minimum ) /
                        ( maximum - minimum );

  rgb returnColor;

  switch ( function )
  {
    case LINEAR:
      returnColor = functionLinear( norm, minimum, maximum );
      break;

    case STEPS:
      returnColor = functionSteps( norm, minimum, maximum );
      break;

    case LOGARITHMIC:
      returnColor = functionLog( norm, minimum, maximum );
      break;

    case EXPONENTIAL:
      returnColor = functionExp( norm, minimum, maximum );
      break;
  }

  return returnColor;
}

bool GradientColor::calcValue( rgb whichColor, TSemanticValue& returnValue ) const
{
  returnValue = 0.0;
  return true;
}

void GradientColor::recalcSteps()
{
  redStep = ( endGradientColor.red - beginGradientColor.red );
  greenStep = ( endGradientColor.green - beginGradientColor.green );
  blueStep = ( endGradientColor.blue - beginGradientColor.blue );
}

void GradientColor::copy( GradientColor &destiny )
{
  destiny.drawOutlier = drawOutlier;
  destiny.drawOutOfScale = drawOutOfScale;

  destiny.beginGradientColor = beginGradientColor;
  destiny.endGradientColor = endGradientColor;
  destiny.aboveOutlierColor = aboveOutlierColor;
  destiny.belowOutlierColor = belowOutlierColor;

  destiny.redStep = redStep;
  destiny.greenStep = greenStep;
  destiny.blueStep = blueStep;
}

rgb GradientColor::functionLinear( TSemanticValue whichValue,
                                   TSemanticValue minimum,
                                   TSemanticValue maximum ) const
{
  rgb tmpColor = beginGradientColor;

  tmpColor.red += ( ParaverColor ) lround( redStep * whichValue );
  tmpColor.green += ( ParaverColor ) lround( greenStep * whichValue );
  tmpColor.blue += ( ParaverColor ) lround( blueStep * whichValue );

  return tmpColor;
}

rgb GradientColor::functionSteps( TSemanticValue whichValue,
                                  TSemanticValue minimum,
                                  TSemanticValue maximum ) const
{
  rgb tmpColor = beginGradientColor;

  double stepNorm = floor( numSteps * whichValue ) / numSteps;
  tmpColor.red += ( ParaverColor ) lround( redStep * stepNorm );
  tmpColor.green += ( ParaverColor ) lround( greenStep * stepNorm );
  tmpColor.blue += ( ParaverColor ) lround( blueStep * stepNorm );

  return tmpColor;
}

rgb GradientColor::functionLog( TSemanticValue whichValue,
                                TSemanticValue minimum,
                                TSemanticValue maximum ) const
{
  rgb tmpColor = beginGradientColor;

  double stepNorm = log( (double)(whichValue * 100 + 1) ) / log( (double)101 );
  tmpColor.red += ( ParaverColor ) lround( redStep * stepNorm );
  tmpColor.green += ( ParaverColor ) lround( greenStep * stepNorm );
  tmpColor.blue += ( ParaverColor ) lround( blueStep * stepNorm );

  return tmpColor;
}

rgb GradientColor::functionExp( TSemanticValue whichValue,
                                TSemanticValue minimum,
                                TSemanticValue maximum ) const
{
  rgb tmpColor = beginGradientColor;

  double stepNorm = exp( (double)(whichValue * 10) ) / exp( (double)10 );
  tmpColor.red += ( ParaverColor ) lround( redStep * stepNorm );
  tmpColor.green += ( ParaverColor ) lround( greenStep * stepNorm );
  tmpColor.blue += ( ParaverColor ) lround( blueStep * stepNorm );

  return tmpColor;
}
