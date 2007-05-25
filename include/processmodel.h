#ifndef PROCESSMODEL_H_INCLUDED
#define PROCESSMODEL_H_INCLUDED

#include <vector>
#include <string>
#include "processmodelappl.h"

using namespace std;

class ProcessModel
{

  public:
    ProcessModel()
    {
      ready = false;
    }

    ProcessModel( istringstream& headerInfo );

    ~ProcessModel()
    {};

    bool isReady()
    {
      return ready;
    }

    TApplOrder totalApplications() const;

    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;

    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;

  protected:

    vector<ProcessModelAppl> applications;

    bool ready;

  private:

};




#endif // PROCESSMODEL_H_INCLUDED
