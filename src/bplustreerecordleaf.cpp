#include <iomanip>

#include "bplustreerecordleaf.h"
#include "paraverkernelexception.h"

using namespace bplustree;

// Inits class attribute global_order.
RecordLeaf::TRecordOrder RecordLeaf::global_order = 0;

/******************************************************************************
 * Constructors & Destructor Methods
 ******************************************************************************/
RecordLeaf::RecordLeaf( TRecord *r ) // Preferred form.
{
  rec = r;
  ord = RecordLeaf::global_order++;
}

RecordLeaf::RecordLeaf( TRecord *r, TRecordOrder order )
{
  rec = r;
  ord = order;
}

RecordLeaf::RecordLeaf()
{
  rec = NULL;
  ord = 0;
}

RecordLeaf::~RecordLeaf()
{}


/******************************************************************************
 * Assign & Compare Methods
 ******************************************************************************/
RecordLeaf& RecordLeaf::operator=( const RecordLeaf &rl )
{
  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '='" ,
                                  __FILE__,
                                  __LINE__ );
  }

  // Does not make a copy!
  rec = rl.rec;
  ord = rl.ord;

  return *this;
}


bool RecordLeaf::operator<( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '<'",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  time2 = getTime( rl.rec );

  if( time1 < time2 )
    return true;
  else if( time1 > time2 )
    return false;

  type1 = getTypeOrdered( rec );
  type2 = getTypeOrdered( rl.rec );

  return ( type1 < type2 ||
           ( ( type1 == type2 ) && ( ord < rl.ord ) )
         );
}


bool RecordLeaf::operator>( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '>'",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  time2 = getTime( rl.rec );

  if( time1 > time2 )
    return true;
  else if( time1 < time2 )
    return false;

  type1 = getTypeOrdered( rec );
  type2 = getTypeOrdered( rl.rec );

  return ( type1 > type2 ||
           ( ( type1 == type2 ) && ( ord > rl.ord ) )
         );
}


bool RecordLeaf::operator==( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '=='",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  type1 = getTypeOrdered( rec );
  time2 = getTime( rl.rec );
  type2 = getTypeOrdered( rl.rec );

  return ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord == rl.ord ) );
}


bool RecordLeaf::operator!=( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '!='",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  type1 = getTypeOrdered( rec );
  time2 = getTime( rl.rec );
  type2 = getTypeOrdered( rl.rec );

  return ( ( time1 != time2 ) || ( type1 != type2 ) || ( ord != rl.ord ) );
}


bool RecordLeaf::operator>=( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '>='",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  type1 = getTypeOrdered( rec );
  time2 = getTime( rl.rec );
  type2 = getTypeOrdered( rl.rec );

  return ( ( time1 > time2 ) ||
           ( ( time1 == time2 ) && ( type1 > type2 ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord > rl.ord ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord == rl.ord ) )
         );
}


bool RecordLeaf::operator<=( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ParaverKernelException( ParaverKernelException::nullOperand,
                                  "BPlusTree: RecordLeaf: Op: '<='",
                                  __FILE__,
                                  __LINE__ );
  }

  time1 = getTime( rec );
  type1 = getTypeOrdered( rec );
  time2 = getTime( rl.rec );
  type2 = getTypeOrdered( rl.rec );

  return ( ( time1 < time2 ) ||
           ( ( time1 == time2 ) && ( type1 < type2 ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord < rl.ord ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord == rl.ord ) )
         );
}


/******************************************************************************
 * I/O Methods
 ******************************************************************************/
std::ostream& bplustree::operator<<( std::ostream& os, const RecordLeaf& rl )
{
  string msg;

  switch ( getTypeOrdered( rl.getRecord() ) )
  {
    case ( UINT8 )0:
      msg = "STEND";
      break;
    case ( UINT8 )1:
      msg = "P RCV";
      break;
    case ( UINT8 )2:
      msg = "R RCV";
      break;
    case ( UINT8 )3:
      msg = "R SND";
      break;
    case ( UINT8 )4:
      msg = "L SND";
      break;
    case ( UINT8 )5:
      msg = "P SND";
      break;
    case ( UINT8 )6:
      msg = "L RCV-EVNT";
      break;
    case ( UINT8 )7:
      msg = "GLOBC";
      break;
    case ( UINT8 )8:
      msg = "STBEG";
      break;
    case ( UINT8 )9:
      msg = "UNKNOWN";
      break;
  }
  os.setf( ios::right, ios::adjustfield );
  os << '[';
  os << setw( 8 )  << getTime( rl.getRecord() );
  os << setw( 3 )  << " - ";
  os << setw( 10 ) << msg;
  os << setw( 3 )  << " - ";
  os << setw( 8 )  << rl.getOrder();
  os << setw( 3 );
  os  << " ]";

  return os << "\n";
}
