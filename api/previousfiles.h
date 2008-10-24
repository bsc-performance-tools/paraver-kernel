#ifndef PREVIOUSFILES_H_INCLUDED
#define PREVIOUSFILES_H_INCLUDED

#include <string>
#include <fstream>
using namespace std;
#include <vector>

#include "paraverkerneltypes.h"

class PreviousFiles
{
  public:
    static PreviousFiles *createPreviousTraces()
    {
      return new PreviousFiles( previousTracesFile );
    }
    static PreviousFiles *createPreviousCFGs()
    {
      return new PreviousFiles( previousCFGsFile );
    }

    ~PreviousFiles();

    bool add( const string &newFile );
    const vector<string>& getFiles() const;

    static const string previousTracesFile;
    static const string previousCFGsFile;

    static const UINT16 SIZE = 20;

  private:
    PreviousFiles( const string &filename );

    fstream myFile;
    string  myFileName;
    vector< string > listFiles;

    void create();
    bool update();
    void read( fstream &myFile );
};


#endif // PREVIOUSFILES_H_INCLUDED
