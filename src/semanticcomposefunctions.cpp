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


#include "semanticcomposefunctions.h"
#include "kwindow.h"

using namespace std;

string ComposeAsIs::name = "As Is";
TSemanticValue ComposeAsIs::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return myInfo->values[ 0 ];
}


string ComposeSign::name = "Sign";
TSemanticValue ComposeSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  if ( myInfo->values[ 0 ] != 0 )
    return 1;
  return 0;
}


string ComposeUnsign::name = "1-Sign";
TSemanticValue ComposeUnsign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp;

  if ( myInfo->values[ 0 ] != 0 )
    tmp = 1;
  else
    tmp = 0;

  return 1 - tmp;
}


string ComposeMod::name = "Mod";
TSemanticValue ComposeMod::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue remainder = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    divider = parameters[ DIVIDER ][ 0 ];
  else
    divider = 1;

  remainder = fmod( myInfo->values[ 0 ], divider );

  return remainder;
}


string ComposeModPlus1::name = "Mod+1";
TSemanticValue ComposeModPlus1::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue remainder = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    divider = parameters[ DIVIDER ][ 0 ];
  else
    divider = 1;

  remainder = fmod( myInfo->values[ 0 ], divider );

  return remainder + 1;
}


string ComposeDivide::name = "Div";
TSemanticValue ComposeDivide::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue quotient = 0, divider;

  if ( parameters[ DIVIDER ][ 0 ] != 0 )
    quotient = myInfo->values[ 0 ] / parameters[ DIVIDER ][ 0 ];
  else
  {
    divider = 1;
    quotient = fmod( myInfo->values[ 0 ], divider );
  }
  return quotient;
}


string ComposeProduct::name = "Prod";
TSemanticValue ComposeProduct::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] * parameters[ FACTOR ][ 0 ];
}


string ComposeAdding::name = "Add";
TSemanticValue ComposeAdding::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] + parameters[ FACTOR ][ 0 ];
}


string ComposeSubstract::name = "Subs";
TSemanticValue ComposeSubstract::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ] - parameters[ FACTOR ][ 0 ];
}


string ComposeSelectRange::name = "Select Range";
TSemanticValue ComposeSelectRange::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] <= parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = myInfo->values[ 0 ];
  else
    tmp = 0;

  return tmp;
}


string ComposeSelectRangeOpen::name = "Select Range [)";
TSemanticValue ComposeSelectRangeOpen::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] < parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = myInfo->values[ 0 ];
  else
    tmp = 0;

  return tmp;
}


string ComposeIsInRange::name = "Is In Range";
TSemanticValue ComposeIsInRange::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] <= parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = 1;
  else
    tmp = 0;

  return tmp;
}


string ComposeIsInRangeOpen::name = "Is In Range [)";
TSemanticValue ComposeIsInRangeOpen::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  if ( myInfo->values[ 0 ] < parameters[ MAXVALUE ][ 0 ] &&
       myInfo->values[ 0 ] >= parameters[ MINVALUE ][ 0 ] )
    tmp = 1;
  else
    tmp = 0;

  return tmp;
}


string ComposeIsEqual::name = "Is Equal";
TSemanticValue ComposeIsEqual::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  for ( TParamIndex i = 0; i < parameters[ VALUES ].size(); ++i )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = myInfo->values[ 0 ];
      break;
    }
  }

  return tmp;
}


string ComposeIsEqualSign::name = "Is Equal (Sign)";
TSemanticValue ComposeIsEqualSign::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TSemanticValue tmp = 0;

  for ( TParamIndex i = 0; i < parameters[ VALUES ].size(); ++i )
  {
    if ( myInfo->values[ 0 ] == parameters[ VALUES ][ i ] )
    {
      tmp = 1;
      break;
    }
  }

  return tmp;
}


string ComposeFloor::name = "Floor";
TSemanticValue ComposeFloor::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return floor( myInfo->values[ 0 ] );
}


string ComposeCeil::name = "Ceil";
TSemanticValue ComposeCeil::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return ceil( myInfo->values[ 0 ] );
}


#ifdef WIN32
inline double rint(double nr)
{
  double f = floor(nr);
  double c = ceil(nr);
  return (((c-nr) >= (nr-f)) ? f :c);
}
#endif

string ComposeRound::name = "Round";
TSemanticValue ComposeRound::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return rint( myInfo->values[ 0 ] );
}


string ComposeAbs::name = "Abs";
TSemanticValue ComposeAbs::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return abs( myInfo->values[ 0 ] );
}


void ComposeStackedValue::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); ++i )
      myStack.push_back( vector<TSemanticValue>() );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); ++i )
      myStack.push_back( vector<TSemanticValue>() );
  }
}


string ComposeStackedValue::name = "Stacked Val";
TSemanticValue ComposeStackedValue::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    myStack[ tmpOrder ].push_back( myInfo->values[ 0 ] );
  else
  {
    if ( !myStack[ tmpOrder ].empty() )
      myStack[ tmpOrder ].pop_back();
  }

  if ( myStack[ tmpOrder ].empty() )
    return 0;

  return myStack[ tmpOrder ].back();
}


void ComposeInStackedValue::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); ++i )
      myStack.push_back( vector<TSemanticValue>() );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); ++i )
      myStack.push_back( vector<TSemanticValue>() );
  }
}


string ComposeInStackedValue::name = "In Stacked Val";
TSemanticValue ComposeInStackedValue::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    myStack[ tmpOrder ].push_back( myInfo->values[ 0 ] );
  else
  {
    if ( !myStack[ tmpOrder ].empty() )
      myStack[ tmpOrder ].pop_back();
  }

  if ( myStack[ tmpOrder ].empty() )
    return 0;

  return myStack[ tmpOrder ].back() == parameters[ VALUE ][ 0 ] ?
         myStack[ tmpOrder ].back() : 0;
}


void ComposeNestingLevel::init( KWindow *whichWindow )
{
  myStack.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    myStack.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); ++i )
      myStack.push_back( 0 );
  }
  else
  {
    myStack.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); ++i )
      myStack.push_back( 0 );
  }
}


string ComposeNestingLevel::name = "Nesting level";
TSemanticValue ComposeNestingLevel::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();

  if ( myInfo->values[ 0 ] != 0 )
    ++myStack[ tmpOrder ];
  else if( myStack[ tmpOrder ] > 0 )
    --myStack[ tmpOrder ];

  return myStack[ tmpOrder ];
}


void ComposeEnumerate::init( KWindow *whichWindow )
{

}


string ComposeEnumerate::name = "Enumerate";
TSemanticValue ComposeEnumerate::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder newInfoVal = myInfo->callingInterval->getValue();
  if ( myInfo->values[ 0 ] != 0 )
    return ++newInfoVal;

  return newInfoVal;
}

void ComposeAccumulate::init( KWindow *whichWindow )
{
}


string ComposeAccumulate::name = "Accumulate";
TSemanticValue ComposeAccumulate::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->callingInterval->getValue() + myInfo->values[ 0 ];
}




void ComposeDelta::init( KWindow *whichWindow )
{
  semPrevValue.clear();

  if ( whichWindow->getTrace()->totalThreads() >
       whichWindow->getTrace()->totalCPUs() )
  {
    semPrevValue.reserve( whichWindow->getTrace()->totalThreads() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalThreads(); ++i )
    {
      semPrevValue.push_back( 0 );
    }
  }
  else
  {
    semPrevValue.reserve( whichWindow->getTrace()->totalCPUs() );
    for ( TThreadOrder i = 0; i < whichWindow->getTrace()->totalCPUs(); ++i )
    {
      semPrevValue.push_back( 0 );
    }
  }
}


string ComposeDelta::name = "Delta";
TSemanticValue ComposeDelta::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TObjectOrder tmpOrder = myInfo->callingInterval->getOrder();
  TSemanticValue result = 0.0;

  if ( myInfo->values[ 0 ] != semPrevValue[ tmpOrder ] )
    result = myInfo->values[ 0 ] - semPrevValue[ tmpOrder ];

  semPrevValue[ tmpOrder ] = myInfo->values[ 0 ];

  return result;
}


string ComposeBurstTime::name = "Burst Time";
TSemanticValue ComposeBurstTime::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TSemanticValue result = myInfo->callingInterval->getEnd()->getTime() -
                          myInfo->callingInterval->getBegin()->getTime();
  result = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( result );
  return result;
}


string ComposeJoinBursts::name = "Join Bursts";
TSemanticValue ComposeJoinBursts::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  return myInfo->values[ 0 ];
}


string ComposeBeginTime::name = "Begin Time";
TSemanticValue ComposeBeginTime::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TSemanticValue result = myInfo->callingInterval->getBegin()->getTime();
  result = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( result );
  return result;
}


string ComposeEndTime::name = "End Time";
TSemanticValue ComposeEndTime::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  TSemanticValue result = myInfo->callingInterval->getEnd()->getTime();
  result = myInfo->callingInterval->getWindow()->traceUnitsToWindowUnits( result );
  return result;
}


string ComposeTranslate::name = "Translate";
TSemanticValue ComposeTranslate::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;

  TParamValue &myList = parameters[ TRANSLATION_LIST ];
  for ( PRV_UINT16 i = 0; i < myList.size(); i = i + 2 )
  {
    if ( myList[ i ] == myInfo->values[ 0 ] )
    {
      return myList[ i + 1 ];
    }
  }

  return myInfo->values[ 0 ];
}


string ComposeSine::name = "sin";
TSemanticValue ComposeSine::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return sin( myInfo->values[ 0 ] );
}


string ComposeCosine::name = "cos";
TSemanticValue ComposeCosine::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return cos( myInfo->values[ 0 ] );
}


string ComposeSqrtAbs::name = "sqrt (abs)";
TSemanticValue ComposeSqrtAbs::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return sqrt( fabs( myInfo->values[ 0 ] ) );
}


string ComposeArcTan::name = "arc tan";
TSemanticValue ComposeArcTan::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return atan( myInfo->values[ 0 ] );
}


string ComposeLogN::name = "log N";
TSemanticValue ComposeLogN::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return log( myInfo->values[ 0 ] ) / log( parameters[ BASE ][ 0 ] );
}


string ComposeExponential::name = "exp";
TSemanticValue ComposeExponential::execute( const SemanticInfo *info )
{
  const SemanticHighInfo *myInfo = ( const SemanticHighInfo * ) info;
  return exp( myInfo->values[ 0 ] );
}
