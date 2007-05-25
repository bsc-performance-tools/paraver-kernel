#ifndef RESOURCEMODEL_H_INCLUDED
#define RESOURCEMODEL_H_INCLUDED

#include <vector>
#include <string>
#include "resourcemodelnode.h"

using namespace std;

class ResourceModel
{
  public:
    ResourceModel()
    {
      ready = false;
    }

    ResourceModel( istringstream& headerInfo );

    ~ResourceModel()
    {};

    bool isReady()
    {
      return ready;
    }

    TNodeOrder totalNodes() const;

    TCPUOrder totalCPUs() const;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;

  protected:
    vector<ResourceModelNode> nodes;
    bool ready;

  private:

};

#endif // RESOURCEMODEL_H_INCLUDED
