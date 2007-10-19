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
    {}

    bool isReady() const
    {
      return ready;
    }

    TApplOrder totalApplications() const;

    TTaskOrder totalTasks() const;
    TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                              const TTaskOrder& inTask ) const;
    void getTaskLocation( TTaskOrder globalTask,
                          TApplOrder& inAppl,
                          TTaskOrder& inTask ) const;
    TTaskOrder getFirstTask( TApplOrder inAppl ) const;
    TTaskOrder getLastTask( TApplOrder inAppl ) const;

    TThreadOrder totalThreads() const;
    TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                  const TTaskOrder& inTask,
                                  const TThreadOrder& inThread ) const;
    void getThreadLocation( TThreadOrder globalThread,
                            TApplOrder& inAppl,
                            TTaskOrder& inTask,
                            TThreadOrder& inThread ) const;
    TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const;

  protected:

    struct ThreadLocation
    {
      TApplOrder appl;
      TTaskOrder task;
      TThreadOrder thread;
    };

    struct TaskLocation
    {
      TApplOrder appl;
      TTaskOrder task;
    };

    vector<ThreadLocation> threads;
    vector<TaskLocation> tasks;
    vector<ProcessModelAppl> applications;

    bool ready;

  private:

};




#endif // PROCESSMODEL_H_INCLUDED
