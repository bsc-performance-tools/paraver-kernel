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

    vector<string> nameGroups;
    map<string, T *> hash;
    vector<vector<T *> > groups;

    FunctionManagement( vector<string>&,
                        vector<string>&,
                        vector<vector<T *> >& );


  public:
    static FunctionManagement *getInstance();
    static FunctionManagement *getInstance( vector<string>&,
                                            vector<string>&,
                                            vector<vector<T *> >& );

    ~FunctionManagement();

    T *getFunction( const string& ) const;
    UINT32 numGroups() const;
    void getNameGroups( vector<string>& );
    void getAll( vector<T *>& ) const;
    void getAll( vector<T *>&, UINT32 ) const;
    void getAll( vector<string>& ) const;
    void getAll( vector<string>&, UINT32 ) const;
};

#include "src/functionmanagement.cpp"

#endif // FUNCTIONMANAGEMENT_H_INCLUDED
