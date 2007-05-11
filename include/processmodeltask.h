#ifndef PROCESSMODELTASK_H_INCLUDED
#define PROCESSMODELTASK_H_INCLUDED

#include <vector>
#include "processmodelthread.h"

using namespace std;

class ProcessModelTask
{

  public:
    ProcessModelTask( TTaskOrder order = 0 ): traceGlobalOrder( order ) {};

    ~ProcessModelTask() {};

  protected:
    TTaskOrder traceGlobalOrder;
    vector<ProcessModelThread> threads;

  private:

    friend class ProcessModel;
};

#endif // PROCESSMODELTASK_H_INCLUDED
