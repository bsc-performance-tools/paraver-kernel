#include <iomanip>
#include "bplustreerecordleaf.h"


RecordLeaf::recordLeaf( TRecord *r )
{
  rec = r;
  ord = global_order++;
}


RecordLeaf::recordLeaf( TRecord *r, TRecordOrder order )
{
  rec = r;
  ord = order;
}


RecordLeaf::recordLeaf()
{
  rec = NULL;
  ord = 0;
}


RecordLeaf::~recordLeaf()
{}


void RecordLeaf::setRecord( TRecord *rec )
{
  this->rec  = rec;
  ord = global_order++;
}


RecordLeaf& RecordLeaf::operator=( const RecordLeaf &rl )
{
  if ( rl.rec == NULL )
  {
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '='",
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
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '<'",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

  return ( ( time1 < time2 ) ||
           ( ( time1 == time2 ) && ( type1 < type2 ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord < rl.ord ) )
         );
}


bool RecordLeaf::operator>( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '>'",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

  return ( ( time1 >  time2 ) ||
           ( ( time1 == time2 ) && ( type1 > type2 ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord > rl.ord ) )
         );
}


bool RecordLeaf::operator==( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '=='",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

  return ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord == rl.ord ) );
}


bool RecordLeaf::operator!=( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '!='",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

  return ( ( time1 != time2 ) || ( type1 != type2 ) || ( ord != rl.ord ) );
}


bool RecordLeaf::operator>=( const RecordLeaf &rl )
{
  TRecordTime time1, time2;
  TRecordType type1, type2;

  if ( rl.rec == NULL )
  {
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '>='",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

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
    throw ( ParaverKernelException::nullOperand,
            "BPlusTree: RecordLeaf: Op: '<='",
            __FILE__,
            __LINE__ );
  }

  time1 = rec.getTime();
  type1 = rec.getTypeOrdered();
  time2 = rl.rec.getTime();
  type2 = rl.rec.getTypeOrdered();

  return ( ( time1 < time2 ) ||
           ( ( time1 == time2 ) && ( type1 < type2 ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord < rl.ord ) ) ||
           ( ( time1 == time2 ) && ( type1 == type2 ) && ( ord == rl.ord ) )
         );
}

// ---[ I/O METHODS ]---------------------------------------------------------

/*
void RecordLeaf::Print( string indent )
{
  string msg;
  unsigned int aux_order;

  switch(get_type_ordered(rec))
  {
    case (unsigned int)0: msg = "STEND"; break;
    case (unsigned int)1: msg = "P RCV"; break;
    case (unsigned int)2: msg = "R RCV"; break;
    case (unsigned int)3: msg = "R SND"; break;
    case (unsigned int)4: msg = "L SND"; break;
    case (unsigned int)5: msg = "P SND"; break;
    case (unsigned int)6: msg = "L RCV-EVNT"; break;
    case (unsigned int)7: msg = "GLOBC"; break;
    case (unsigned int)8: msg = "STBEG"; break;
    case (unsigned int)9: msg = "?????"; break;
  }
  cout.setf( ios::right, ios::adjustfield);
  cout << indent   << '[';
  cout << setw(8)  << Get_time(rec);
  cout << setw(3)  << " - ";
  cout << setw(10) << msg;
  cout << setw(3)  << " - ";
  cout << setw(8)  << ord;
  cout << setw(3)  << " ]";
}

*/
ostream& operator<<( ostream& os, const RecordLeaf& rl )
{
  string msg;

  switch ( rl.rec.getTypeOrdered() )
  {
    case ( unsigned int )0:
      msg = "STEND";
      break;
    case ( unsigned int )1:
      msg = "P RCV";
      break;
    case ( unsigned int )2:
      msg = "R RCV";
      break;
    case ( unsigned int )3:
      msg = "R SND";
      break;
    case ( unsigned int )4:
      msg = "L SND";
      break;
    case ( unsigned int )5:
      msg = "P SND";
      break;
    case ( unsigned int )6:
      msg = "L RCV-EVNT";
      break;
    case ( unsigned int )7:
      msg = "GLOBC";
      break;
    case ( unsigned int )8:
      msg = "STBEG";
      break;
    case ( unsigned int )9:
      msg = "?????";
      break;
  }
  os.setf( ios::right, ios::adjustfield );
  os << '[';
  os << setw( 8 )  << rl.rec.getTime();
  os << setw( 3 )  << " - ";
  os << setw( 10 ) << msg;
  os << setw( 3 )  << " - ";
  os << setw( 8 )  << rl.ord;
  os << setw( 3 );
  os  << " ]";

  return os << "\n";
}
