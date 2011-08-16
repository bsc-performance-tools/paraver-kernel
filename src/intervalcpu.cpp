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

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "kwindow.h"
#include "intervalcpu.h"


KRecordList *IntervalCPU::init( TRecordTime initialTime, TCreateList create,
                                KRecordList *displayList )
{
  createList = create;
  currentValue = 0.0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  function = ( SemanticCPU * ) window->getSemanticFunction( level );
  functionThread = ( SemanticThread * ) window->getSemanticFunction( THREAD );
  functionComposeThread = ( SemanticCompose * ) window->getSemanticFunction( COMPOSETHREAD );

  if ( begin != NULL )
    delete begin;
  if ( end != NULL )
    delete end;

  threadState.clear();
  threadState.insert( threadState.begin(), window->getTrace()->totalThreads(), 0.0 );
// se necesitan los dos: getCPURecordByTime para el recorrido dentro del intervalo
// de CPU, y getThreadRecordByTime para los recorridos de los intervalos de thread
// !!! Para cada CPU hay que reiniciar los intervalos de thread que le tocan por si
//     acaso estamos dibujando por CPU !!!
// VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
// !!! Aunque quizas seria mejor crear en cada intervalo de CPU todos sus posibles
//     intervalos de thread para que no hubiera conflicto entre ellos !!!
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Por culpa de funciones como Stacked se han de copiar (clonar) las funciones semanticas
// de los compose thread y thread en cada intervalo de CPU y que se asignen directamente
// a esos intervalos, debiendo cambiar los constructores o los init para que admitan
// esa posibilidad. No deberia haber ningun problema por usar el constructor por copia
// por defecto.
// Ademas en los intervalos de compose tambien se tiene que poder
// pàsar los intervalos de thread para que no use los de la ventana.
// El problema puede estar en la copia de iteradores que se hace en los compose,
// ya que mira si la ventana esta a nivel de recursos o procesos. Quizas hay que
// meter otra opcion que sea si el intervalo por debajo es thread que siempre copie
// los iteradores por procesos.
// Luego en la CPU (aqui) solo se miraran los tiempos y se usaran los propios iteradores
// de CPU para delimitar el intervalo semantico.
  begin = window->copyCPUIterator( window->getThreadRecordByTime( order ) );
  end = window->copyCPUIterator( begin );

  if ( ( !function->getInitFromBegin() ) && ( !functionThread->getInitFromBegin() ) &&
       ( !functionComposeThread->getInitFromBegin() ) && ( initialTime > 0.0 ) )
    calcPrev( displayList, true );

  calcNext( displayList, true );
  while ( ( !end->isNull() ) && ( end->getTime() <= initialTime ) )
    calcNext( displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcNext( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;
  SemanticThreadInfo threadInfo;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *begin = *end;
  }

  threadInfo.callingInterval = getWindowInterval( THREAD, begin->getOrder() );
  highInfo.callingInterval = this;
  threadInfo.it = begin;
  if( !( window->passFilter( begin ) && functionThread->validRecord( begin ) ) )
    highInfo.values.push_back( threadState[ begin->getThread() ] );
  else if( begin->getType() == STATE + END )
    highInfo.values.push_back( 0.0 );
  else
  {
    SemanticHighInfo tmpHighInfo;
    tmpHighInfo.callingInterval = getWindowInterval( COMPOSETHREAD, begin->getThread() );
    tmpHighInfo.values.push_back( functionThread->execute( &threadInfo ) );
    threadState[ begin->getThread() ] = functionComposeThread->execute( &tmpHighInfo );
    highInfo.values.push_back( threadState[ begin->getThread() ] );
  }
  currentValue = function->execute( &highInfo );
  end = getNextRecord( end, displayList );

  return displayList;
}


KRecordList *IntervalCPU::calcPrev( KRecordList *displayList, bool initCalc )
{
  SemanticHighInfo highInfo;
  SemanticThreadInfo threadInfo;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  if ( !initCalc )
  {
    *end = *begin;
  }

  begin = getPrevRecord( begin, displayList );
  highInfo.callingInterval = this;
  threadInfo.callingInterval = this;
  threadInfo.it = begin;
  highInfo.values.push_back( functionThread->execute( &threadInfo ) );
  currentValue = function->execute( &highInfo );

  if ( initCalc )
  {
    *end = *begin;
  }


  return displayList;
}


MemoryTrace::iterator *IntervalCPU::getNextRecord( MemoryTrace::iterator *it,
    KRecordList *displayList )
{
  TThreadOrder threadOrder = it->getThread();
  ++( *it );
  while ( !it->isNull() )
  {
    if ( window->passFilter( it ) )
    {
      if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
           ||
           ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if ( functionThread->validRecord( it ) )
        break;
    }
    if( it->getThread() != threadOrder )
      break;
    ++( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getCPUEndRecord( order );
  }

  return it;
}


MemoryTrace::iterator *IntervalCPU::getPrevRecord( MemoryTrace::iterator *it,
    KRecordList *displayList )
{
  --( *it );
  while ( !it->isNull() )
  {
    if ( window->passFilter( it ) )
    {
      if ( ( ( createList & CREATEEVENTS ) && ( it->getType() & EVENT ) )
           ||
           ( ( createList & CREATECOMMS ) && ( it->getType() & COMM ) ) )
        displayList->insert( window, it );

      if ( functionThread->validRecord( it ) )
        break;
    }
    --( *it );
  }

  if ( it->isNull() )
  {
    delete it;
    it = window->getCPUBeginRecord( order );
  }

  return it;
}

TWindowLevel IntervalCPU::getWindowLevel() const
{
  return window->getLevel();
}


Interval *IntervalCPU::getWindowInterval( TWindowLevel whichLevel,
    TObjectOrder whichOrder )
{
  return window->getLevelInterval( whichLevel, whichOrder );
}


bool IntervalCPU::IsDerivedWindow() const
{
  return window->isDerivedWindow();
}


TWindowLevel IntervalCPU::getComposeLevel( TWindowLevel whichLevel ) const
{
  return window->getComposeLevel( whichLevel );
}


KTrace *IntervalCPU::getWindowTrace() const
{
  return ( KTrace* )window->getTrace();
}

