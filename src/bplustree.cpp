using namespace std;

//#include "paraver.h"
#include <math.h>
#include "bplustree.h"


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
  SetUsed( 0 );
}
BPlusLeaf::~BPlusLeaf()
{}

RecordLeaf *BPlusLeaf::MinKey()
{
  return &records[0];
}
RecordLeaf *BPlusLeaf::MinKeyTotal()
{
  return &records[0];
}


void BPlusLeaf::SetUsed( unsigned char used )
{
  if ( used < LEAF_SIZE )
  {
    records[LEAF_SIZE-1].SetOrder( ( RecOrder_t )used );
    records[LEAF_SIZE-1].SetRecordDirect( NULL );
  }
}


unsigned char BPlusLeaf::GetUsed()
{
  unsigned char used;

  if ( records[LEAF_SIZE-1].GetRecord() == NULL )
    used = ( unsigned char )records[LEAF_SIZE-1].GetOrder();
  else
    used = LEAF_SIZE;

  return used;
}


void BPlusLeaf::InsertRecordInOrder( RecordLeaf *rl )
{
  bool done( false );
  unsigned char used = GetUsed();

  for ( unsigned char ii = 0; ii < used; ii++ )
    if ( *rl < records[ii] )
    {
      for ( unsigned char jj = used; jj > ii; jj-- )
        records[jj] = records[jj-1];

      records[ii] = *rl;
      done = true;
      break;
    }

  if ( !done )
    records[used] = *rl;

  used++;
  SetUsed( used );
}


RecordLeaf *BPlusLeaf::Insert( RecordLeaf *rl, BPlusNode *&newChild )
{
  RecordLeaf *retKey;
  unsigned char used = GetUsed();

  if ( used < LEAF_SIZE )
  {
    InsertRecordInOrder( rl );
    newChild = NULL;
  }
  else
    newChild = SplitAndInsert( rl , retKey );

  return retKey;
}


BPlusLeaf *BPlusLeaf::Split( BPlusNode *dest, RecordLeaf *&retdat )
{
  BPlusLeaf *newLeaf = new BPlusLeaf;
  unsigned char used = GetUsed();
  unsigned char d = ( used - 2 ) / 2;

  for ( unsigned char ii = d + 1; ii < used; ii++ )
    newLeaf->AppendRecord( records[ii] );

  used = d + 1;
  SetUsed( used );

  dest = newLeaf;
  retdat = newLeaf->MinKey();
  return newLeaf;
}


BPlusLeaf *BPlusLeaf::SplitAndInsert( RecordLeaf *rec, RecordLeaf *&retKey )
{
  BPlusLeaf *newLeaf = new BPlusLeaf;
  unsigned char used = GetUsed();
  unsigned char d = ( used - 2 ) / 2;

  if ( ( d*2 < used ) && ( *rec >= records[d+1] ) )
    d++;

  for ( unsigned char ii = d + 1; ii < used; ii++ )
    newLeaf->AppendRecord( records[ii] );

  used = d + 1;
  SetUsed( used );
  BPlusNode *c( NULL );

  if ( *rec < *newLeaf->MinKey() )
    Insert( rec, c );
  else
    newLeaf->Insert( rec, c );

  retKey = newLeaf->MinKey();

  return newLeaf;
}


void BPlusLeaf::AppendRecord( RecordLeaf newRecord )
{
  unsigned char used = GetUsed();
  if ( used < LEAF_SIZE - 1 )
  {
    records[used++] = newRecord;
    SetUsed( used );
  }
  else
    cout << "\nERROR - Leaf\n"; // usar errores!!
}


bool BPlusLeaf::GetLeafData( unsigned char ii, record_t *&data )
{
  unsigned char used = GetUsed();
  if ( ii < used )
  {
    data = records[ii].GetRecord();
    return true;
  }
  else
  {
    data = NULL;
    return false;
  }
}


bool BPlusLeaf::GetLeafKey( unsigned char ii,  RecordLeaf *&key )
{
  unsigned char used = GetUsed();
  if ( ii < used )
  {
    key = &records[ii];
    return true;
  }
  else
  {
    key = NULL;
    return false;
  }
}


// primera version: los enlaza todos en la hoja.
unsigned int BPlusLeaf::LinkRecords( record_t *&ini,
                                     record_t *&fin,
                                     int &recs2link,
                                     RecordLeaf *&last_leaf )
{
  record_t *ant, *act, *inicial;
  unsigned char used = GetUsed();
  unsigned char num = 0;
  unsigned int local_recs2link;

  inicial = NULL;
  ant = NULL;
  act = NULL;

#ifdef DEBUG_BTREE
  // cout << endl << "<HOJA:Records pendientes: " << recs2link;
#endif
  // enlazar todos los records de la hoja
  if ( ( recs2link > LEAF_SIZE ) || ( recs2link < 0 ) )
    local_recs2link = LEAF_SIZE;
  else
    local_recs2link = recs2link;

  if ( ( used > 0 )  && ( local_recs2link > 0 ) )
  {
    ant = records[0].GetRecord();
    inicial = ant;

    num = 1;
    while ( ( num < used ) && ( --local_recs2link > 0 ) )
    {
      act = records[num].GetRecord();
      act->prev = ant;
      ant->next = act;
      ant = act;
      num++;
    }
    ini = inicial;
    fin = ant;
    last_leaf = &records[num-1];
  }
#ifdef DEBUG_BTREE
  //cout << "--> enlazados " << (int)num << endl;
#endif
  if ( recs2link > 0 )
    recs2link -= num;

  return ( unsigned int )num;
}


void BPlusLeaf::Print( string indent )
{
  unsigned char used = GetUsed();
  cout << indent << "<" << ( int )used <<  endl;
  for ( unsigned char ii = 0; ii < used - 1; ii++ )
    cout << indent << records[ii];

  cout << indent << records[used-1];
  cout << indent << ">\n\n";
}


// ojo, no se pueden hacer dos partial deletes seguidos, porque es la
// operacion de LinkRecords la que asigna last_leaf;
// si tras un partial delete insertamos nuevos records, y alguno va
// delante, se podria perder
bool BPlusLeaf::IsEmpty()
{
  return ( GetUsed() == 0 );
}


bool BPlusLeaf::PartialDelete( RecordLeaf *limit_key,
                               BPlusNode **valid_predecessor )
{
  unsigned char used = GetUsed();
  unsigned char num = 0;
  bool end = false;
  bool deleted_all = false;

  if ( used > 0 )
  {
    // contar las que son mas pequenyas
    num = 0;
    while ( !end )
    {
      if ( num < used )
      {
        if ( records[num] <= *limit_key )
          num++;
        else
          end = true;
      }
      else
        end = true;
    }

    // copiar las mayores al principio
    for ( unsigned char ii = num; ii < used; ii++ )
      records[ii-num] = records[ii];

    // actualizar used
    used -= num;
    deleted_all = ( used == 0 );
    SetUsed( used );
    if ( deleted_all )
      delete this;
  }

  return ( deleted_all );
}


unsigned char BPlusLeaf::CountElems()
{
  return GetUsed();
}


/****************************************************************************
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
****************************************************************************/
BPlusInternal::BPlusInternal()
{

  used = 0;
  for ( unsigned char ii = 0; ii < NODE_SIZE; ii++ )
  {
    key[ii] = NULL;
    child[ii] = NULL;
  }
  child[NODE_SIZE] = NULL;
}

BPlusInternal::~BPlusInternal()
{
  for ( unsigned char ii = 0; ii < used; ii++ )
    delete child[ii];
}

RecordLeaf *BPlusInternal::MinKey()
{
  return key[0];
}
RecordLeaf *BPlusInternal::MinKeyTotal()
{
  return child[0]->MinKeyTotal();
}

void BPlusInternal::InsertInOrder( BPlusNode *c )
{
  bool done( false );

  for ( int ii = ( int )used - 2; ii >= 0; ii-- )
    if ( *c->MinKeyTotal() > *key[ii] )
    {
      for ( unsigned char jj = used - 2; jj >= ii + 1; jj-- )
      {
        key[jj+1] = key[jj];
        child[jj+2] = child[jj+1];
      }
      child[ii+2] = c;
      key[ii+1] = c->MinKeyTotal();
      done = true;
      break;
    }

  if ( !done )
  {
    for ( int jj = ( int )used - 2; jj >= 0; jj-- )
    {
      key[jj+1] = key[jj];
      child[jj+2] = child[jj+1];
    }
    child[1] = child[0];
    child[0] = c;

    if ( *child[0]->MinKey() > *child[1]->MinKey() )
    {
      BPlusNode *tmp = child[0];
      child[0] = child[1];
      child[1] = tmp;
    }
    key[0] = child[1]->MinKeyTotal();
  }

  used++;

  for ( unsigned char ii = 0; ii < used; ii++ )
    if ( child[ii] == NULL )
      used = ii;
}

void BPlusInternal::Append( BPlusNode *newNode )
{
  child[used] = newNode;

  if ( used >= 1 )
    key[used-1] = newNode->MinKeyTotal();

  used++;
}

BPlusInternal *BPlusInternal::SplitAndInsert( BPlusNode *c,
    RecordLeaf *&retdat )
{
  BPlusInternal *newInternal = new BPlusInternal;
  unsigned char d = ( ( unsigned char ) ceil( used / 2.0 ) ) - 1; //?
  bool intothis = false;

  if ( *c->MinKey() < *child[d]->MinKey() )
  {
    d--;
    intothis = true;
  }

  for ( unsigned char ii = d + 1; ii < used; ii++ )
    newInternal->Append( child[ii] );

  used = d + 1;

  if ( intothis )
    InsertInOrder( c );
  else
    newInternal->InsertInOrder( c );

  retdat = newInternal->MinKeyTotal();

  return newInternal;
}

RecordLeaf *BPlusInternal::Insert( RecordLeaf *rl, BPlusNode *&newChild )
{
  BPlusNode *c;
  RecordLeaf *retKey, *other;
  unsigned char pos;
  bool done( false );

  for ( pos = 0; pos < used - 1; pos++ )
    if ( *rl < *key[pos] )
    {
      retKey = child[pos]->Insert( rl, c );
      if ( c != NULL )
      {
        if ( pos > 0 )
          if ( *retKey < *key[pos-1] )
            key[pos-1] = retKey;
      }
      done = true;
      break;
    }

  if ( !done )
  {
    retKey = child[used-1]->Insert( rl, c );
    pos = used - 1;
  }

  if ( c == NULL )
    newChild = NULL;
  else
  {
    if ( used < NODE_SIZE + 1 )
    {
      InsertInOrder( c );
      newChild = NULL;
    }
    else
    {
      newChild = SplitAndInsert( c, retKey );
      other = newChild->MinKeyTotal();
    }
  }

  return retKey;
}

bool BPlusInternal::GetLeafData( unsigned char ii, record_t *&data )
{
  return child[0]->GetLeafData( ii, data );
}

bool BPlusInternal::GetLeafKey( unsigned char ii, RecordLeaf *&key )
{
  return child[0]->GetLeafKey( ii, key );
}

unsigned int BPlusInternal::LinkRecords( record_t *&ini, record_t *&fin,
    int &recs2link,
    RecordLeaf *&last_leaf )
{
  record_t *ant_ini, *ant_fin, *act_ini, *act_fin;
  unsigned int records_linked = 0;
  unsigned char ii = 0;

#ifdef DEBUG_BTREE
  //cout << '\t' << "[pend-->" << recs2link;
#endif
  // enlazar todos los records de la hoja
  if ( recs2link != 0 )
  {
    ant_ini = ant_fin = act_ini = act_fin = NULL;

    records_linked += child[0]->LinkRecords( ant_ini, ant_fin, recs2link, last_leaf );

    ii = 1;
    while ( ( ii < used ) && ( recs2link != 0 ) )
    {
      records_linked += child[ii]->LinkRecords( act_ini, act_fin, recs2link, last_leaf );
      if ( act_fin != NULL )
      {
        ant_fin->next = act_ini;
        act_ini->prev = ant_fin;
        // ant_ini se queda igual
        ant_fin = act_fin;
      }
      ii++;
    }

    ini = ant_ini;
    fin = ant_fin;
  }

#ifdef DEBUG_BTREE
  //cout << endl << "<-- quedan " << recs2link << "]" << '\t';
#endif

  return records_linked;
}


void BPlusInternal::Print( string indent )
{
  cout << indent << "^(" << ( int )used << ")" << endl;
  cout << indent << "[";
  for ( unsigned char ii = 0; ii < used - 1; ii++ )
  {
    if ( key[ii] != NULL )
      cout << Get_time( key[ii]->GetRecord() ) << " ";
    else
      cout << "key " << ii << " NULL!!!";
  }
  cout << "]" << endl;

  if ( child[0] != NULL )
    child[0]->Print( indent + "  " );
  else
    cout << "child[0] NULL!!!";
  for ( unsigned char ii = 1; ii < used; ii++ )
  {
    if ( child[0] != NULL )
      child[ii]->Print( indent + "  " );
    else
      cout << "child[0] NULL!!!";
  }
  cout << indent << "v\n";
}

BPlusInternal *BPlusInternal::Split( BPlusNode *dest,
                                     RecordLeaf *&retdat )
{
  BPlusInternal *newInternal = new BPlusInternal;
  unsigned char d = ( ( unsigned char ) ceil( used / 2.0 ) ) - 1; //?


  for ( unsigned char ii = d + 1; ii < used; ii++ )
    newInternal->Append( child[ii] );

  used = d + 1;

  dest = newInternal;
  retdat = newInternal->MinKeyTotal();
  return newInternal;
}

unsigned char BPlusInternal::CountElems()
{
  return used;
}


/*****************************************************************************
 ** PartialDelete
 *****************************************************************************/
bool BPlusInternal::PartialDelete( RecordLeaf *limit_key,
                                   BPlusNode **valid_predecessor )
{
  unsigned char removed;
  bool deleted_all = false;
  int ii;
  BPlusNode *aux_valid_pred =  *valid_predecessor;

  if ( limit_key != NULL )
  {
    // Left recursive total delete.
    removed = 0;                       // testing <=
    while ( ( removed < used - 1 ) && ( *key[removed] <= *limit_key ) )
    {
      delete child[removed];
      removed++;
    }

    // Recursive partial delete.
    if ( removed == used - 1 )
      deleted_all = child[removed]->PartialDelete( limit_key,
                    valid_predecessor );
    else
      deleted_all = child[removed]->PartialDelete( limit_key,
                    &child[removed] );

    if ( deleted_all )
      removed++; // Count the last child.

    // Rearrange keys and childs, copying at the beginning of the node.
    // It could be improved implementing circular vector.
    for ( ii = removed; ii < used - 1; ii++ )
    {
      key[ii-removed] = key[ii];
      if ( removed != 0 )
        key[ii] = NULL;
    }
    for ( ii = removed; ii < used; ii++ )
    {
      child[ii-removed] = child[ii];
      if ( removed != 0 )
        child[ii] = NULL;
    }
    // Rearrange node size.
    used -= removed;

    switch ( used )
    {
      case 0: // Treat ZOMBIE:  valid_predecessor-->zombie ==>
        //                valid_predecessor
        deleted_all = true;
        break;
      case 1: // Treat ZOMBIE:  valid_predecessor-->zombie--> only child  ==>
        //                valid_predecessor --> only_child
        if ( *valid_predecessor != aux_valid_pred )
        {
          child[0] = NULL;
          delete this;
        }
        else
        {
          *valid_predecessor = child[0];
          child[0] = NULL;
          delete this;
        }
        deleted_all = false; // because we still have to check the childs
        break;
      default:  // In fact, we should maintain the conditions about node filling
        // percent. Still not implemented.
        deleted_all = false;  // returns deleted_all if child returns it.
        break;
    }
  }

  return deleted_all;
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
  tmp_aux = new RecordLeaf;
  records_inserted = 0;
  records_linked_last_time = 0;
  last_leaf = NULL;
}


BPlusTree::~BPlusTree()
{
  if ( root != NULL )
  {
    delete root;
  }
  delete tmp_aux;
}

void BPlusTree::GetRecordFirstTime( record_t **rft )
{
  if ( last_leaf == NULL )
    // ojo last_leaf se actualiza al sacar.
    *rft = NULL;  // error, nada se ha enlazado todavía.
  else
    *rft = last_leaf->GetRecord();
}

void BPlusTree::Insert( record_t *r )
{
  tmp_aux->SetRecord( r );

  BPlusNode *c( NULL );

  if ( root == NULL )
  {
    ini = new BPlusLeaf;
    root = ini;
  }

  root->Insert( tmp_aux, c );

  if ( c != NULL )
  {
    BPlusInternal *newRoot = new BPlusInternal;
    newRoot->Append( root );
    newRoot->Append( c );
    root = newRoot;
  }

  records_inserted++;
// cout << "Tras insertar, hay " << records_inserted <<  endl;
}

bool BPlusTree::GetLeafData( unsigned char ii, record_t *&data )
{
  return ini->GetLeafData( ii, data );
}

bool BPlusTree::GetLeafKey( unsigned char ii, RecordLeaf *&key )
{
  return ini->GetLeafKey( ii, key );
}

unsigned int BPlusTree::LinkRecords( int recs2link )
{
  unsigned int records_linked;

#ifdef DEBUG_BTREE
  //cout<< "Records para borrar " << recs2link << endl;
#endif

  records_linked = root->LinkRecords( rini, rfin, recs2link, last_leaf );

#ifdef DEBUG_BTREE
  // cout<< "Records pendientes de borrar " << recs2link << endl;
  //cout<< "BPlusTree::LinkRecords: Ultima hoja :" << *last_leaf << endl;
#endif
//  cout << "Tras enlazar " << records_linked
//       << ", quedan "     << records_inserted << endl;
  records_linked_last_time = records_linked;
  return records_linked;
}

void BPlusTree::Print()
{
  if ( root != NULL )
  {
    root->Print( "" );
    cout << '\n';
  }
}


void BPlusTree::PartialDelete()
{
  if ( root != NULL )
  {
    root->PartialDelete( last_leaf, &root );
    records_inserted -= records_linked_last_time;
    if ( records_inserted <= 0 )
      root = NULL;
  }
}
