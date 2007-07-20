#include <math.h>

#include "bplustree.h"
#include "bplustreeexception.h"

using namespace std;
using namespace bplustree;


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


void BPlusLeaf::setUsed( UINT8 used )
{
  if ( used < LEAF_SIZE )
  {
    records[ LEAF_SIZE - 1 ].setOrder( ( RecordLeaf::TRecordOrder )used );
    records[ LEAF_SIZE - 1 ].setRecordDirect( NULL );
  }
}


UINT8 BPlusLeaf::getUsed()
{
  UINT8 used;

  if ( records[ LEAF_SIZE - 1 ].getRecord() == NULL )
    used = ( UINT8 )records[ LEAF_SIZE - 1 ].getOrder();
  else
    used = LEAF_SIZE;

  return used;
}


void BPlusLeaf::insertRecordInOrder( RecordLeaf *rl )
{
  bool inserted( false );
  UINT8 used = getUsed();

  for ( UINT8 ii = 0; ii < used; ii++ )
  {
    if ( *rl < records[ ii ] )
    {
      for ( UINT8 jj = used; jj > ii; jj-- )
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
  RecordLeaf *retKey;
  UINT8 used = getUsed();

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
  UINT8 used = getUsed();
  UINT8 endPos = ( used - 2 ) / 2;

  for ( UINT8 ii = endPos + 1; ii < used; ii++ )
    newLeaf->appendRecord( records[ii] );

  setUsed( endPos + 1 );

  dest = newLeaf;
  retdat = newLeaf->minKey();

  return newLeaf;
}


BPlusLeaf *BPlusLeaf::splitAndInsert( RecordLeaf *rec, RecordLeaf *&retKey )
{
  BPlusLeaf *newLeaf = new BPlusLeaf;
  UINT8 used = getUsed();
  UINT8 endPos = ( used - 2 ) / 2;
  BPlusNode *newNode( NULL );

  if ( ( endPos * 2 < used ) && ( *rec >= records[ endPos + 1 ] ) )
    endPos++;

  for ( UINT8 ii = endPos + 1; ii < used; ii++ )
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
  UINT8 used = getUsed();

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


bool BPlusLeaf::getLeafData( UINT8 ii, TRecord *&data )
{
  UINT8 used = getUsed();

  if ( ii < used )
    data = records[ ii ].getRecord();
  else
    data = NULL;

  return ( ii < used );
}


bool BPlusLeaf::getLeafKey( UINT8 ii, RecordLeaf *&key )
{
  UINT8 used = getUsed();

  if ( ii < used )
    key = &records[ ii ];
  else
    key = NULL;

  return ( ii < used );
}


UINT32 BPlusLeaf::linkRecords( TRecord *&ini,
                               TRecord *&fin,
                               int &recs2link,
                               RecordLeaf *&lastLeaf )
{
  TRecord *prev, *cur, *initial;
  UINT8 used = getUsed();
  UINT8 num = 0;
  UINT32 localRecs2link;

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
      num++;
    }
    ini = initial;
    fin = prev;
    lastLeaf = &records[ num - 1 ];
  }

  if ( recs2link > 0 )
    recs2link -= num;

  return ( UINT32 )num;
}


void BPlusLeaf::print( string indent )
{
  UINT8 used = getUsed();

  cout << indent << "<" << ( int )used << endl;
  for ( UINT8 ii = 0; ii < used - 1; ii++ )
    cout << indent << records[ ii ];

  cout << indent << records[ used - 1 ];
  cout << indent << ">" << endl << endl;
}

// ojo, no se pueden hacer dos partial deletes seguidos, porque es la
// operacion de linkRecords la que asigna last_leaf;
// si tras un partial delete insertamos nuevos records, y alguno va
// delante, se podria perder
bool BPlusLeaf::partialDelete( RecordLeaf *limitKey,
                               BPlusNode **validPredecessor )
{
  UINT8 used = getUsed();
  UINT8 num = 0;
  bool end = false;
  bool deletedAll = false;

  if ( used > 0 )
  {
    // Find greater key.
    num = 0;
    while ( !end )
    {
      if ( ( num < used ) && ( records[ num ] <= *limitKey ) )
        num++;
      else
        end = true;
    }

    // Copy the greater ones to the beginning.
    for ( UINT8 ii = num; ii < used; ii++ )
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
UINT8 BPlusLeaf::countElems()
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
  for ( UINT8 ii = 0; ii < NODE_SIZE; ii++ )
  {
    key[ ii ] = NULL;
    child[ ii ] = NULL;
  }
  child[ NODE_SIZE ] = NULL;
}


BPlusInternal::~BPlusInternal()
{
  for ( UINT8 ii = 0; ii < used; ii++ )
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

  for ( int ii = ( int )used - 2; ii >= 0; ii-- )
  {
    if ( *newNode->minKeyTotal() > *key[ ii ] )
    {
      for ( UINT8 jj = used - 2; jj >= ii + 1; jj-- )
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
    for ( int jj = ( int )used - 2; jj >= 0; jj-- )
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

  used++;

  for ( UINT8 ii = 0; ii < used; ii++ )
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

  used++;
}


BPlusInternal *BPlusInternal::splitAndInsert( BPlusNode *newNode,
    RecordLeaf *&retdat )
{
  BPlusInternal *newInternal = new BPlusInternal;
  UINT8 middle = ( ( UINT8 ) ceil( used / 2.0 ) ) - 1;
  bool intoThis = false;

  if ( *newNode->minKey() < *child[ middle ]->minKey() )
  {
    middle--;
    intoThis = true;
  }

  for ( UINT8 ii = middle + 1; ii < used; ii++ )
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
  RecordLeaf *retKey, *other;
  UINT8 pos;
  bool inserted( false );

  for ( pos = 0; pos < used - 1; pos++ )
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
      other = newChild->minKeyTotal();
    }
  }

  return retKey;
}


bool BPlusInternal::getLeafData( UINT8 ii, TRecord *&data )
{
  return child[ 0 ]->getLeafData( ii, data );
}


bool BPlusInternal::getLeafKey( UINT8 ii, RecordLeaf *&key )
{
  return child[ 0 ]->getLeafKey( ii, key );
}


UINT32 BPlusInternal::linkRecords( TRecord *&ini, TRecord *&fin,
                                   int &recs2link,
                                   RecordLeaf *&lastLeaf )
{
  TRecord *prevIni, *prevFin, *currIni, *currFin;
  UINT32 recsLinked = 0;
  UINT8 ii = 0;

  // Link all the records in the leaf.
  if ( recs2link != 0 )
  {
    prevIni = prevFin = currIni = currFin = NULL;

    recsLinked += child[ 0]->linkRecords( prevIni, prevFin, recs2link, lastLeaf );

    ii = 1;
    while ( ( ii < used ) && ( recs2link != 0 ) )
    {
      recsLinked += child[ ii ]->linkRecords( prevIni, currFin, recs2link, lastLeaf );
      if ( currFin != NULL )
      {
        prevFin->next = currIni;
        currIni->prev = prevFin;
        // ant_ini not modified
        prevFin = currFin;
      }
      ii++;
    }

    ini = prevIni;
    fin = prevFin;
  }

  return recsLinked;
}


void BPlusInternal::print( string indent )
{
  cout << indent << "^(" << ( int )used << ")" << endl;
  cout << indent << "[";

  for ( UINT8 ii = 0; ii < used - 1; ii++ )
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
  for ( UINT8 ii = 1; ii < used; ii++ )
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
  UINT8 middle = ( ( UINT8 ) ceil( used / 2.0 ) ) - 1;


  for ( UINT8 ii = middle + 1; ii < used; ii++ )
    newInternal->append( child[ ii ] );

  used = middle + 1;

  dest = newInternal;
  retdat = newInternal->minKeyTotal();

  return newInternal;
}

/*
UINT8 BPlusInternal::countElems()
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
  UINT8 removed;
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
      removed++;
    }

    // Recursive partial delete.
    if ( removed == used - 1 )
      deletedAll = child[ removed ]->partialDelete( limitKey,
                   validPredecessor );
    else
      deletedAll = child[removed]->partialDelete( limitKey, &child[ removed ] );

    if ( deletedAll )
      removed++; // Count the last child.

    // Rearrange keys and childs, copying at the beginning of the node.
    // It could be improved implementing circular vector.
    for ( ii = removed; ii < used - 1; ii++ )
    {
      key[ ii - removed ] = key[ ii ];
      if ( removed != 0 )
        key[ ii ] = NULL;
    }
    for ( ii = removed; ii < used; ii++ )
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
BPlusTree::BPlusTree()
{
  root = NULL;
  ini = NULL;
  tmpAux = new RecordLeaf;
  recordsInserted = 0;
  recordsLinkedLastTime = 0;
  lastLeaf = NULL;
}


BPlusTree::~BPlusTree()
{
  if ( root != NULL )
  {
    delete root;
  }
  delete tmpAux;
}


void BPlusTree::getRecordFirstTime( TRecord **rft )
{
  if ( lastLeaf == NULL )
    // ojo last_leaf se actualiza al sacar.
    *rft = NULL;  // error, nada se ha enlazado todavi­a.
  else
    *rft = lastLeaf->getRecord();
}


void BPlusTree::insert( TRecord *r )
{
  tmpAux->setRecord( r );

  BPlusNode *newNode( NULL );

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

  recordsInserted++;
}


bool BPlusTree::getLeafData( UINT8 ii, TRecord *&data )
{
  return ini->getLeafData( ii, data );
}


bool BPlusTree::getLeafKey( UINT8 ii, RecordLeaf *&key )
{
  return ini->getLeafKey( ii, key );
}


UINT32 BPlusTree::linkRecords( int recs2link )
{
  UINT32 recordsLinked;

  recordsLinked = root->linkRecords( rini, rfin, recs2link, lastLeaf );
  recordsLinkedLastTime = recordsLinked;

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
      root = NULL;
  }
}

/******************************************************************************
 * MemoryTrace Inherited Iterator.
 ******************************************************************************/
BPlusTree::iterator::iterator( )
{}

BPlusTree::iterator::iterator( TRecord *whichRecord )
{
  record = whichRecord;
}

BPlusTree::iterator::~iterator()
{}


void BPlusTree::iterator::operator++()
{
}

void BPlusTree::iterator::operator--()
{}

TRecordType  BPlusTree::iterator::getType() const
{
  return record->type;
}

TRecordTime  BPlusTree::iterator::getTime() const
{
  return record->time;
}

TThreadOrder BPlusTree::iterator::getThread() const
{
  return record->thread;
}

TCPUOrder    BPlusTree::iterator::getCPU() const
{
  return record->CPU;
}

TEventType   BPlusTree::iterator::getEventType() const
{
  return record->URecordInfo.eventRecord.type;
}

TEventValue  BPlusTree::iterator::getEventValue() const
{
  return record->URecordInfo.eventRecord.value;
}

TState       BPlusTree::iterator::getState() const
{
  return record->URecordInfo.stateRecord.state;
}

TRecordTime  BPlusTree::iterator::getStateEndTime() const
{
  return record->URecordInfo.stateRecord.endTime;
}

TCommID      BPlusTree::iterator::getCommIndex() const
{
  return record->URecordInfo.commRecord.index;
}


/**************************************************************************
 * MemoryTrace Inherited ThreadIterator.
 **************************************************************************/
BPlusTree::ThreadIterator::~ThreadIterator()
{}

void BPlusTree::ThreadIterator::operator++()
{}

void BPlusTree::ThreadIterator::operator--()
{}

/**************************************************************************
 * MemoryTrace Inherited CPUIterator.
 **************************************************************************/
BPlusTree::CPUIterator::~CPUIterator()
{}

void BPlusTree::CPUIterator::operator++()
{}

void BPlusTree::CPUIterator::operator--()
{}


void BPlusTree::getRecordByTime( vector<MemoryTrace::iterator *>& listIter,
                                 TRecordTime whichTime ) const
{}
