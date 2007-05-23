#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include <string>
#include  "bplustreerecordleaf.h"

// Tuning this parameter changes tree performance.
#define NODE_SIZE 3
#define LEAF_SIZE 4 // max 256

class BPlusNode
{
  public:
    virtual ~BPlusNode() {};

    // Inserts rl in the tree.
    // If a leaf split is done, newChild returns its address instead of NULL.
    // Returns a pointer to the RecordLeaf with the smallest key.
    virtual RecordLeaf *Insert( RecordLeaf *rl, BPlusNode *&newChild ) = 0;

    // Returns a pointer to the RecordLeaf with the smallest key of this node.
    virtual RecordLeaf *MinKey() = 0;

    // Same as MinKey, but all the subtree is searched.
    virtual RecordLeaf *MinKeyTotal() = 0;

    // Returns a pointer to the record_t stored in the i cell of the tree.
    virtual bool GetLeafData( unsigned char ii, record_t *&data ) = 0;

    // Returns a pointer to the i cell of the leaf.
    virtual bool GetLeafKey( unsigned char ii, RecordLeaf *&key ) = 0;
    virtual unsigned int LinkRecords( record_t *&ini,
                                      record_t *&fin,
                                      int &recs2link,
                                      RecordLeaf *&last_leaf) = 0;

    virtual void Print( string indent ) = 0;
    virtual BPlusNode *Split( BPlusNode *dest, RecordLeaf *&retdat ) = 0;
    virtual bool PartialDelete( RecordLeaf *limit_key,
                                BPlusNode **valid_predecessor )=0;
    virtual unsigned char CountElems() = 0;
};


class BPlusLeaf : public BPlusNode
{
  private:
    // Performs a RecordLeaf ordered insertion in a node.
    void InsertRecordInOrder(RecordLeaf *r);
    void SetUsed(unsigned char used);
    unsigned char GetUsed();
    bool IsEmpty();
    unsigned char CountElems(); // same as GetUsed

  public:
    RecordLeaf records[LEAF_SIZE];

    BPlusLeaf();
    ~BPlusLeaf();
    virtual RecordLeaf *Insert( RecordLeaf *rl, BPlusNode *&newChild );
    virtual RecordLeaf *MinKey();
    virtual RecordLeaf *MinKeyTotal();

    // Inserts a RecordLeaf in the first free position.
    void AppendRecord(RecordLeaf newRecord);

    // Inserts a RecordLeaf in the node. If it's full, splits returning
    // the new leaf and the smallest key (retKey).
    BPlusLeaf *SplitAndInsert(RecordLeaf *rec, RecordLeaf *&retKey);

    virtual BPlusLeaf *Split(BPlusNode *dest, RecordLeaf *&retdat );

    virtual bool GetLeafData(unsigned char ii, record_t *&data);
    virtual bool GetLeafKey(unsigned char ii,  RecordLeaf *&key);

    virtual unsigned int LinkRecords( record_t *&ini, record_t *&fin,
                                       int &recs2link,
                                       RecordLeaf *&last_leaf);

    virtual void Print(string indent);
    virtual bool PartialDelete( RecordLeaf *limit_key,
                                BPlusNode **valid_predecessor );
};


class BPlusInternal : public BPlusNode
{
  public:
    unsigned char used; // to be removed.
    RecordLeaf *key[NODE_SIZE];
    BPlusNode  *child[NODE_SIZE+1];

    BPlusInternal();
    ~BPlusInternal();
    virtual RecordLeaf *Insert( RecordLeaf *rl, BPlusNode *&newChild );
    virtual RecordLeaf *MinKey();
    virtual RecordLeaf *MinKeyTotal();
    void InsertInOrder( BPlusNode *c );
    void Append( BPlusNode *newNode );
    BPlusInternal *SplitAndInsert( BPlusNode *c, RecordLeaf *&retdat );
    virtual BPlusInternal *Split(BPlusNode *dest, RecordLeaf *&retdat );

    virtual bool GetLeafData(unsigned char ii, record_t *&data);
    virtual bool GetLeafKey(unsigned char ii,  RecordLeaf *&key);

    virtual void Print(string indent);
    virtual unsigned int LinkRecords( record_t *&ini,
                                      record_t *&fin,
                                      int &recs2link,
                                      RecordLeaf *&last_leaf );
    virtual bool PartialDelete( RecordLeaf *limit_key,
                                BPlusNode **valid_predecessor );
    unsigned char CountElems();
};


class BPlusTree
{
  public:

    struct TRecord
    {
      TRecordType type;
      TRecordTime time;
      union
      {
        struct TStateRecord stateRec;
        struct TEventRecord eventRec;
        struct TCommRecord  commRec;
      } URecordInfo;
      struct TRecord *next, *prev;
      struct TRecord *threadNext, *threadPrev;
    }

    BPlusNode *root;
    BPlusNode *ini;
    BPlusNode *fin;
    record_t *rini; // returns the initial record.
    record_t *rfin; // returns the final record.
    RecordLeaf *tmp_aux; // Temporal copy of RecordLeaf to be inserted. Needed?
    unsigned int records_inserted;
    RecordLeaf *last_leaf;
    unsigned int records_linked_last_time;

    // Methods
    BPlusTree();
    ~BPlusTree();
    void Insert( record_t *r );
    BPlusNode *GetIni(){ return ini; };

    bool GetLeafData( unsigned char ii, record_t *&data );
    bool GetLeafKey( unsigned char ii, RecordLeaf *&key );
    unsigned int GetNumRecords() { return records_inserted; }
    record_t *GetIniRecord() { return rini; }
    record_t *GetFinRecord() { return rfin; }
    void GetRecordFirstTime( record_t **rft );

    unsigned int LinkRecords( int recs2link );
    void Print();
    void PartialDelete();
};
#endif
