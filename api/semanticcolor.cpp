#include "semanticcolor.h"
#include "window.h"

rgb SemanticColor::BACKGROUND = { 0, 0, 0 };

UINT32 SemanticColor::numColors = 49;
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

rgb SemanticColor::beginGradientColor = {   0, 255,   0 };
rgb SemanticColor::endGradientColor   = {   0,   0, 255 };
rgb SemanticColor::aboveOutlierColor  = { 255, 146,  24 };
rgb SemanticColor::belowOutlierColor  = { 207, 207,  68 };

const double SemanticColor::GradientSteps = 513.0;

UINT32 SemanticColor::getNumColors()
{
  return numColors;
}

rgb* SemanticColor::getCodeColors()
{
  return codeColor;
}

rgb SemanticColor::getBeginGradientColor()
{
  return beginGradientColor;
}

rgb SemanticColor::getEndGradientColor()
{
  return endGradientColor;
}

rgb SemanticColor::getAboveOutlierColor()
{
  return aboveOutlierColor;
}

rgb SemanticColor::getBelowOutlierColor()
{
  return belowOutlierColor;
}

// CODECOLOR METHODS
CodeColor::CodeColor( )
{
  rgb* codeColor = SemanticColor::getCodeColors();
  for( UINT32 i = 0; i < SemanticColor::getNumColors(); i++ )
    colors.push_back( codeColor[ i ] );
}

CodeColor::CodeColor( CodeColor& color )
{
  colors = color.colors;
}

CodeColor::~CodeColor()
{}

UINT32 CodeColor::getNumColors() const
{
  return colors.size();
}

rgb CodeColor::getColor( UINT32 pos ) const
{
  return colors[ pos ];
}

void CodeColor::setColor( UINT32 pos, rgb color )
{
  colors[ pos ] = color;
}

void CodeColor::addColor( rgb color )
{
  colors.push_back( color );
}

rgb CodeColor::calcColor( TSemanticValue whichValue,
                                 Window& whichWindow  ) const
{
  if ( whichValue < whichWindow.getMinimumY() ||
       whichValue > whichWindow.getMaximumY() )
    return getColor( 0 ); // IDLE!

  return getColor( static_cast< UINT32 >( whichValue ) );
}

// GRADIENTCOLOR METHODS
GradientColor::GradientColor( )
{
  drawOutlier = true;
  drawOutOfScale = true;

  beginGradientColor = SemanticColor::getBeginGradientColor();
  endGradientColor = SemanticColor::getEndGradientColor();
  aboveOutlierColor = SemanticColor::getAboveOutlierColor();
  belowOutlierColor = SemanticColor::getBelowOutlierColor();
}

GradientColor::GradientColor( GradientColor& color )
{
  drawOutlier = color.drawOutlier;
  drawOutOfScale = color.drawOutOfScale;

  beginGradientColor = color.beginGradientColor;
  endGradientColor = color.endGradientColor;
  aboveOutlierColor = color.aboveOutlierColor;
  belowOutlierColor = color.belowOutlierColor;
}

GradientColor::~GradientColor()
{}

void GradientColor::setBeginGradientColor( rgb color )
{
  beginGradientColor = color;
}

rgb GradientColor::getBeginGradientColor() const
{
  return beginGradientColor;
}

void GradientColor::setEndGradientColor( rgb color )
{
  endGradientColor = color;
}

void GradientColor::selectMinorComponents( rgb color, vector<colorIndex>* components )
{
  ParaverColor cr = color.red;
  ParaverColor cg = color.green;
  ParaverColor cb = color.blue;

  // (1,2,3) and (1,2,2)
  if (( cr < cg ) && ( cr <= cb ))
    components->push_back( RED );
  else if (( cg < cr ) && ( cg <= cb ))
    components->push_back( GREEN );
  else if (( cb < cr ) && ( cb <= cg ))
    components->push_back( BLUE );

  // (2,2,3)
  if (( cr == cg ) && ( cr < cb ))
  {
    components->push_back( RED );
    components->push_back( GREEN );
  }
  else if (( cr == cb ) && ( cr < cg ))
  {
    components->push_back( RED );
    components->push_back( BLUE );
  }
  else if (( cg == cb ) && ( cg < cr ))
  {
    components->push_back( GREEN );
    components->push_back( BLUE );
  }

  // (1,1,1)
  if (( cr == cg ) && ( cr == cb ))
    components->push_back( RED );
}


void GradientColor::setLimitsGradientColor( rgb begin, rgb end )
{
  vector<colorIndex> minorComponentBegin, minorComponentEnd;

  beginGradientColor = begin;
  endGradientColor = end;

  selectMinorComponents( begin, &minorComponentBegin);
  selectMinorComponents( end,   &minorComponentEnd );

  // Select common


  // Decidir menor componente de cada limite
  // Ver si coincide -> si si, ok
  // si no coincide buscar la pareja más alejada del 0 -(negro)

  // Guardar componente(s) descartada(s)

  // Calcular número de paso(s) para cada componente.
  // Guardarlo, además del reparto entre las mismas.

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

inline void GradientColor::allowOutliers( bool activate )
{
  drawOutlier = activate;
}

inline void GradientColor::allowOutOfScale( bool activate )
{
  drawOutOfScale = activate;
}

rgb GradientColor::calcColor( TSemanticValue whichValue, Window& whichWindow ) const
{

  if ( whichValue < whichWindow.getMinimumY() )
  {
    if ( drawOutlier )
      return belowOutlierColor;
    if ( drawOutOfScale )
      return beginGradientColor;
    return SemanticColor::BACKGROUND;
  }

  if ( whichValue > whichWindow.getMaximumY() )
  {
    if ( drawOutlier )
      return aboveOutlierColor;
    if ( drawOutOfScale )
      return endGradientColor;
    return SemanticColor::BACKGROUND;
  }

  // mapear [whichWindow.getMinimumY(),...,whichValue,...,whichWindow.getMinimumY()] -->
  //        [0, SemanticColor::GradientSteps ]

  //return SemanticColor::GradientSteps * whichValue / (whichWindow.getMaximumY() - whichWindow.getMinimumY())


  return SemanticColor::BACKGROUND;
}

