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


#include <cmath>
#include <math.h>
#include <iostream>
#include  <unordered_set>
#include "semanticcolor.h"
#include "window.h"
#include "paraverconfig.h"

using namespace std;

PRV_INT16 Normalizer::numSteps = 10;

double Normalizer::calculate( TSemanticValue whichValue,
                              TSemanticValue whichMinimum,
                              TSemanticValue whichMaximum,
                              TGradientFunction whichFunction,
                              bool minimumAsBase )
{
  TSemanticValue norm;  

  if( whichMinimum >= 0.0 || minimumAsBase ) 
    norm = ( whichValue - whichMinimum ) / ( whichMaximum - whichMinimum );
  else
  {
    if ( whichMaximum < 0.0 )
    {
      norm = ( whichValue - whichMinimum ) / ( whichMaximum - whichMinimum );
    }
    else
    {
       // Normalize using only positive/negative scale applying its own different color palettes
      if( whichValue >= 0.0 )
        norm = whichValue / whichMaximum;
      else
        norm = whichValue / whichMinimum;
    }    
  }

  switch ( whichFunction )
  {
    case TGradientFunction::LINEAR:
      break;

    case TGradientFunction::STEPS:
      norm = floor( Normalizer::numSteps * norm ) / Normalizer::numSteps;
      break;

    case TGradientFunction::LOGARITHMIC:
      norm = log( ( double )( norm * 100 + 1 ) ) / log( ( double )101 );
      break;

    case TGradientFunction::EXPONENTIAL:
      norm = exp( ( double )( norm * 10 ) ) / exp( ( double )10 );
      break;
  }

  return norm;
}

rgb SemanticColor::BACKGROUND = { 0, 0, 0 };
rgb SemanticColor::FOREGROUND = { 255, 255, 255 };
rgb SemanticColor::ZERO_AXIS  = { 127, 127, 127 };

rgb SemanticColor::DEFAULT_PUNCTUAL = { 255, 128, 128 };
rgb SemanticColor::DEFAULT_LOGICAL_COMMUNICATIONS = { 255, 255, 0 };
rgb SemanticColor::DEFAULT_PHYSICAL_COMMUNICATIONS = { 255, 0, 0 };

PRV_UINT32 SemanticColor::numColors = 49;
rgb SemanticColor::codeColor[ ] =
{
  { 117, 195, 255 }, //  0 - Idle
  {   0,   0, 255 }, //  1 - Running
  { 255, 255, 255 }, //  2 - Not created
  { 255,   0,   0 }, //  3 - Waiting a message
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
  {  10,  66,  40 }, // 23 - Not used
  { 139, 121, 177 }, // 24 - Not used
  { 116, 116, 116 }, // 25 - Not used
  { 200,  50,  89 }, // 26 - Not used
  { 255, 171,  98 }, // 27 - Not used
  {   0,  68, 189 }, // 28 - Not used
  {  66,  66,   0 }, // 29 - Not used
  { 255,  46,   0 }, // 30 - Not used
  { 100, 216,  32 }, // 31 - Not used
  {   0,  50, 120 }, // 32 - Not used
  { 105, 105,   0 }, // 33 - Not used
  { 132,  75, 255 }, // 34 - Not used
  { 184, 232,   0 }, // 35 - Not used
  {   0, 109, 112 }, // 36 - Not used
  { 189, 168, 100 }, // 37 - Not used
  { 132,  75,  75 }, // 38 - Not used
  { 255,  75,  75 }, // 39 - Not used
  { 255,  20,   0 }, // 40 - Not used
  { 130,  30,  30 }, // 41 - Not used
  {  10,  66,  10 }, // 42 - Not used
  { 184, 132,   0 }, // 43 - Not used
  { 100,  16,  32 }, // 44 - Not used
  { 146, 255, 255 }, // 45 - Not used
  {  50,  50, 100 }, // 46 - Not used
  { 146,   0, 255 }, // 47 - Not used
  {   0, 138, 119 }  // 48 - Not used
};

// Program defaults
rgb SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR          = {   0, 255,   0 };
rgb SemanticColor::DEFAULT_END_GRADIENT_COLOR            = {   0,   0, 255 };
rgb SemanticColor::DEFAULT_NEGATIVE_BEGIN_GRADIENT_COLOR = { 160, 255, 160 };
rgb SemanticColor::DEFAULT_NEGATIVE_END_GRADIENT_COLOR   = { 160, 160, 255 };
rgb SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR           = { 255, 146,  24 };
rgb SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR           = { 207, 207,  68 };

// Preferences may change these
rgb SemanticColor::beginGradientColor         = SemanticColor::DEFAULT_BEGIN_GRADIENT_COLOR;
rgb SemanticColor::endGradientColor           = SemanticColor::DEFAULT_END_GRADIENT_COLOR;
rgb SemanticColor::negativeBeginGradientColor = SemanticColor::DEFAULT_NEGATIVE_BEGIN_GRADIENT_COLOR;
rgb SemanticColor::negativeEndGradientColor   = SemanticColor::DEFAULT_NEGATIVE_END_GRADIENT_COLOR;
rgb SemanticColor::aboveOutlierColor          = SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR;
rgb SemanticColor::belowOutlierColor          = SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR;

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

rgb SemanticColor::getNegativeBeginGradientColor()
{
  return ParaverConfig::getInstance()->getColorsBeginNegativeGradient();
}

rgb SemanticColor::getNegativeEndGradientColor()
{
  return ParaverConfig::getInstance()->getColorsEndNegativeGradient();
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
const int CodeColor::MAX_COLORS = 32000;

CodeColor::CodeColor( )
{
  rgb* codeColor = SemanticColor::getCodeColors();
  for ( PRV_UINT32 i = 0; i < SemanticColor::getNumColors(); i++ )
    colors.push_back( codeColor[ i ] );
  expandColors();
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

void CodeColor::setColor( PRV_UINT32 whichPos, rgb whichColor )
{
  if ( whichPos >= colors.size() )
  {
    while( whichPos >= colors.size() )
    {
      vector<rgb>::iterator lastColorPos = colors.end();
      --lastColorPos;
      colors.insert( lastColorPos, colors.begin(), colors.end() );
    }
  }
  colors[ whichPos ] = whichColor;
}

void CodeColor::setCustomColor( TSemanticValue whichValue, rgb color ) 
{
  customPalette[ whichValue ] = color;
}

bool CodeColor::existCustomColors() const
{
  return !customPalette.empty();
}

const std::map<TSemanticValue, rgb>& CodeColor::getCustomPalette() const
{
  return customPalette;
}

void CodeColor::setCustomPalette( const std::map<TSemanticValue, rgb>& whichPalette )
{
  customPalette = whichPalette;
}

struct eqrgb
{
  bool operator()( rgb color1, rgb color2 ) const
  {
    return color1 == color2;
  }
};

struct hashrgb
{
  size_t operator()( rgb color ) const
  {
    return color.red + ( color.blue * 256 ) + (color.green * 65536 );
  }
};


void CodeColor::expandColors()
{
  unsigned int iterations = MAX_COLORS / colors.size() / 3;
  unsigned int numBaseColors = colors.size();
  unordered_set<rgb, hashrgb, eqrgb> insertedColors;

  insertedColors.insert( colors.begin(), colors.end() );

  unsigned int baseColor = 1;
  for( unsigned int i = 0; i < iterations; ++i )
  {
    if ( baseColor > colors.size() - 1 )
      baseColor = colors.size() - 1;

    auto expandComponent = [&]( auto incrementComponent )
    {
      for( unsigned int iBaseColor = baseColor; iBaseColor < baseColor + numBaseColors; ++iBaseColor )
      {
        if( iBaseColor > colors.size() - 1 )
          break;
        rgb tmpColor = colors[ iBaseColor ];
        incrementComponent( tmpColor );

        if( !isColorSimilarToBackground( tmpColor ) )
        {
          pair<unordered_set<rgb, hashrgb, eqrgb>::iterator, bool > result = insertedColors.insert( tmpColor );

          if( result.second )
            colors.push_back( tmpColor );
        }
      }
    };

    expandComponent( []( rgb& c ){ ++c.red; } );
    expandComponent( []( rgb& c ){ ++c.green; } );
    expandComponent( []( rgb& c ){ ++c.blue; } );

    baseColor += numBaseColors;
  }
}

bool CodeColor::isColorSimilarToBackground( rgb whichColor ) const
{
  rgb background = ParaverConfig::getInstance()->getColorsTimelineBackground();

  int luminanceBackground = SemanticColor::getLuminance( background.red, background.green, background.blue );
  int luminanceColor      = SemanticColor::getLuminance( whichColor.red, whichColor.green, whichColor.blue );

  return std::abs( luminanceBackground - luminanceColor ) < 42;
}

rgb CodeColor::calcColor( TSemanticValue whichValue,
                          TSemanticValue minimum,
                          TSemanticValue maximum,
                          bool useCustomPalette ) const
{
  if ( whichValue < 0.0 ||
       whichValue < minimum ||
       whichValue > maximum )
    return getColor( 0 ); // IDLE!

  if( useCustomPalette )
  {
    map<TSemanticValue, rgb>::const_iterator itCustom = customPalette.find( whichValue );
    if( itCustom != customPalette.end() )
      return itCustom->second;
  }

  return getColor( static_cast< PRV_UINT32 >( whichValue ) );
}


// GRADIENTCOLOR METHODS
GradientColor::GradientColor()
{
  stopColors.push_back( SemanticColor::getBeginGradientColor() );
  stopColors.push_back( SemanticColor::getEndGradientColor() );
  negativeStopColors.push_back( SemanticColor::getNegativeBeginGradientColor() ); 
  negativeStopColors.push_back( SemanticColor::getNegativeEndGradientColor() ); 

  initCommon();
  drawOutlier = true;
}

GradientColor::GradientColor( const std::vector< rgb >& whichStopColors ): stopColors( whichStopColors )
{
  if ( whichStopColors.size() < 2 )
    throw std::logic_error( "GradientColor: Too few stop colors." );
  initCommon();
  drawOutlier = false;
}


GradientColor::~GradientColor()
{}

void GradientColor::setBeginGradientColor( rgb color )
{
  stopColors[ 0 ] = color;
  recalcSteps();
}

rgb GradientColor::getBeginGradientColor() const
{
  return stopColors.front();
}

void GradientColor::setEndGradientColor( rgb color )
{
  stopColors[ 1 ] = color;
  recalcSteps();
}

rgb GradientColor::getEndGradientColor() const
{
  return stopColors.back();
}

void GradientColor::setNegativeBeginGradientColor( rgb color )
{
  negativeStopColors[ 0 ] = color;
  recalcSteps();
}

rgb GradientColor::getNegativeBeginGradientColor() const
{
  return negativeStopColors[ 0 ];
}

void GradientColor::setNegativeEndGradientColor( rgb color )
{
  negativeStopColors[ 1 ] = color;
  recalcSteps();
}

inline rgb GradientColor::getNegativeEndGradientColor() const
{
  return negativeStopColors[ 1 ];
}

void GradientColor::setAboveOutlierColor( rgb color )
{
  aboveOutlierColor = color;
}

rgb GradientColor::getAboveOutlierColor() const
{
  return aboveOutlierColor;
}

void GradientColor::setBelowOutlierColor( rgb color )
{
  belowOutlierColor = color;
}

rgb GradientColor::getBelowOutlierColor() const
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

TGradientFunction GradientColor::getGradientFunction() const
{
  return function;
}

void GradientColor::setGradientFunction( TGradientFunction whichFunction )
{
  function = whichFunction;
}

rgb GradientColor::calcColor( TSemanticValue whichValue,
                              TSemanticValue minimum,
                              TSemanticValue maximum,
                              bool useCustomPalette ) const
{
  if ( whichValue == 0 && !drawOutOfScale )
  {
    if( ParaverConfig::getInstance()->getColorsTimelineUseZero() )
      return ParaverConfig::getInstance()->getColorsTimelineColorZero();
    else
      return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if ( whichValue < minimum )
  {
    if ( drawOutlier )
      return belowOutlierColor;
    if ( drawOutOfScale )
      return stopColors[ 0 ];
    return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if ( whichValue > maximum )
  {
    if ( drawOutlier )
      return aboveOutlierColor;
    if ( drawOutOfScale )
      return stopColors[ 1 ];
    return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if( maximum == minimum )
    return stopColors[ 0 ];

  std::reference_wrapper<const TRangeFunctions> tmpRangeFunctions = rangeFunctions;
  bool minimumAsBase = true;
  if( whichValue < 0.0 && !negativeRangeFunctions.empty() )
  {
    tmpRangeFunctions = negativeRangeFunctions;
    minimumAsBase = false;
  }

  whichValue = Normalizer::calculate( whichValue, minimum, maximum, function, minimumAsBase );

  const auto tmpFunction = ( --tmpRangeFunctions.get().upper_bound( whichValue ) )->second;
  rgb returnColor = tmpFunction( whichValue );

  return returnColor;
}


bool GradientColor::isColorOutlier( rgb whichColor ) const
{
  return ( whichColor == belowOutlierColor || whichColor == aboveOutlierColor );
}


void GradientColor::initCommon()
{
  bool blackNotNull = ParaverConfig::getInstance()->getTimelineColor() != TColorFunction::NOT_NULL_GRADIENT;

  drawOutOfScale = blackNotNull;

  aboveOutlierColor = SemanticColor::getAboveOutlierColor();
  belowOutlierColor = SemanticColor::getBelowOutlierColor();

  function = TGradientFunction::STEPS;

  recalcSteps();
}


void fillRangeFunctions( GradientColor::TRangeFunctions& whichRangeFunctions, const std::vector< rgb >& whichColors )
{
  whichRangeFunctions.clear();
  double index = 0.0;
  const double indexStep = 1.0 / (double)( whichColors.size() - 1 );
  for( auto itColor = whichColors.begin(); itColor != whichColors.end() - 1; ++itColor )
  {
    double tmpRedStep = ( (double)( itColor + 1 )->red - (double)itColor->red );
    double tmpGreenStep = ( (double)( itColor + 1 )->green - (double)itColor->green );
    double tmpBlueStep = ( (double)( itColor + 1 )->blue - (double)itColor->blue );
    whichRangeFunctions[ index ] =
      [ red = tmpRedStep, green = tmpGreenStep, blue = tmpBlueStep, beginColor = *itColor, index, rangeWidth = indexStep ]( TSemanticValue whichValue )
      {
        rgb returnColor{ beginColor };
        const double normValue = ( ( whichValue - index ) / rangeWidth );

        returnColor.red += floor( red * normValue );
        returnColor.green += floor( green * normValue );
        returnColor.blue += floor( blue * normValue );

        return returnColor;
      };

    index += indexStep;
  }
}

void GradientColor::recalcSteps()
{
  fillRangeFunctions( rangeFunctions, stopColors );
  if( !negativeStopColors.empty() )
    fillRangeFunctions( negativeRangeFunctions, negativeStopColors );
}

void GradientColor::copy( GradientColor &destiny )
{
  destiny.drawOutlier = drawOutlier;
  destiny.drawOutOfScale = drawOutOfScale;

  destiny.stopColors = stopColors;
  destiny.negativeStopColors = negativeStopColors;
  destiny.aboveOutlierColor = aboveOutlierColor;
  destiny.belowOutlierColor = belowOutlierColor;

  destiny.redStep   = redStep;
  destiny.greenStep = greenStep;
  destiny.blueStep  = blueStep;
  destiny.negativeRedStep   = negativeRedStep;
  destiny.negativeGreenStep = negativeGreenStep;
  destiny.negativeBlueStep   = negativeBlueStep;
}

