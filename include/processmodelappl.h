#ifndef PROCESSMODELAPPL_H_INCLUDED
#define PROCESSMODELAPPL_H_INCLUDED

#include <vector>
#include "processmodeltask.h"

class ProcessModelAppl
{

  public:
    ProcessModelAppl( TApplOrder order = 0 ): traceGlobalOrder( order ) {};

    ~ProcessModelAppl() {};

  protected:
    TApplOrder traceGlobalOrder;
    vector<ProcessModelTask> tasks;

  private:

    friend class ProcessModel;
};

#endif // PROCESSMODELAPPL_H_INCLUDED
