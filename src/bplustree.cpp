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

//#include <stdlib.h>
#include <math.h>
//#include <vector>

#include "bplustree.h"
#include "bplustreeexception.h"
#include "trace.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using namespace bplustree;

/******************************************************************************
 * UnloadedTrace
 ******************************************************************************/
UnloadedTrace::UnloadedTrace()
{}


UnloadedTrace::UnloadedTrace( const TThreadOrder totalThreads,
                              const TCPUOrder    totalCPUs )
{
  numThreads = totalThreads;
  numCPUs    = totalCPUs;

  // Time list.
  first = NULL;
  last = NULL;

  // Threads list.
  threadFirst.reserve( numThreads );
  threadLast.reserve( numThreads );
  for ( TObjectOrder ii = 0; ii < numThreads; ++ii )
  {
    threadFirst.push_back( NULL );
    threadLast.push_back( NULL );
  }

  // CPUs list.
  CPUFirst.reserve( numCPUs );
  CPULast.reserve( numCPUs );
  for ( TCPUOrder ii = 0; ii < numCPUs; ++ii )
  {
    CPUFirst.push_back( NULL );
    CPULast.push_back( NULL );
  }
}

UnloadedTrace::~UnloadedTrace()
{}


// enlaza fragmento recien descargado
// append del fragmento de traza recien descargada
void UnloadedTrace::append( TRecord *rini, TRecord *rfin )
{
  TRecord      *current;
  TThreadOrder thread;
  TCPUOrder    cpu;

  current = rini;
  while ( current != NULL )
  {
    if ( first == NULL )
    {
      first = current;
      current->prev = NULL;
    }
    last = current;

    thread = current->thread;
    if ( threadFirst[ thread ] == NULL )
    {
      threadFirst[ thread ] = current;
      threadLast[ thread ] = current;
      current->threadNext = NULL;
      current->threadPrev = NULL;
    }
    else
    {
      current->threadPrev = threadLast[ thread ];
      current->threadNext = NULL;
      threadLast[ thread ]->threadNext = current;
      threadLast[ thread ] = current;
    }

    if ( numCPUs > 0 )
    {
      cpu = current->CPU;
      if( cpu > 0 )
      {
        --cpu;
        if( cpu < numCPUs )
        {
          if ( CPUFirst[ cpu ] == NULL )
          {
            CPUFirst[ cpu ] = current;
          }
          CPULast[ cpu ] = current;
        }
      }
    }
    current = current->next;

  }
}


/****************************************************************************
   ######  ######  #       #     #  #####  #       #######    #    #######
   #     # #     # #       #     # #     # #       #         # #   #
   #     # #     # #       #     # #       #       #        #   #  #
   ######  ######  #       #     #  #####  #       #####   #     # #####
   #     # #       #       #     #       # #       #       ####### #
   #     # #       #       #     # #     # #       #       #     # #
   ######  #       #######  #####   #####  ####### ####### #     # #
 ****************************************************************************/

BPlusLeaf::BPlusLeaf()
{
  setUsed( 0 );
}


BPlusLeaf::~BPlusLeaf()
{}


RecordLeaf *BPlusLeaf::minKey()
{
  return &records[ 0 ];
}


RecordLeaf *BPlusLeaf::minKeyTotal()
{
  return &records[ 0 ];
}


void BPlusLeaf::setUsed( PRV_UINT16 used )
{
  if ( used < LEAF_SIZE )
  {
    records[ LEAF_SIZE - 1 ].setOrder( ( RecordLeaf::TRecordOrder )used );
    records[ LEAF_SIZE - 1 ].setRecordDirect( NULL );
  }
}


PRV_UINT16 BPlusLeaf::getUsed()
{
  PRV_UINT16 used;

  if ( records[ LEAF_SIZE - 1 ].getRecord() == NULL )
    used = ( PRV_UINT16 )records[ LEAF_SIZE - 1 ].getOrder();
  else
    used = LEAF_SIZE;

  return used;
}


void BPlusLeaf::insertRecordInOrder( RecordLeaf *rl )
{
  bool inserted( false );
  PRV_UINT16 used = getUsed();

  for ( PRV_UINT16 ii = 0; ii < used; ++ii )
  {
    if ( *rl < records[ ii ] )
    {
      for ( PRV_UINT16 jj = used; jj > ii; --jj )
        records[ jj ] = records[ jj - 1 ];

      records[ ii ] = *rl;
      inserted = true;
      break;
    }
  }

  if ( !inserted )
    records[ used ] = *rl;

  setUsed( ++used );
}


RecordLeaf *BPlusLeaf::insert( RecordLeaf *rl, BPlusNode *&newChild )
{
  RecordLeaf *retKey = NULL;
  PRV_UINT16 used = getUsed();

  if ( used < LEAF_SIZE )
  {
    insertRecordInOrder( rl );
    newChild = NULL;
  }
  else
    newChild = splitAndInsert( rl , retKey );

  return retKey;
}


BPlusLeaf *BPlusLeaf::split( BPlusNode *dest, RecordLeaf *&retdat )
{
  BPlusLeaf *newLeaf = new BPlusLeaf;
  PRV_UINT16 used = getUsed();
  PRV_UINT16 endPos = ( used - 2 ) / 2;

  for ( PRV_UINT16 ii = endPos + 1; ii < used; ++ii )
    newLeaf->appendRecord( records[ii] );

  setUsed( endPos + 1 );

  dest = newLeaf;
  retdat = newLeaf->minKey();

  return newLeaf;
}


BPlusLeaf *BPlusLeaf::splitAndInsert( RecordLeaf *rec, RecordLeaf *&retKey )
{
  BPlusLeaf *newLeaf = new BPlusLeaf;
  PRV_UINT16 used = getUsed();
  PRV_UINT16 endPos = ( used - 2 ) / 2;
  BPlusNode *newNode( NULL );

  if ( ( endPos * 2 < used ) && ( *rec >= records[ endPos + 1 ] ) )
    endPos++;

  for ( PRV_UINT16 ii = endPos + 1; ii < used; ++ii )
    newLeaf->appendRecord( records[ii] );

  setUsed( endPos + 1 );

  if ( *rec < *newLeaf->minKey() )
    insert( rec, newNode );
  else
    newLeaf->insert( rec, newNode );

  retKey = newLeaf->minKey();

  return newLeaf;
}


void BPlusLeaf::appendRecord( RecordLeaf newRecord )
{
  PRV_UINT16 used = getUsed();

  if ( used < LEAF_SIZE - 1 )
  {
    records[ used ] = newRecord;
    setUsed( ++used );
  }
  else
    throw BPlusTreeException( BPlusTreeException::invalidAppend,
                              "Leaf is full.",
                              __FILE__,
                              __LINE__ );
}


bool BPlusLeaf::getLeafData( PRV_UINT16 ii, TRecord *&data )
{
  PRV_UINT16 used = getUsed();

  if ( ii < used )
    data = records[ ii ].getRecord();
  else
    data = NULL;

  return ( ii < used );
}


bool BPlusLeaf::getLeafKey( PRV_UINT16 ii, RecordLeaf *&key )
{
  PRV_UINT16 used = getUsed();

  if ( ii < used )
    key = &records[ ii ];
  else
    key = NULL;

  return ( ii < used );
}


PRV_UINT32 BPlusLeaf::linkRecords( TRecord **ini,
                               TRecord **fin,
                               PRV_INT32 &recs2link,
                               RecordLeaf *&lastLeaf,
                               Index<TRecord *> *traceIndex )
{
  TRecord *prev, *cur, *initial;
  PRV_UINT16 used = getUsed();
  PRV_UINT16 num = 0;
  PRV_UINT32 localRecs2link;

  initial = NULL;
  prev    = NULL;
  cur     = NULL;

  if ( ( recs2link > LEAF_SIZE ) || ( recs2link < 0 ) )
    localRecs2link = LEAF_SIZE;  // Link all records in the leaf.
  else
    localRecs2link = recs2link;  // Link only indicated records

  if ( ( used > 0 )  && ( localRecs2link > 0 ) )
  {
    prev = records[ 0 ].getRecord();
    initial = prev;

    num = 1;
    while ( ( num < used ) && ( --localRecs2link > 0 ) )
    {
      cur = records[ num ].getRecord();
      cur->prev = prev;
      prev->next = cur;
      prev = cur;
      ++num;
      traceIndex->indexRecord( cur->time, cur );
    }
    *ini = initial;
    *fin = prev;
    lastLeaf = &records[ num - 1 ];
  }

  if ( recs2link > 0 )
    recs2link -= num;

  return ( PRV_UINT32 )num;
}


void BPlusLeaf::print( string indent )
{
  /*  PRV_UINT16 used = getUsed();

    cout << indent << "<" << ( int )used << endl;
    for ( PRV_UINT16 ii = 0; ii < used - 1; ii++ )
      cout << indent << records[ ii ];

    cout << indent << records[ used - 1 ];
    cout << indent << ">" << endl << endl;*/
}

// ojo, no se pueden hacer dos partial deletes seguidos, porque es la
// operacion de linkRecords la que asigna last_leaf;
// si tras un partial delete insertamos nuevos records, y alguno va
// delante, se podria perder
bool BPlusLeaf::partialDelete( RecordLeaf *limitKey,
                               BPlusNode **validPredecessor )
{
  PRV_UINT16 used = getUsed();
  PRV_UINT16 num = 0;
  bool end = false;
  bool deletedAll = false;

  if ( used > 0 )
  {
    // Find greater key.
    num = 0;
    while ( !end )
    {
      if ( ( num < used ) && ( records[ num ] <= *limitKey ) )
        ++num;
      else
        end = true;
    }

    // Copy the greater ones to the beginning.
    for ( PRV_UINT16 ii = num; ii < used; ++ii )
      records[ ii - num ] = records[ ii ];

    // Update used
    used -= num;
    deletedAll = ( used == 0 );
    setUsed( used );

    if ( deletedAll )
      delete this;
  }

  return ( deletedAll );
}

/*
PRV_UINT16 BPlusLeaf::countElems()
{
  return getUsed();
}
*/

/******************************************************************************
                ######  ######  #       #     #  #####
                #     # #     # #       #     # #     #
                #     # #     # #       #     # #
                ######  ######  #       #     #  #####
                #     # #       #       #     #       #
                #     # #       #       #     # #     #
                ######  #       #######  #####   #####

         ###   #     # ####### ####### ######  #     #    #    #
          #    ##    #    #    #       #     # ##    #   # #   #
          #    # #   #    #    #       #     # # #   #  #   #  #
          #    #  #  #    #    #####   ######  #  #  # #     # #
          #    #   # #    #    #       #   #   #   # # ####### #
          #    #    ##    #    #       #    #  #    ## #     # #
         ###   #     #    #    ####### #     # #     # #     # #######
 ******************************************************************************/
BPlusInternal::BPlusInternal()
{
  used = 0;
  for ( PRV_UINT16 ii = 0; ii < NODE_SIZE; ++ii )
  {
    key[ ii ] = NULL;
    child[ ii ] = NULL;
  }
  child[ NODE_SIZE ] = NULL;
}


BPlusInternal::~BPlusInternal()
{
  for ( PRV_UINT16 ii = 0; ii < used; ++ii )
    delete child[ ii ];
}


RecordLeaf *BPlusInternal::minKey()
{
  return key[ 0 ];
}


RecordLeaf *BPlusInternal::minKeyTotal()
{
  return child[ 0 ]->minKeyTotal();
}


void BPlusInternal::insertInOrder( BPlusNode *newNode )
{
  bool inserted( false );

  for ( int ii = ( int )used - 2; ii >= 0; --ii )
  {
    if ( *newNode->minKeyTotal() > *key[ ii ] )
    {
      for ( PRV_UINT16 jj = used - 2; jj >= ii + 1; --jj )
      {
        key[ jj + 1 ] = key[ jj ];
        child[ jj + 2 ] = child[ jj + 1 ];
      }
      child[ ii + 2 ] = newNode;
      key[ ii + 1 ] = newNode->minKeyTotal();
      inserted = true;
      break;
    }
  }

  if ( !inserted )
  {
    for ( int jj = ( int )used - 2; jj >= 0; --jj )
    {
      key[ jj + 1 ] = key[ jj ];
      child[ jj + 2 ] = child[ jj + 1 ];
    }
    child[ 1 ] = child[ 0 ];
    child[ 0 ] = newNode;

    if ( *child[ 0 ]->minKey() > *child[ 1 ]->minKey() )
    {
      BPlusNode *tmp = child[ 0 ];
      child[ 0 ] = child[ 1 ];
      child[ 1 ] = tmp;
    }
    key[ 0 ] = child[ 1 ]->minKeyTotal();
  }

  ++used;

  for ( PRV_UINT16 ii = 0; ii < used; ++ii )
  {
    if ( child[ ii ] == NULL )
      used = ii;
  }
}


void BPlusInternal::append( BPlusNode *newNode )
{
  child[ used ] = newNode;

  if ( used >= 1 )
    key[ used - 1 ] = newNode->minKeyTotal();

  ++used;
}


BPlusInternal *BPlusInternal::splitAndInsert( BPlusNode *newNode,
    RecordLeaf *&retdat )
{
  BPlusInternal *newInternal = new BPlusInternal;
  PRV_UINT16 middle = ( ( PRV_UINT16 ) ceil( used / 2.0 ) ) - 1;
  bool intoThis = false;

  if ( *newNode->minKey() < *child[ middle ]->minKey() )
  {
    --middle;
    intoThis = true;
  }

  for ( PRV_UINT16 ii = middle + 1; ii < used; ++ii )
    newInternal->append( child[ ii ] );

  used = middle + 1;

  if ( intoThis )
    insertInOrder( newNode );
  else
    newInternal->insertInOrder( newNode );

  retdat = newInternal->minKeyTotal();

  return newInternal;
}


RecordLeaf *BPlusInternal::insert( RecordLeaf *rl, BPlusNode *&newChild )
{
  BPlusNode *newNode;
  RecordLeaf *retKey;
  PRV_UINT16 pos;
  bool inserted( false );

  for ( pos = 0; pos < used - 1; ++pos )
    if ( *rl < *key[ pos ] )
    {
      retKey = child[ pos ]->insert( rl, newNode );
      if ( newNode != NULL )
      {
        if ( pos > 0 )
          if ( *retKey < *key[ pos - 1 ] )
            key[ pos - 1 ] = retKey;
      }
      inserted = true;
      break;
    }

  if ( !inserted )
  {
    retKey = child[ used - 1 ]->insert( rl, newNode );
    pos = used - 1;
  }

  if ( newNode == NULL )
    newChild = NULL;
  else
  {
    if ( used < NODE_SIZE + 1 )
    {
      insertInOrder( newNode );
      newChild = NULL;
    }
    else
    {
      newChild = splitAndInsert( newNode, retKey );
    }
  }

  return retKey;
}


bool BPlusInternal::getLeafData( PRV_UINT16 ii, TRecord *&data )
{
  return child[ 0 ]->getLeafData( ii, data );
}


bool BPlusInternal::getLeafKey( PRV_UINT16 ii, RecordLeaf *&key )
{
  return child[ 0 ]->getLeafKey( ii, key );
}


PRV_UINT32 BPlusInternal::linkRecords( TRecord **ini, TRecord **fin,
                                   PRV_INT32 &recs2link,
                                   RecordLeaf *&lastLeaf,
                                   Index<TRecord *> *traceIndex )
{
  TRecord *prevIni, *prevFin, *currIni, *currFin;
  PRV_UINT32 recsLinked = 0;
  PRV_UINT16 ii = 0;

  // Link all the records in the leaf.
  if ( recs2link != 0 )
  {
    prevIni = prevFin = currIni = currFin = NULL;

    recsLinked += child[ 0]->linkRecords( &prevIni, &prevFin, recs2link, lastLeaf, traceIndex );

    ii = 1;
    while ( ( ii < used ) && ( recs2link != 0 ) )
    {
      recsLinked += child[ ii ]->linkRecords( &currIni, &currFin, recs2link, lastLeaf, traceIndex );
      if ( currFin != NULL )
      {
        prevFin->next = currIni;
        currIni->prev = prevFin;
        // ant_ini not modified
        prevFin = currFin;
      }
      ++ii;
    }

    *ini = prevIni;
    *fin = prevFin;
  }

  return recsLinked;
}


void BPlusInternal::print( string indent )
{
  cout << indent << "^(" << ( int )used << ")" << endl;
  cout << indent << "[";

  for ( PRV_UINT16 ii = 0; ii < used - 1; ++ii )
  {
    if ( key[ ii ] != NULL )
      cout << ( key[ ii ]->getRecord() )->time << " ";
    else
      cout << "key " << ii << " NULL!!!";
  }
  cout << "]" << endl;

  if ( child[ 0 ] != NULL )
    child[ 0 ]->print( indent + "  " );
  else
    cout << "child[0] NULL!!!";
  for ( PRV_UINT16 ii = 1; ii < used; ++ii )
  {
    if ( child[ 0 ] != NULL )
      child[ ii ]->print( indent + "  " );
    else
      cout << "child[0] NULL!!!";
  }
  cout << indent << "v" << endl;
}


BPlusInternal *BPlusInternal::split( BPlusNode *dest,
                                     RecordLeaf *&retdat )
{
  BPlusInternal *newInternal = new BPlusInternal;
  PRV_UINT16 middle = ( ( PRV_UINT16 ) ceil( used / 2.0 ) ) - 1;


  for ( PRV_UINT16 ii = middle + 1; ii < used; ++ii )
    newInternal->append( child[ ii ] );

  used = middle + 1;

  dest = newInternal;
  retdat = newInternal->minKeyTotal();

  return newInternal;
}

/*
PRV_UINT16 BPlusInternal::countElems()
{
  return used;
}
*/

/*****************************************************************************
 ** partialDelete
 *****************************************************************************/
bool BPlusInternal::partialDelete( RecordLeaf *limitKey,
                                   BPlusNode **validPredecessor )
{
  PRV_UINT16 removed;
  bool deletedAll = false;
  int ii;
  BPlusNode *auxValidPred =  *validPredecessor;

  if ( limitKey != NULL )
  {
    // Left recursive total delete.
    removed = 0;                       // testing <=
    while ( ( removed < used - 1 ) && ( *key[ removed ] <= *limitKey ) )
    {
      delete child[ removed ];
      ++removed;
    }

    // Recursive partial delete.
    if ( removed == used - 1 )
      deletedAll = child[ removed ]->partialDelete( limitKey,
                   validPredecessor );
    else
      deletedAll = child[removed]->partialDelete( limitKey, &child[ removed ] );

    if ( deletedAll )
      ++removed; // Count the last child.

    // Rearrange keys and childs, copying at the beginning of the node.
    // It could be improved implementing circular vector.
    for ( ii = removed; ii < used - 1; ++ii )
    {
      key[ ii - removed ] = key[ ii ];
      if ( removed != 0 )
        key[ ii ] = NULL;
    }
    for ( ii = removed; ii < used; ++ii )
    {
      child[ ii - removed ] = child[ ii ];
      if ( removed != 0 )
        child[ ii ] = NULL;
    }
    // Rearrange node size.
    used -= removed;

    switch ( used )
    {
      case 0: // Treat ZOMBIE:  valid_predecessor-->zombie ==>
        //                valid_predecessor
        deletedAll = true;
        break;
      case 1: // Treat ZOMBIE:  valid_predecessor-->zombie--> only child  ==>
        //                valid_predecessor --> only_child
        if ( *validPredecessor != auxValidPred )
        {
          child[ 0 ] = NULL;
          delete this;
        }
        else
        {
          *validPredecessor = child[ 0 ];
          child[ 0 ] = NULL;
          delete this;
        }
        deletedAll = false; // because we still have to check the childs
        break;
      default:  // In fact, we should maintain the conditions about node filling
        // percent. Still not implemented.
        deletedAll = false;  // returns deleted_all if child returns it.
        break;
    }
  }

  return deletedAll;
}


/****************************************************************************
    ######  ######  #       #     #  #####  ####### ######  ####### #######
    #     # #     # #       #     # #     #    #    #     # #       #
    #     # #     # #       #     # #          #    #     # #       #
    ######  ######  #       #     #  #####     #    ######  #####   #####
    #     # #       #       #     #       #    #    #   #   #       #
    #     # #       #       #     # #     #    #    #    #  #       #
    ######  #       #######  #####   #####     #    #     # ####### #######
 ****************************************************************************/
BPlusTree::BPlusTree( const TThreadOrder totalThreads,
                      const TCPUOrder totalCPUs,
                      const PRV_UINT32 uthreshold,
                      const PRV_UINT32 upercent )
{
  root = NULL;
  ini = NULL;
  tmpAux = new RecordLeaf;
  recordsInserted = 0;
  recordsLinkedLastTime = 0;
  lastLeaf = NULL;

  numThreads = totalThreads;
  numCPUs    = totalCPUs;
  unloadThreshold = uthreshold;
  unloadPercent   = upercent;

  unloadedTrace = new UnloadedTrace( totalThreads, totalCPUs );
  traceIndex    = new Index<TRecord *>();
}

BPlusTree::~BPlusTree()
{
  if ( root != NULL )
  {
    delete root;
  }
  delete tmpAux;
  delete unloadedTrace;
  delete traceIndex;
}


TTime BPlusTree::finish( TTime headerTime, Trace *whichTrace )
{
  TRecord tmpBegin, tmpEnd;

  unload( );

  if ( unloadedTrace->getEnd()->time > headerTime )
    headerTime = unloadedTrace->getEnd()->time;

  emptyThreadBegin.reserve( numThreads );
  emptyThreadEnd.reserve( numThreads );

  tmpBegin.time = 0;
  tmpEnd.time = headerTime;
  tmpBegin.type = EMPTYREC;
  tmpEnd.type = EMPTYREC;
  emptyRec = tmpBegin;
  for ( TThreadOrder i = 0; i < numThreads; ++i )
  {
    tmpBegin.thread = i;
    tmpEnd.thread = i;
    tmpBegin.threadNext = unloadedTrace->getThreadBegin( i );
    tmpBegin.threadPrev = NULL;
    tmpEnd.threadNext = NULL;
    tmpEnd.threadPrev = unloadedTrace->getThreadEnd( i );
    emptyThreadBegin.push_back( tmpBegin );
    emptyThreadEnd.push_back( tmpEnd );
  }
  endRec = tmpEnd;

  emptyCPUBegin.reserve( numCPUs );
  emptyCPUEnd.reserve( numCPUs );

  tmpBegin.time = 0;
  tmpEnd.time = headerTime;
  for ( TCPUOrder i = 0; i < numCPUs; ++i )
  {
    tmpBegin.CPU = i + 1;
    tmpEnd.CPU = i + 1;
    TNodeOrder tmpNode;
    TCPUOrder tmpCPU;
    whichTrace->getCPULocation( i + 1, tmpNode, tmpCPU );
    std::vector<TThreadOrder> tmpThreads;
    whichTrace->getThreadsPerNode( tmpNode + 1, tmpThreads );
    if( tmpThreads.empty() )
    {
      tmpBegin.thread = 0;
      tmpEnd.thread = 0;
    }
    else
    {
      tmpBegin.thread = tmpThreads[ 0 ];
      tmpEnd.thread = tmpThreads[ 0 ];
    }

    tmpBegin.next = unloadedTrace->getCPUBegin( i );
    tmpBegin.prev = NULL;
    tmpEnd.next = NULL;
    tmpEnd.prev = unloadedTrace->getCPUEnd( i );
    emptyCPUBegin.push_back( tmpBegin );
    emptyCPUEnd.push_back( tmpEnd );
  }

  return headerTime;
}


void BPlusTree::insert( MemoryBlocks *blocks )
{
  TRecord *tmp;

  for ( PRV_UINT32 i = 0; i < blocks->getCountInserted(); ++i )
  {
    tmp = ( TRecord * )blocks->getLastRecord( i );

    insert( tmp );
  }

  blocks->resetCountInserted();
}


void BPlusTree::insert( TRecord *r )
{
  tmpAux->setRecord( r );

  BPlusNode *newNode = NULL;

  if ( root == NULL )
  {
    ini = new BPlusLeaf;
    root = ini;
  }

  root->insert( tmpAux, newNode );

  if ( newNode != NULL )
  {
    BPlusInternal *newRoot = new BPlusInternal;
    newRoot->append( root );
    newRoot->append( newNode );
    root = newRoot;
  }

  ++recordsInserted;

  if ( unloadCriteria() )
    unload( ( PRV_UINT32 )( unloadThreshold * unloadPercent / 100 ) );
}

void BPlusTree::getRecordFirstTime( TRecord **rft )
{
  if ( lastLeaf == NULL )
    // ojo last_leaf se actualiza al sacar.
    *rft = NULL;  // error, nada se ha enlazado todavia.
  else
    *rft = lastLeaf->getRecord();
}


bool BPlusTree::getLeafData( PRV_UINT16 ii, TRecord *&data )
{
  return ini->getLeafData( ii, data );
}


bool BPlusTree::getLeafKey( PRV_UINT16 ii, RecordLeaf *&key )
{
  return ini->getLeafKey( ii, key );
}


PRV_UINT32 BPlusTree::linkRecords( TRecord **ini, TRecord **fin, PRV_INT32 recs2link )
{
  PRV_UINT32 recordsLinked;

  recordsLinked = root->linkRecords( ini, fin, recs2link, lastLeaf, traceIndex );
  recordsLinkedLastTime = recordsLinked;

//  cout << "#RECORDS LINKED: " << recordsLinked << endl;

  return recordsLinked;
}


void BPlusTree::print()
{
  if ( root != NULL )
  {
    root->print( "" );
    cout << endl;
  }
}


void BPlusTree::partialDelete()
{
  if ( root != NULL )
  {
    root->partialDelete( lastLeaf, &root );
    recordsInserted -= recordsLinkedLastTime;
    if ( recordsInserted <= 0 )
    {
      delete root;
      root = NULL;
    }
  }
}


void BPlusTree::unload( PRV_INT32 numrecords )
{
  PRV_UINT32 records_linked;
  TRecord *ini, *fin;

  if ( root != NULL )
  {
    records_linked = linkRecords( &ini, &fin, numrecords );
    unloadedTrace->append( ini, fin );
    if ( numrecords != -1 )
      this->partialDelete();
    else
    {
      delete root;
      root = NULL;
    }
  }
}


void BPlusTree::getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                       TRecordTime whichTime ) const
{
  TRecord *current = NULL;
  TThreadOrder filled = 0;

  for ( TThreadOrder ii = 0; ii < numThreads; ++ii )
  {
    if ( listIter[ ii ] != NULL )
    {
      delete listIter[ ii ];
      listIter[ ii ] = NULL;
    }
  }

  // Search for especific time.
  if ( traceIndex->findRecord( whichTime, current ) )
  {
    while ( current != NULL )
    {
      if ( current->time < whichTime )
        break;
      current = current->prev;
    }
  }

  // Backward search filling vector of iterators.
  while ( ( current != NULL ) && ( filled < numThreads ) )
  {
    if ( listIter[ current->thread ] == NULL )
    {
      listIter[ current->thread ] = new BPlusTree::ThreadIterator( current );
      ++filled;
    }
    current = current->prev;
  }

  // Fill the rest.
  if ( filled < numThreads )
  {
    for ( TThreadOrder i = 0; i < numThreads; ++i )
      if ( listIter[ i ] == NULL )
        listIter[ i ] = threadBegin( i );
  }
}

void BPlusTree::getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                    TRecordTime whichTime ) const
{
  TRecord *current = NULL;
  TCPUOrder filled = 0;

  for ( TCPUOrder ii = 0; ii < numCPUs; ++ii )
  {
    if ( listIter[ ii ] != NULL )
    {
      delete listIter[ ii ];
      listIter[ ii ] = NULL;
    }
  }

  // Search for especific time.
  if ( traceIndex->findRecord( whichTime, current ) )
  {
    while ( current != NULL )
    {
      if ( current->time < whichTime )
        break;
      current = current->prev;
    }
  }
  // Backward search filling vector of iterators.
  while ( ( current != NULL ) && ( filled < numCPUs ) )
  {
    if ( current->CPU > 0 && listIter[ current->CPU - 1 ] == NULL )
    {
      listIter[ current->CPU - 1 ] = new BPlusTree::CPUIterator( current );
      ++filled;
    }
    current = current->prev;
  }

  // Fill the rest.
  if ( filled < numCPUs )
  {
    for ( TCPUOrder i = 0; i < numCPUs; ++i )
      if ( listIter[ i ] == NULL )
        listIter[ i ] = CPUBegin( i );
  }
}

/******************************************************************************
 * MemoryTrace Inherited Iterator.
 ******************************************************************************/

inline BPlusTree::iterator::iterator( TRecord *whichRecord )
{
  record = whichRecord;
}

inline void BPlusTree::iterator::operator++()
{
#ifdef STRICT_CHECK_DEBUG
  if ( record !=  NULL )
    record = ( ( TRecord * )record )->next;
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "next unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
#else
  record = ( ( TRecord * )record )->next;
#endif
}

inline void BPlusTree::iterator::operator--()
{
#ifdef STRICT_CHECK_DEBUG
  if ( record !=  NULL )
    record = ( ( TRecord * )record )->prev;
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "prev unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
#else
  record = ( ( TRecord * )record )->prev;
#endif
}


inline BPlusTree::iterator *BPlusTree::iterator::clone() const
{
  return new BPlusTree::iterator( ( TRecord * )record );
}


inline TRecordType  BPlusTree::iterator::getType() const
{
  return ( ( TRecord * )record )->type;
}

inline TRecordTime  BPlusTree::iterator::getTime() const
{
  return ( ( TRecord * )record )->time;
}

inline TThreadOrder BPlusTree::iterator::getThread() const
{
  return ( ( TRecord * )record )->thread;
}

inline TCPUOrder    BPlusTree::iterator::getCPU() const
{
  return ( ( TRecord * )record )->CPU;
}

inline TObjectOrder BPlusTree::iterator::getOrder() const
{
  return ( ( TRecord * )record )->thread;
}

inline TEventType   BPlusTree::iterator::getEventType() const
{
  return ( ( TRecord * )record )->URecordInfo.eventRecord.type;
}

inline TEventValue  BPlusTree::iterator::getEventValue() const
{
  return ( ( TRecord * )record )->URecordInfo.eventRecord.value;
}

inline TState       BPlusTree::iterator::getState() const
{
  return ( ( TRecord * )record )->URecordInfo.stateRecord.state;
}

inline TRecordTime  BPlusTree::iterator::getStateEndTime() const
{
  return ( ( TRecord * )record )->URecordInfo.stateRecord.endTime;
}

inline TCommID      BPlusTree::iterator::getCommIndex() const
{
  return ( ( TRecord * )record )->URecordInfo.commRecord.index;
}

inline void BPlusTree::iterator::setTime( const TRecordTime whichTime )
{
  ( ( TRecord * )record )->time = whichTime;
}

inline void BPlusTree::iterator::setType( const TRecordType whichType )
{
  ( ( TRecord * )record )->type = whichType;
}

inline void BPlusTree::iterator::setStateEndTime( const TRecordTime whichEndTime )
{
  ( ( TRecord * )record )->URecordInfo.stateRecord.endTime = whichEndTime;
}

/**************************************************************************
 * MemoryTrace Inherited ThreadIterator.
 **************************************************************************/

inline TObjectOrder BPlusTree::ThreadIterator::getOrder() const
{
  return ( ( TRecord * )record )->thread;
}

inline void BPlusTree::ThreadIterator::operator++()
{
#ifdef STRICT_CHECK_DEBUG
  if ( record !=  NULL )
    record = ( ( TRecord * )record )->threadNext;
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "threadNext unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
#else
  record = ( ( TRecord * )record )->threadNext;
#endif
}

inline void BPlusTree::ThreadIterator::operator--()
{
#ifdef STRICT_CHECK_DEBUG
  if ( record !=  NULL )
    record = ( ( TRecord * )record )->threadPrev;
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "threadPrev unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
#else
  record = ( ( TRecord * )record )->threadPrev;
#endif
}

inline BPlusTree::ThreadIterator *BPlusTree::ThreadIterator::clone() const
{
  return new BPlusTree::ThreadIterator( ( TRecord * )record );
}

/**************************************************************************
 * MemoryTrace Inherited CPUIterator.
 **************************************************************************/

inline TObjectOrder BPlusTree::CPUIterator::getOrder() const
{
  return ( ( TRecord * )record )->CPU;
}

inline void BPlusTree::CPUIterator::operator++()
{
  if ( record !=  NULL )
  {
    TRecord *myRecord = ( TRecord * )this->record; // Keep current, maybe it's the last one.

    // Forward search, looking for next record with same CPU.
    record = ( ( TRecord * )record )->next;
    while ( record != NULL )
    {
      if ( ( ( TRecord * )record )->CPU == myRecord->CPU )
        break;
      record = ( ( TRecord * )record )->next;
    }
  }
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "CPUNext unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
}

inline void BPlusTree::CPUIterator::operator--()
{
  // Shared code with ++, but the prev/next and exception message.
  if ( record !=  NULL )
  {
    TRecord *myRecord = ( TRecord * )this->record; // Keep current, maybe it's the last one.

    // Backward search, looking for previous record with same CPU.
    record = ( ( TRecord * )record )->prev;
    while ( record != NULL )
    {
      if ( ( ( TRecord * )record )->CPU == myRecord->CPU )
        break;
      record = ( ( TRecord * )record )->prev;
    }
  }
  else
    throw BPlusTreeException( BPlusTreeException::wrongIterator,
                              "CPUPrev unreachable, record NULL.",
                              __FILE__,
                              __LINE__ );
}

inline BPlusTree::CPUIterator *BPlusTree::CPUIterator::clone() const
{
  return new BPlusTree::CPUIterator( ( TRecord * )record );
}

/**************************************************************************
 * MemoryTrace Inherited
 **************************************************************************/
MemoryTrace::iterator* BPlusTree::empty() const
{
  return new BPlusTree::iterator( ( TRecord * )&emptyRec );
}

MemoryTrace::iterator* BPlusTree::begin() const
{
  return new BPlusTree::iterator( unloadedTrace->getBegin() );
}

MemoryTrace::iterator* BPlusTree::end() const
{
  return new BPlusTree::iterator( ( TRecord * )&endRec );
}

MemoryTrace::iterator* BPlusTree::threadBegin( TThreadOrder whichThread ) const
{
  return new BPlusTree::ThreadIterator( ( TRecord * )&emptyThreadBegin[ whichThread ] );
}

MemoryTrace::iterator* BPlusTree::threadEnd( TThreadOrder whichThread ) const
{
  return new BPlusTree::ThreadIterator( ( TRecord * )&emptyThreadEnd[ whichThread ] );
}

MemoryTrace::iterator* BPlusTree::CPUBegin( TCPUOrder whichCPU ) const
{
  return new BPlusTree::CPUIterator( ( TRecord * )&emptyCPUBegin[ whichCPU ] );
}

MemoryTrace::iterator* BPlusTree::CPUEnd( TCPUOrder whichCPU ) const
{
  return new BPlusTree::CPUIterator( ( TRecord * )&emptyCPUEnd[ whichCPU ] );
}
