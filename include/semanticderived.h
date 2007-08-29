#ifndef SEMANTICDERIVED_H_INCLUDED
#define SEMANTICDERIVED_H_INCLUDED

#include "semantichigh.h"

class SemanticDerived: public SemanticHigh
{
  public:
    SemanticDerived()
    {}

    ~SemanticDerived()
    {}

    virtual bool isControlDerived() = 0;

  protected:

  private:

};


#endif // SEMANTICDERIVED_H_INCLUDED
