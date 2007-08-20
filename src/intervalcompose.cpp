#include "intervalcompose.h"

RecordList *IntervalCompose::init( TRecordTime initialTime, TCreateList create,
                                   RecordList *displayList )
{
  createList = create;
  currentValue = 0;

  if ( displayList == NULL )
    displayList = &myDisplayList;

  return displayList;
}


RecordList *IntervalCompose::calcNext( RecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  return displayList;
}


RecordList *IntervalCompose::calcPrev( RecordList *displayList, bool initCalc )
{
  if ( displayList == NULL )
    displayList = &myDisplayList;

  return displayList;
}
