#ifndef PROCESSMODELTHREAD_H_INCLUDED
#define PROCESSMODELTHREAD_H_INCLUDED

#include "paraverkerneltypes.h"

class ProcessModelThread
{

  public:
    ProcessModelThread( TThreadOrder order = 0 ): traceGlobalOrder( order ) {};

    ~ProcessModelThread() {};

  protected:
    TThreadOrder traceGlobalOrder;

  private:

    friend class ProcessModel;
};



#endif // PROCESSMODELTHREAD_H_INCLUDED
