#ifndef SEMANTICMANAGEMENT_H_INCLUDED
#define SEMANTICMANAGEMENT_H_INCLUDED

#include <string>
#include "semanticfunction.h"

class SemanticManagement
{
  public:
    static SemanticFunction *createFunction( const string& functionName );

  protected:

  private:

};


#endif // SEMANTICMANAGEMENT_H_INCLUDED
