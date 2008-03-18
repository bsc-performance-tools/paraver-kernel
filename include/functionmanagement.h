#ifndef FUNCTIONMANAGEMENT_H_INCLUDED
#define FUNCTIONMANAGEMENT_H_INCLUDED


#include <map>
#include <vector>


using namespace std;

template <class T>
class FunctionManagement
{

  private:
    static FunctionManagement *inst;
    map<string, T *> hash;
    vector<vector<T *> > groups;

    FunctionManagement( vector<string>&, vector<vector<T *> >& );


  public:
    ~FunctionManagement();

    T *getFunction( const string& ) const;
    UINT32 numGroups() const;
    void getAll( vector<T *>& ) const;
    void getAll( vector<T *>&, UINT32 whichGroup ) const;
    static FunctionManagement *getInstance();
    static FunctionManagement *getInstance( vector<string>&, vector<vector<T *> >& );

};

#include "src/functionmanagement.cpp"

#endif // FUNCTIONMANAGEMENT_H_INCLUDED
