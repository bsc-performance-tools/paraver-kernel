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

PRV_UINT32 SemanticColor::numFixedCodeColors = 49;
rgb SemanticColor::fixedCodeColor[ ] =
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


PRV_UINT32 SemanticColor::getFixedNumColors()
{
  return numFixedCodeColors;
}

rgb* SemanticColor::getFixedCodeColors()
{
  return fixedCodeColor;
}

SemanticColor::SemanticColor( const std::vector< rgb >& whichAlternativeGradientColors ) : alternativeStopColors( whichAlternativeGradientColors )
{
  if ( whichAlternativeGradientColors.size() < 2 )
    throw std::logic_error( "SemanticColor: Too few stop colors." );

  rgb* codeColor = SemanticColor::getFixedCodeColors();
  for ( PRV_UINT32 i = 0; i < SemanticColor::getFixedNumColors(); i++ )
    colors.push_back( codeColor[ i ] );
  expandColors();

  initGradient();
}

void SemanticColor::setColorMode( TColorFunction whichMode )
{
  colorMode = whichMode;
  if( colorMode == TColorFunction::GRADIENT )
    allowOutOfScale( true );
  else if( colorMode == TColorFunction::NOT_NULL_GRADIENT )
    allowOutOfScale( false );
}

TColorFunction SemanticColor::getColorMode() const
{
  return colorMode;
}

void SemanticColor::setCodeColorMode()
{
  colorMode = TColorFunction::CODE_COLOR;
}

void SemanticColor::setGradientColorMode()
{
  colorMode = TColorFunction::GRADIENT;
  allowOutOfScale( true );
}

void SemanticColor::setNotNullGradientColorMode()
{
  colorMode = TColorFunction::NOT_NULL_GRADIENT;
  allowOutOfScale( false );
}

void SemanticColor::setAlternativeGradientColorMode()
{
  colorMode = TColorFunction::ALTERNATIVE_GRADIENT;
  allowOutOfScale( false );
}

void SemanticColor::setFunctionLineColorMode()
{
  colorMode = TColorFunction::FUNCTION_LINE;
}

void SemanticColor::setFusedLinesColorMode()
{
  colorMode = TColorFunction::FUSED_LINES;
}

void SemanticColor::setPunctualColorMode()
{
  colorMode = TColorFunction::PUNCTUAL;
}

bool SemanticColor::isCodeColorSet() const
{
  return colorMode == TColorFunction::CODE_COLOR;
}

bool SemanticColor::isGradientColorSet() const
{
  return colorMode == TColorFunction::GRADIENT;
}

bool SemanticColor::isNotNullGradientColorSet() const
{
  return colorMode == TColorFunction::NOT_NULL_GRADIENT;
}

bool SemanticColor::isAlternativeGradientColorSet() const
{
  return colorMode == TColorFunction::ALTERNATIVE_GRADIENT;
}

bool SemanticColor::isFunctionLineColorSet() const
{
  return colorMode == TColorFunction::FUNCTION_LINE;
}

bool SemanticColor::isFusedLinesColorSet() const
{
  return colorMode == TColorFunction::FUSED_LINES;
}

bool SemanticColor::isPunctualColorSet() const
{
  return colorMode == TColorFunction::PUNCTUAL;
}

rgb SemanticColor::calcColor( TSemanticValue whichValue,
                              TSemanticValue minimum,
                              TSemanticValue maximum ) const
{
  switch( colorMode )
  {
    case TColorFunction::CODE_COLOR:
    case TColorFunction::FUSED_LINES:
      return calcCodeColor( whichValue, minimum, maximum );
      break;
    case TColorFunction::GRADIENT:
    case TColorFunction::NOT_NULL_GRADIENT:
    case TColorFunction::ALTERNATIVE_GRADIENT:
    default:
      return calcGradientColor( whichValue, minimum, maximum );
      break;
  }

  return { 0, 0, 0 };
}

// CODECOLOR METHODS
const int SemanticColor::MAX_COLORS = 32000;


PRV_UINT32 SemanticColor::getNumColors() const
{
  return colors.size();
}

rgb SemanticColor::getColor( PRV_UINT32 pos ) const
{
  if( pos == 0 && ParaverConfig::getInstance()->getColorsTimelineUseZero() )
    return ParaverConfig::getInstance()->getColorsTimelineColorZero();
  pos = pos % colors.size();
  return colors[ pos ];
}

void SemanticColor::setColor( PRV_UINT32 whichPos, rgb whichColor )
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

void SemanticColor::setCustomColor( TSemanticValue whichValue, rgb color ) 
{
  customPalette[ whichValue ] = color;
}

bool SemanticColor::existCustomColors() const
{
  return !customPalette.empty() ||
         customBackgroundColor != ParaverConfig::getInstance()->getColorsTimelineBackground() ||
         customAxisColor != ParaverConfig::getInstance()->getColorsTimelineAxis() ||
         customPunctualColor != ParaverConfig::getInstance()->getColorsTimelinePunctual();
}

const std::map<TSemanticValue, rgb>& SemanticColor::getCustomPalette() const
{
  return customPalette;
}

void SemanticColor::setCustomPalette( const std::map<TSemanticValue, rgb>& whichPalette )
{
  customPalette = whichPalette;
}

bool SemanticColor::getUseCustomPalette() const
{
  return useCustomPalette;
}

void SemanticColor::setUseCustomPalette( bool newValue )
{
  useCustomPalette = newValue;
}

void SemanticColor::setCustomBackgroundColor( rgb whichColor )
{
  customBackgroundColor = whichColor;
}

void SemanticColor::setCustomAxisColor( rgb whichColor )
{
  customAxisColor = whichColor;
}

void SemanticColor::setCustomPunctualColor( rgb whichColor )
{
  customPunctualColor = whichColor;
}

rgb SemanticColor::getCustomBackgroundColor() const
{
  return customBackgroundColor;
}

rgb SemanticColor::getCustomAxisColor() const
{
  return customAxisColor;
}

rgb SemanticColor::getCustomPunctualColor() const
{
  return customPunctualColor;
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
    return color.red + ( color.blue * 256 ) + ( color.green * 65536 );
  }
};

constexpr bool findProperColor( const SemanticColor& whichCodeColor, unordered_set<rgb, hashrgb, eqrgb>& insertedColors, rgb& whichColor )
{
  return false;
}

template< typename TComponent, typename ...TArgs >
bool findProperColor( const SemanticColor& whichCodeColor, unordered_set<rgb, hashrgb, eqrgb>& insertedColors, rgb& whichColor, TComponent& currentComponent, TArgs& ...restComponents )
{
  if( !findProperColor( whichCodeColor, insertedColors, whichColor, restComponents... ) )
  {
    TComponent firstValue = currentComponent++;

    while( currentComponent != firstValue )
    {
      if( !whichCodeColor.isColorSimilarToBackground( whichColor ) )
      {
        bool colorInserted = false;
        tie( ignore, colorInserted ) = insertedColors.insert( whichColor );

        if( colorInserted )
          return true;
      }

      if( findProperColor( whichCodeColor, insertedColors, whichColor, restComponents... ) )
        return true;

      ++currentComponent;
    }
    return false;
  }
  return true;
}

void SemanticColor::expandColors()
{
  unsigned int iterations = MAX_COLORS / colors.size() / 3;
  unsigned int numBaseColors = colors.size();
  unordered_set<rgb, hashrgb, eqrgb> insertedColors;
  vector< decltype( colors.size() ) > conflictColorsIndex;

  insertedColors.insert( colors.begin(), colors.end() );

  auto expandComponent = [&]( unsigned int baseColor, auto incrementComponent )
  {
    for( unsigned int iBaseColor = baseColor; iBaseColor < baseColor + numBaseColors; ++iBaseColor )
    {
      if( iBaseColor > colors.size() - 1 )
        break;
      rgb tmpColor = colors[ iBaseColor ];
      incrementComponent( tmpColor );

      bool colorInserted = false;
      tie( ignore, colorInserted ) = insertedColors.insert( tmpColor );

      if( colorInserted && isColorSimilarToBackground( tmpColor ) )
        conflictColorsIndex.push_back( colors.size() );

      if( colorInserted )
        colors.push_back( tmpColor );
    }
  };

  unsigned int baseColor = 1;
  for( unsigned int i = 0; i < iterations; ++i )
  {
    if ( baseColor > colors.size() - 1 )
      baseColor = colors.size() - 1;

    expandComponent( baseColor, []( rgb& c ){ ++c.red; } );
    expandComponent( baseColor, []( rgb& c ){ ++c.green; } );
    expandComponent( baseColor, []( rgb& c ){ ++c.blue; } );

    baseColor += numBaseColors;
  }

  for( auto colorIndex : conflictColorsIndex )
  {
    rgb& currentColor = colors[ colorIndex ];

    if( !findProperColor( *this, insertedColors, currentColor, currentColor.red, currentColor.green, currentColor.blue ) )
      throw std::length_error( "Impossible to find enough palette colors." );
  }
}

bool SemanticColor::isColorSimilarToBackground( rgb whichColor ) const
{
  rgb background = ParaverConfig::getInstance()->getColorsTimelineBackground();

  int luminanceBackground = SemanticColor::getLuminance( background.red, background.green, background.blue );
  int luminanceColor      = SemanticColor::getLuminance( whichColor.red, whichColor.green, whichColor.blue );

  return std::abs( luminanceBackground - luminanceColor ) < 42;
}

rgb SemanticColor::calcCodeColor( TSemanticValue whichValue,
                                  TSemanticValue minimum,
                                  TSemanticValue maximum ) const
{
  if ( whichValue < 0.0 ||
       whichValue < minimum ||
       whichValue > maximum )
    whichValue = 0.0;

  if( useCustomPalette )
  {
    map<TSemanticValue, rgb>::const_iterator itCustom = customPalette.find( whichValue );
    if( itCustom != customPalette.end() )
      return itCustom->second;
  }

  return getColor( static_cast< PRV_UINT32 >( whichValue ) );
}


// GRADIENTCOLOR METHODS
void SemanticColor::setBeginGradientColor( rgb color )
{
  stopColors[ 0 ] = color;
  recalcSteps();
}

rgb SemanticColor::getBeginGradientColor() const
{
  return stopColors.front();
}

void SemanticColor::setEndGradientColor( rgb color )
{
  stopColors[ 1 ] = color;
  recalcSteps();
}

rgb SemanticColor::getEndGradientColor() const
{
  return stopColors.back();
}

void SemanticColor::setNegativeBeginGradientColor( rgb color )
{
  negativeStopColors[ 0 ] = color;
  recalcSteps();
}

rgb SemanticColor::getNegativeBeginGradientColor() const
{
  return negativeStopColors[ 0 ];
}

void SemanticColor::setNegativeEndGradientColor( rgb color )
{
  negativeStopColors[ 1 ] = color;
  recalcSteps();
}

inline rgb SemanticColor::getNegativeEndGradientColor() const
{
  return negativeStopColors[ 1 ];
}

void SemanticColor::setAboveOutlierColor( rgb color )
{
  aboveOutlierColor = color;
}

rgb SemanticColor::getAboveOutlierColor() const
{
  return aboveOutlierColor;
}

void SemanticColor::setBelowOutlierColor( rgb color )
{
  belowOutlierColor = color;
}

rgb SemanticColor::getBelowOutlierColor() const
{
  return belowOutlierColor;
}

void SemanticColor::allowOutliers( bool activate )
{
  drawOutlier = activate;
}

bool SemanticColor::getAllowOutliers() const
{
  return drawOutlier;
}

void SemanticColor::allowOutOfScale( bool activate )
{
  drawOutOfScale = activate;
}

bool SemanticColor::getAllowOutOfScale() const
{
  return drawOutOfScale;
}

TGradientFunction SemanticColor::getGradientFunction() const
{
  return function;
}

void SemanticColor::setGradientFunction( TGradientFunction whichFunction )
{
  function = whichFunction;
}

rgb SemanticColor::calcGradientColor( TSemanticValue whichValue,
                                      TSemanticValue minimum,
                                      TSemanticValue maximum ) const
{
  if ( whichValue == 0 && !drawOutOfScale )
  {
    if( useCustomPalette )
    {
      auto itColorZero = customPalette.find( 0.0 );
      if( itColorZero != customPalette.end() )
        return itColorZero->second;
    }

    if( ParaverConfig::getInstance()->getColorsTimelineUseZero() )
      return ParaverConfig::getInstance()->getColorsTimelineColorZero();
    else
      return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if ( whichValue < minimum )
  {
    if ( drawOutlier && colorMode != TColorFunction::ALTERNATIVE_GRADIENT )
      return belowOutlierColor;
    if ( drawOutOfScale )
      return stopColors[ 0 ];
    return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if ( whichValue > maximum )
  {
    if ( drawOutlier && colorMode != TColorFunction::ALTERNATIVE_GRADIENT )
      return aboveOutlierColor;
    if ( drawOutOfScale )
      return stopColors[ 1 ];
    return ParaverConfig::getInstance()->getColorsTimelineBackground();
  }

  if( maximum == minimum )
    return stopColors[ 0 ];

  std::reference_wrapper<const TRangeFunctions> tmpRangeFunctions = rangeFunctions;
  bool minimumAsBase = true;
  if( colorMode == TColorFunction::ALTERNATIVE_GRADIENT )
    tmpRangeFunctions = alternativeRangeFunctions;
  else if( whichValue < 0.0 && !negativeRangeFunctions.empty() )
  {
    tmpRangeFunctions = negativeRangeFunctions;
    minimumAsBase = false;
  }

  whichValue = Normalizer::calculate( whichValue, minimum, maximum, function, minimumAsBase );

  const auto tmpFunction = ( --tmpRangeFunctions.get().upper_bound( whichValue ) )->second;
  rgb returnColor = tmpFunction( whichValue );

  return returnColor;
}


bool SemanticColor::isColorOutlier( rgb whichColor ) const
{
  if( colorMode != TColorFunction::GRADIENT )
    return false;
  return ( whichColor == belowOutlierColor || whichColor == aboveOutlierColor );
}


void SemanticColor::initGradient()
{
  stopColors[ 0 ] = ParaverConfig::getInstance()->getColorsBeginGradient();
  stopColors[ 1 ] = ParaverConfig::getInstance()->getColorsEndGradient();
  negativeStopColors[ 0 ] = ParaverConfig::getInstance()->getColorsBeginNegativeGradient();
  negativeStopColors[ 1 ] = ParaverConfig::getInstance()->getColorsEndNegativeGradient();

  drawOutOfScale = ParaverConfig::getInstance()->getTimelineColor() != TColorFunction::NOT_NULL_GRADIENT;
  drawOutlier = true;

  aboveOutlierColor = SemanticColor::DEFAULT_ABOVE_OUTLIER_COLOR;
  belowOutlierColor = SemanticColor::DEFAULT_BELOW_OUTLIER_COLOR;

  function = TGradientFunction::STEPS;

  recalcSteps();
}

template< typename Container >
void fillRangeFunctions( SemanticColor::TRangeFunctions& whichRangeFunctions, const Container& whichColors )
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

void SemanticColor::recalcSteps()
{
  fillRangeFunctions( rangeFunctions, stopColors );
  if( !negativeStopColors.empty() )
    fillRangeFunctions( negativeRangeFunctions, negativeStopColors );

  fillRangeFunctions( alternativeRangeFunctions, alternativeStopColors );
}
