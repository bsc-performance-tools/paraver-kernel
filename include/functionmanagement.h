#include <map>
#include <vector>


using namespace std;

template <class T>
class FunctionManagement {

  private:
    static FunctionManagement *inst;
    map<string, T> hash;

    FunctionManagement(vector<string> &, vector<T> &);
    

  public:
    T *getFunction(const string&);
    static FunctionManagement *getInstance();
    static FunctionManagement *getInstance(vector<string> &, vector<T> &);

};

#include "functionmanagement.cpp"


