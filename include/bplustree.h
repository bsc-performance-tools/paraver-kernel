#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include <string>

#include "paraverkerneltypes.h"
#include "memorytrace.h"
#include "bplustreetypes.h"
#include "bplustreerecordleaf.h"

namespace bplustree
{
// Tuning this parameters changes tree performance.
  static const UINT8 NODE_SIZE = 3;
  static const UINT8 LEAF_SIZE = 4; // max 256
  static const UINT32 UNLOAD_RECORDS_THRESHOLD = 1000000;
  static const UINT32 UNLOAD_PERCENT = 30;

  class BPlusNode
  {
    public:

      virtual ~BPlusNode()
      {};

      /************************************************************************
       * Inserts rl in the tree.
       * If a leaf split is done, newChild returns its address instead of NULL.
       * Returns a pointer to the RecordLeaf with the smallest key.
       ************************************************************************/
      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild ) = 0;

      /************************************************************************
       * Returns a pointer to the RecordLeaf with the smallest key of this node.
       ************************************************************************/
      virtual RecordLeaf *minKey() = 0;

      /************************************************************************
       * Same as MinKey, but all the subtree is searched.
       ************************************************************************/
      virtual RecordLeaf *minKeyTotal() = 0;

      /************************************************************************
       * Returns a pointer to the TRecord stored in the i cell of the tree.
       ************************************************************************/
      virtual bool getLeafData( UINT8 ii, TRecord *&data ) = 0;

      /************************************************************************
       * Returns a pointer to the i cell of the leaf.
       ************************************************************************/
      virtual bool getLeafKey( UINT8 ii, RecordLeaf *&key ) = 0;
      virtual UINT32 linkRecords( TRecord *&ini,
                                  TRecord *&fin,
                                  int &recs2link,
                                  RecordLeaf *&last_leaf ) = 0;

      virtual BPlusNode *split( BPlusNode *dest, RecordLeaf *&retdat ) = 0;
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor ) = 0;
      //virtual UINT8 countElems() = 0;
      virtual void print( string indent ) = 0;
  };


  class BPlusLeaf : public BPlusNode
  {
    public:
      RecordLeaf records[LEAF_SIZE];

    public:
      BPlusLeaf();
      ~BPlusLeaf();
      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild );
      virtual RecordLeaf *minKey();
      virtual RecordLeaf *minKeyTotal();

      /************************************************************************
       * Inserts a RecordLeaf in the first free position.
       ************************************************************************/
      void appendRecord( RecordLeaf newRecord );

      /************************************************************************
       * Inserts a RecordLeaf in the node. If it's full, splits returning
       * the new leaf and the smallest key (retKey).
       ************************************************************************/
      BPlusLeaf *splitAndInsert( RecordLeaf *rec, RecordLeaf *&retKey );

      virtual BPlusLeaf *split( BPlusNode *dest, RecordLeaf *&retdat );

      virtual bool getLeafData( UINT8 ii, TRecord *&data );
      virtual bool getLeafKey( UINT8 ii,  RecordLeaf *&key );

      virtual UINT32 linkRecords( TRecord *&ini, TRecord *&fin,
                                  int &recs2link,
                                  RecordLeaf *&last_leaf );

      virtual void print( string indent );
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor );

    private:
      /************************************************************************
       * Performs a RecordLeaf ordered insertion in a node.
       ************************************************************************/
      void insertRecordInOrder( RecordLeaf *r );

      void setUsed( UINT8 used );
      UINT8 getUsed();
      inline bool isEmpty() { return ( getUsed() == (UINT8)0 ); }
      // UINT8 countElems();
  };


  class BPlusInternal : public BPlusNode
  {
    public:
      UINT8 used; // to be removed.
      RecordLeaf *key[NODE_SIZE];
      BPlusNode  *child[NODE_SIZE+1];

      BPlusInternal();
      ~BPlusInternal();
      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild );
      virtual RecordLeaf *minKey();
      virtual RecordLeaf *minKeyTotal();
      void insertInOrder( BPlusNode *c );
      void append( BPlusNode *newNode );
      BPlusInternal *splitAndInsert( BPlusNode *c, RecordLeaf *&retdat );
      virtual BPlusInternal *split( BPlusNode *dest, RecordLeaf *&retdat );

      virtual bool getLeafData( UINT8 ii, TRecord *&data );
      virtual bool getLeafKey( UINT8 ii,  RecordLeaf *&key );

      virtual void print( string indent );
      virtual UINT32 linkRecords( TRecord *&ini,
                                  TRecord *&fin,
                                  int &recs2link,
                                  RecordLeaf *&last_leaf );
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor );
      //UINT8 countElems();
  };


  class BPlusTree : public MemoryTrace
  {
    private:
      UINT32 unloadThreshold;
      UINT32 unloadPercent;

      TThreadOrder numThreads;
      TCPUOrder    numCPUs;

      // List by time.
      TRecord *rini;
      TRecord *rfin;
      // List by thread.
      vector<TRecord *> threadFirst;
      vector<TRecord *> threadLast;
      // List by CPU.
      vector<TRecord *> CPUFirst;
      vector<TRecord *> CPULast;

      inline TRecord *getIniRecord()
      {
        return rini;
      }
      inline TRecord *getFinRecord()
      {
        return rfin;
      }

    public:

      BPlusNode *root;
      BPlusNode *ini;
      BPlusNode *fin;
      RecordLeaf *tmpAux; // Temporal copy of RecordLeaf to be inserted. Needed?
      UINT32 recordsInserted;
      RecordLeaf *lastLeaf;
      UINT32 recordsLinkedLastTime;

      // Methods
      BPlusTree();
      BPlusTree( const TThreadOrder totalThreads,
                 const TCPUOrder totalCPUs,
                 const UINT32 uthresh  = UNLOAD_RECORDS_THRESHOLD,
                 const UINT32 upercent = UNLOAD_PERCENT );
      ~BPlusTree();
      void insert( TRecord *r );

      inline  BPlusNode *getIni()
      {
        return ini;
      };

      bool getLeafData( UINT8 ii, TRecord *&data );
      bool getLeafKey( UINT8 ii, RecordLeaf *&key );

      inline UINT32 getNumRecords()
      {
        return recordsInserted;
      }

      inline bool unloadCriteria()
      {
        return ( recordsInserted > unloadThreshold );
      }

      void getRecordFirstTime( TRecord **rft );

      UINT32 linkRecords( int recs2link );
      void print();
      void partialDelete();
      UINT32 unload( const UINT32 numrecords );

      protected:

      public:

    /**************************************************************************
     * MemoryTrace Inherited Iterator.
     **************************************************************************/
    class iterator : public MemoryTrace::iterator
    {
        public:
          iterator();

          iterator( TRecord *whichRecord );

          virtual ~iterator();

          virtual void operator++();
          virtual void operator--();
          virtual bool operator==( const iterator &it );
          virtual bool operator!=( const iterator &it );
          virtual bool isNull() const;

          virtual TRecordType  getType() const;
          virtual TRecordTime  getTime() const;
          virtual TThreadOrder getThread() const;
          virtual TCPUOrder    getCPU() const;
          virtual TEventType   getEventType() const;
          virtual TEventValue  getEventValue() const;
          virtual TState       getState() const;
          virtual TRecordTime  getStateEndTime() const;
          virtual TCommID      getCommIndex() const;

        protected:
          TRecord *record;

        private:
      };

    class ThreadIterator : public BPlusTree::iterator, MemoryTrace::ThreadIterator
    {
      public:
        //ThreadIterator();
        //ThreadIterator( TRecord *whichRecord );

        virtual ~ThreadIterator();

        virtual void operator++();
        virtual void operator--();
    };

    class CPUIterator : public BPlusTree::iterator, MemoryTrace::CPUIterator
    {
      public:
        //CPUIterator();
        //CPUIterator( TRecord *whichRecord );

        virtual ~CPUIterator();

        virtual void operator++();
        virtual void operator--();
    };

      // MemoryTrace Inherited Methods
    virtual MemoryTrace::iterator* begin() const;
    virtual MemoryTrace::iterator* end() const;
    virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const;
    virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const;
    virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const;
    virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const;

    virtual void getRecordByTime( vector<MemoryTrace::iterator *>& listIter,
                                  TRecordTime whichTime ) const;

  };
}

#endif
