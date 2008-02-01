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

    FunctionManagement( vector<string> &, vector<T *> & );


  public:
    ~FunctionManagement();

    T *getFunction( const string& );
    static FunctionManagement *getInstance();
    static FunctionManagement *getInstance( vector<string> &, vector<T *> & );
    static vector<T> *getAll( );
};

#include "src/functionmanagement.cpp"

#endif // FUNCTIONMANAGEMENT_H_INCLUDED
