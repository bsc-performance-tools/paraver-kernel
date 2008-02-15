#ifndef _BPLUSTREE_H
#define _BPLUSTREE_H

#include <string>

#include "paraverkerneltypes.h"
#include "memorytrace.h"
#include "bplustreetypes.h"
#include "bplustreerecordleaf.h"
#include "index.h"
#include "bplustreeblocks.h"

namespace bplustree
{
// Tuning this parameters changes tree performance.
  static const UINT16 NODE_SIZE = 3;
  static const UINT16 LEAF_SIZE = 4;
  static const UINT32 UNLOAD_RECORDS_THRESHOLD = 1000000;
  static const UINT32 UNLOAD_PERCENT = 30;

  class UnloadedTrace
  {
    public:
      UnloadedTrace();
      UnloadedTrace( const TThreadOrder totalThreads,
                     const TCPUOrder    totalCPUs );

      ~UnloadedTrace();

      inline TRecord *getBegin()
      {
        return first;
      }
      inline TRecord *getEnd()
      {
        return last;
      }
      inline TRecord *getThreadBegin( TThreadOrder whichThread )
      {
        return threadFirst[ whichThread ];
      }
      inline TRecord *getThreadEnd( TThreadOrder whichThread )
      {
        return threadLast[ whichThread ];
      }
      inline TRecord *getCPUBegin( TCPUOrder whichThread )
      {
        return CPUFirst[ whichThread ];
      }
      inline TRecord *getCPUEnd( TCPUOrder whichThread )
      {
        return CPULast[ whichThread ];
      }

      void append( TRecord *rini, TRecord *rfin );

    protected:

    private:
      TThreadOrder numThreads;
      TCPUOrder    numCPUs;

      // List by time.
      TRecord *first;
      TRecord *last;

      // List by thread.
      vector<TRecord *> threadFirst;
      vector<TRecord *> threadLast;

      // List by CPU.
      vector<TRecord *> CPUFirst;
      vector<TRecord *> CPULast;
  };



  class BPlusNode
  {
    public:

      virtual ~BPlusNode()
      {}

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
      virtual bool getLeafData( UINT16 ii, TRecord *&data ) = 0;

      /************************************************************************
       * Returns a pointer to the i cell of the leaf.
       ************************************************************************/
      virtual bool getLeafKey( UINT16 ii, RecordLeaf *&key ) = 0;
      virtual UINT32 linkRecords( TRecord **ini,
                                  TRecord **fin,
                                  int &recs2link,
                                  RecordLeaf *&last_leaf,
                                  Index *traceIndex ) = 0;

      virtual BPlusNode *split( BPlusNode *dest, RecordLeaf *&retdat ) = 0;
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor ) = 0;
      //virtual UINT16 countElems() = 0;
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

      virtual bool getLeafData( UINT16 ii, TRecord *&data );
      virtual bool getLeafKey( UINT16 ii,  RecordLeaf *&key );

      virtual UINT32 linkRecords( TRecord **ini, TRecord **fin,
                                  INT32 &recs2link,
                                  RecordLeaf *&last_leaf,
                                  Index *traceIndex );

      virtual void print( string indent );
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor );

    private:
      /************************************************************************
       * Performs a RecordLeaf ordered insertion in a node.
       ************************************************************************/
      void insertRecordInOrder( RecordLeaf *r );

      void setUsed( UINT16 used );
      UINT16 getUsed();
      inline bool isEmpty()
      {
        return ( getUsed() == ( UINT16 )0 );
      }
      // UINT16 countElems();
  };


  class BPlusInternal : public BPlusNode
  {
    public:
      UINT16 used; // to be removed.
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

      virtual bool getLeafData( UINT16 ii, TRecord *&data );
      virtual bool getLeafKey( UINT16 ii,  RecordLeaf *&key );

      virtual void print( string indent );
      virtual UINT32 linkRecords( TRecord **ini,
                                  TRecord **fin,
                                  INT32 &recs2link,
                                  RecordLeaf *&last_leaf,
                                  Index *traceIndex );
      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor );
      //UINT16 countElems();
  };


  class BPlusTree : public MemoryTrace
  {
    private:
      TThreadOrder numThreads;
      TCPUOrder numCPUs;
      UINT32 unloadThreshold;
      UINT32 unloadPercent;
      Index *traceIndex;
      vector< TRecord > emptyThreadBegin;
      vector< TRecord > emptyThreadEnd;
      vector< TRecord > emptyCPUBegin;
      vector< TRecord > emptyCPUEnd;

      void insert( TRecord *r );

    public:

      BPlusNode *root;
      BPlusNode *ini;
      BPlusNode *fin;
      RecordLeaf *tmpAux; // Temporal copy of RecordLeaf to be inserted. Needed?
      UINT32 recordsInserted;
      RecordLeaf *lastLeaf;
      UINT32 recordsLinkedLastTime;
      UnloadedTrace *unloadedTrace;

      // Methods
      BPlusTree();
      BPlusTree( const TThreadOrder totalThreads,
                 const TCPUOrder totalCPUs,
                 const UINT32 uthresh  = UNLOAD_RECORDS_THRESHOLD,
                 const UINT32 upercent = UNLOAD_PERCENT );
      ~BPlusTree();
      virtual TTime finish( TTime headerTime );
      void insert( BPlusTreeBlocks *blocks );

      inline  BPlusNode *getIni()
      {
        return ini;
      };

      bool getLeafData( UINT16 ii, TRecord *&data );
      bool getLeafKey( UINT16 ii, RecordLeaf *&key );

      inline UINT32 getNumRecords()
      {
        return recordsInserted;
      }

      inline bool unloadCriteria()
      {
        return ( recordsInserted > unloadThreshold );
      }

      void getRecordFirstTime( TRecord **rft );

      UINT32 linkRecords( TRecord **ini, TRecord **fin, INT32 recs2link );
      void print();
      void partialDelete();
      void unload( INT32 numrecords = -1 );

    protected:

    public:

      /**************************************************************************
       * MemoryTrace Inherited Iterator.
       **************************************************************************/
    class iterator : public MemoryTrace::iterator
      {
        public:
          iterator()
          {}

          iterator( TRecord *whichRecord );

          virtual ~iterator()
          {}

          virtual void operator++();
          virtual void operator--();


          virtual TRecordType  getType() const;
          virtual TRecordTime  getTime() const;
          virtual TThreadOrder getThread() const;
          virtual TCPUOrder    getCPU() const;
          virtual TObjectOrder getOrder() const;
          virtual TEventType   getEventType() const;
          virtual TEventValue  getEventValue() const;
          virtual TState       getState() const;
          virtual TRecordTime  getStateEndTime() const;
          virtual TCommID      getCommIndex() const;

      };

    class ThreadIterator : public BPlusTree::iterator
      {
        public:
          ThreadIterator()
          {}

          // Constructor declaration needed for iterators.
          ThreadIterator( TRecord *whichRecord ) : BPlusTree::iterator( whichRecord )
          {}

          virtual ~ThreadIterator()
          {}

          virtual TObjectOrder getOrder() const;

          virtual void operator++();
          virtual void operator--();
      };

    class CPUIterator : public BPlusTree::iterator
      {
        public:
          CPUIterator()
          {}

          // Constructor declaration needed for iterators.
          CPUIterator( TRecord *whichRecord ): BPlusTree::iterator( whichRecord )
          {}

          virtual ~CPUIterator()
          {}

          virtual TObjectOrder getOrder() const;

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

      virtual void getRecordByTimeThread( vector<MemoryTrace::iterator *>& listIter,
                                          TRecordTime whichTime ) const;
      virtual void getRecordByTimeCPU( vector<MemoryTrace::iterator *>& listIter,
                                       TRecordTime whichTime ) const;

  };
}

#endif
