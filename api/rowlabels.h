#ifndef ROWLABELS_H_INCLUDED
#define ROWLABELS_H_INCLUDED

#include <string>
#include "paraverkerneltypes.h"

class RowLabels
{
  public:
    RowLabels();
    RowLabels( const string& filename );
    ~RowLabels();

    string getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const;

  protected:

  private:
    vector<string> workload;
    vector<string> appl;
    vector<string> task;
    vector<string> thread;

    vector<string> system;
    vector<string> node;
    vector<string> cpu;
};


#endif // ROWLABELS_H_INCLUDED
