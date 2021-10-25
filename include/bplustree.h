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


#pragma once


#include <string>

#include "paraverkerneltypes.h"
#include "memorytrace.h"
#include "bplustreetypes.h"
#include "bplustreerecordleaf.h"
#include "index.h"
#include "bplustreeblocks.h"

namespace bplustree
{
  // Parameters to tune tree performance
  static const PRV_UINT16 NODE_SIZE = 64;
  static const PRV_UINT16 LEAF_SIZE = 64;
  static const PRV_UINT32 UNLOAD_RECORDS_THRESHOLD = 100000000;
  static const PRV_UINT32 UNLOAD_PERCENT = 30;

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
      std::vector<TRecord *> threadFirst;
      std::vector<TRecord *> threadLast;

      // List by CPU.
      std::vector<TRecord *> CPUFirst;
      std::vector<TRecord *> CPULast;
  };


  class BPlusNode
  {
    public:
      virtual ~BPlusNode() = default;

      /************************************************************************
       * Inserts rl in the tree.
       * If a leaf split is done, newChild returns its address instead of nullptr.
       * Returns a pointer to the RecordLeaf with the smallest key.
       ************************************************************************/
      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild ) = 0;

      virtual RecordLeaf *minNodeKey() = 0;
      virtual RecordLeaf *minSubtreeKey() = 0;

      /************************************************************************
       * Returns a pointer to the TRecord stored in the i cell of the tree.
       ************************************************************************/
      virtual bool getLeafData( PRV_UINT16 ii, TRecord *&data ) = 0;

      /************************************************************************
       * Returns a pointer to the i cell of the leaf.
       ************************************************************************/
      virtual bool getLeafKey( PRV_UINT16 ii, RecordLeaf *&key ) = 0;

      virtual PRV_UINT32 linkRecords( TRecord **ini,
                                      TRecord **fin,
                                      int &recs2link,
                                      RecordLeaf *&last_leaf,
                                      Index<TRecord *> *traceIndex ) = 0;

      virtual BPlusNode *split( BPlusNode *dest, RecordLeaf *&retdat ) = 0;

      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor ) = 0;

      virtual void print( std::string indent ) = 0;
  };


  class BPlusLeaf : public BPlusNode
  {
    public:
      RecordLeaf records[LEAF_SIZE];

    public:
      BPlusLeaf();

      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild ) override;

      virtual RecordLeaf *minNodeKey() override;
      virtual RecordLeaf *minSubtreeKey() override;

      virtual bool getLeafData( PRV_UINT16 ii, TRecord *&data ) override;
      virtual bool getLeafKey( PRV_UINT16 ii,  RecordLeaf *&key ) override;

      virtual PRV_UINT32 linkRecords( TRecord **ini, TRecord **fin,
                                      PRV_INT32 &recs2link,
                                      RecordLeaf *&last_leaf,
                                      Index<TRecord *> *traceIndex ) override;

      virtual BPlusLeaf *split( BPlusNode *dest, RecordLeaf *&retdat ) override;

      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor ) override;

      virtual void print( std::string indent ) override;

      /************************************************************************
       * Inserts a RecordLeaf in the first free position.
       ************************************************************************/
      void appendRecord( RecordLeaf newRecord );

      /************************************************************************
       * Inserts a RecordLeaf in the node. If it's full, splits returning
       * the new leaf and the smallest key (retKey).
       ************************************************************************/
      BPlusLeaf *splitAndInsert( RecordLeaf *rec, RecordLeaf *&retKey );

    private:
      /************************************************************************
       * Performs a RecordLeaf ordered insertion in a node.
       ************************************************************************/
      void insertRecordInOrder( RecordLeaf *r );

      void setUsed( PRV_UINT16 used );
      PRV_UINT16 getUsed();
      inline bool isEmpty()
      {
        return ( getUsed() == ( PRV_UINT16 )0 );
      }
  };


  class BPlusInternal : public BPlusNode
  {
    public:
      PRV_UINT16 used; // to be removed.
      RecordLeaf *key[NODE_SIZE];
      BPlusNode  *child[NODE_SIZE+1];

      BPlusInternal();
      ~BPlusInternal();

      virtual RecordLeaf *insert( RecordLeaf *rl, BPlusNode *&newChild ) override;

      virtual RecordLeaf *minNodeKey() override;
      virtual RecordLeaf *minSubtreeKey() override;

      virtual bool getLeafData( PRV_UINT16 ii, TRecord *&data ) override;
      virtual bool getLeafKey( PRV_UINT16 ii,  RecordLeaf *&key ) override;

      virtual PRV_UINT32 linkRecords( TRecord **ini,
                                      TRecord **fin,
                                      PRV_INT32 &recs2link,
                                      RecordLeaf *&last_leaf,
                                      Index<TRecord *> *traceIndex ) override;

      virtual BPlusInternal *split( BPlusNode *dest, RecordLeaf *&retdat ) override;

      virtual bool partialDelete( RecordLeaf *limit_key,
                                  BPlusNode **valid_predecessor ) override;

      virtual void print( std::string indent ) override;

      void insertInOrder( BPlusNode *c );
      void append( BPlusNode *newNode );
      BPlusInternal *splitAndInsert( BPlusNode *c, RecordLeaf *&retdat );
  };


  class BPlusTree : public MemoryTrace
  {
    public:
      BPlusNode *root;
      BPlusNode *ini;
      BPlusNode *fin;
      RecordLeaf *tmpAux; // Temporal copy of RecordLeaf to be inserted. Needed?
      PRV_UINT32 recordsInserted;
      RecordLeaf *lastLeaf;
      PRV_UINT32 recordsLinkedLastTime;
      UnloadedTrace *unloadedTrace;

      BPlusTree();
      BPlusTree( const Trace *whichTrace,
                 const TThreadOrder totalThreads,
                 const TCPUOrder totalCPUs,
                 const PRV_UINT32 uthresh  = UNLOAD_RECORDS_THRESHOLD,
                 const PRV_UINT32 upercent = UNLOAD_PERCENT );
      ~BPlusTree();
      
      virtual void insert( MemoryBlocks *blocks ) override;
      virtual TTime finish( TTime headerTime, Trace *whichTrace ) override;

      inline  BPlusNode *getIni()
      {
        return ini;
      };

      bool getLeafData( PRV_UINT16 ii, TRecord *&data );
      bool getLeafKey( PRV_UINT16 ii, RecordLeaf *&key );

      inline PRV_UINT32 getNumRecords()
      {
        return recordsInserted;
      }

      inline bool unloadCriteria()
      {
        return ( recordsInserted > unloadThreshold );
      }

      void getRecordFirstTime( TRecord **rft );

      PRV_UINT32 linkRecords( TRecord **ini, TRecord **fin, PRV_INT32 recs2link );
      void print();
      void partialDelete();
      void unload( PRV_INT32 numrecords = -1 );

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

          iterator( TRecord *whichRecord, const Trace *whichTrace );

          virtual void operator++() override;
          virtual void operator--() override;

          virtual iterator *clone() const override;

          virtual TRecordType    getType() const override;
          virtual TRecordTime    getTime() const override;
          virtual TThreadOrder   getThread() const override;
          virtual TCPUOrder      getCPU() const override;
          virtual TObjectOrder   getOrder() const override;
          virtual TEventType     getEventType() const override;
          virtual TSemanticValue getEventValue() const override;
          virtual TEventValue    getEventValueAsIs() const override;
          virtual TState         getState() const override;
          virtual TRecordTime    getStateEndTime() const override;
          virtual TCommID        getCommIndex() const override;

          virtual void           setTime( const TRecordTime whichTime ) override;
          virtual void           setType( const TRecordType whichType ) override;
          virtual void           setStateEndTime( const TRecordTime whichEndTime ) override;
      };

      class ThreadIterator : public BPlusTree::iterator
      {
        public:
          ThreadIterator()
          {}

          // Constructor declaration needed for iterators.
          ThreadIterator( TRecord *whichRecord, const Trace *whichTrace )
            : BPlusTree::iterator( whichRecord, whichTrace )
          {}

          virtual TObjectOrder getOrder() const override;

          virtual void operator++() override;
          virtual void operator--() override;

          virtual ThreadIterator *clone() const override;
      };

      class CPUIterator : public BPlusTree::iterator
      {
        public:
          CPUIterator()
          {}

          // Constructor declaration needed for iterators.
          CPUIterator( TRecord *whichRecord, const Trace *whichTrace )
            : BPlusTree::iterator( whichRecord, whichTrace )
          {}

          virtual TObjectOrder getOrder() const override;

          virtual void operator++() override;
          virtual void operator--() override;

          virtual CPUIterator *clone() const override;
      };

      // MemoryTrace inherited methods
      virtual MemoryTrace::iterator* empty() const override; // Dummy iterator set to EMPTYREC
      virtual MemoryTrace::iterator* begin() const override;
      virtual MemoryTrace::iterator* end() const override;
      virtual MemoryTrace::iterator* threadBegin( TThreadOrder whichThread ) const override;
      virtual MemoryTrace::iterator* threadEnd( TThreadOrder whichThread ) const override;
      virtual MemoryTrace::iterator* CPUBegin( TCPUOrder whichCPU ) const override;
      virtual MemoryTrace::iterator* CPUEnd( TCPUOrder whichCPU ) const override;

      virtual void getRecordByTimeThread( std::vector<MemoryTrace::iterator *>& listIter,
                                          TRecordTime whichTime ) const override;
      virtual void getRecordByTimeCPU( std::vector<MemoryTrace::iterator *>& listIter,
                                       TRecordTime whichTime ) const override;

    private:
      const Trace *myTrace;
      TThreadOrder numThreads;
      TCPUOrder numCPUs;
      PRV_UINT32 unloadThreshold;
      PRV_UINT32 unloadPercent;
      Index<TRecord *> *traceIndex;
      TRecord emptyRec;
      TRecord endRec;
      std::vector< TRecord > emptyThreadBegin;
      std::vector< TRecord > emptyThreadEnd;
      std::vector< TRecord > emptyCPUBegin;
      std::vector< TRecord > emptyCPUEnd;

      void insert( TRecord *r );
  };
}
