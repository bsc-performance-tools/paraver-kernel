#ifndef PREVIOUSFILES_H_INCLUDED
#define PREVIOUSFILES_H_INCLUDED

#include <string>
#include <fstream>
using namespace std;
#include <set>

#include "paraverkerneltypes.h"

class PreviousFiles
{
  public:
    PreviousFiles( const string &filename );
    ~PreviousFiles();

    bool add( string newFile );
    const set<string>& getFiles() const;

  private:
    static const UINT16 SIZE = 20;

    fstream myFile;
    string  myFileName;
    set < string > listFiles;

    void create();
    bool update();
    void read( fstream &myFile );
};


#endif // PREVIOUSFILES_H_INCLUDED
