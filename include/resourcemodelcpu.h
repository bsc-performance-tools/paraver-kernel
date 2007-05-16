#ifndef RESOURCEMODELCPU_H_INCLUDED
#define RESOURCEMODELCPU_H_INCLUDED

#include "paraverkerneltypes.h"

class ResourceModelCPU
{
  public:
    ResourceModelCPU( TCPUOrder order = 0 ): traceGlobalOrder( order )
    {};

    ~ResourceModelCPU()
    {};

  protected:
    TCPUOrder traceGlobalOrder;

  private:
    friend class ResourceModel;

};


#endif // RESOURCEMODELCPU_H_INCLUDED
