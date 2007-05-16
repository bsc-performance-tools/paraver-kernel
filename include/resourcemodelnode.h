#ifndef RESOURCEMODELNODE_H_INCLUDED
#define RESOURCEMODELNODE_H_INCLUDED

#include <vector>
#include "resourcemodelcpu.h"

using namespace std;

class ResourceModelNode
{
  public:
    ResourceModelNode( TNodeOrder order = 0 ) : traceGlobalOrder( order )
    {};

    ~ResourceModelNode()
    {};

  protected:
    TNodeOrder traceGlobalOrder;
    vector<ResourceModelCPU> CPUs;

  private:
    friend class ResourceModel;

};


#endif // RESOURCEMODELNODE_H_INCLUDED
