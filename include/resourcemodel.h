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
    {}

    bool isReady() const
    {
      return ready;
    }

    void dumpToFile( fstream& file ) const;

    TNodeOrder totalNodes() const;

    TCPUOrder totalCPUs() const;
    TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                            const TCPUOrder& inCPU ) const;
    void getCPULocation( TCPUOrder globalCPU,
                         TNodeOrder& inNode,
                         TCPUOrder& inCPU ) const;
    TCPUOrder getFirstCPU( TNodeOrder inNode ) const;
    TCPUOrder getLastCPU( TNodeOrder inNode ) const;

  protected:
    struct CPULocation
    {
      TNodeOrder node;
      TCPUOrder CPU;
    };

    vector<CPULocation> CPUs;
    vector<ResourceModelNode> nodes;
    bool ready;

  private:

};

#endif // RESOURCEMODEL_H_INCLUDED
