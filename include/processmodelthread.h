#ifndef PROCESSMODELTHREAD_H_INCLUDED
#define PROCESSMODELTHREAD_H_INCLUDED

#include "paraverkerneltypes.h"

class ProcessModelThread
{

  public:
    ProcessModelThread( TThreadOrder order = 0,
                        TNodeOrder node = 0 ):
        traceGlobalOrder( order ),
        nodeExecution( node )
    {};

    ~ProcessModelThread()
    {};

  protected:
    TThreadOrder  traceGlobalOrder;
    TNodeOrder    nodeExecution;

  private:
    friend class ProcessModel;

};

#endif // PROCESSMODELTHREAD_H_INCLUDED
