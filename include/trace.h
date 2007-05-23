#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include <string>
#include <vector>
#include "paraverkerneltypes.h"
#include "paraverkernelexception.h"
#include "processmodel.h"
#include "resourcemodel.h"

class Trace
{
  public:
    Trace()
    {
      throw ParaverKernelException( ParaverKernelException::emptytrace );
    }

    Trace( const string& whichFile );

    ~Trace();
  protected:
    ProcessModel traceProcessModel;
    ResourceModel traceResourceModel;
    TTime traceEndTime;
    TTimeUnit traceTimeUnit;

  private:
    string fileName;
    string date;
    vector<string> communicators;
};


#endif // TRACE_H_INCLUDED
