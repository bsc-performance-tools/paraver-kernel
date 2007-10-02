#ifndef FILTERMANAGEMENT_H_INCLUDED
#define FILTERMANAGEMENT_H_INCLUDED

#include <string>
#include "filter.h"

class FilterManagement
{
  public:
    static FilterFunction *createFunction( const string& functionName );

  protected:

  private:

};


#endif // FILTERMANAGEMENT_H_INCLUDED
