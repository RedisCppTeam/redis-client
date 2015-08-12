#include "test.h"
using namespace std;
Test::Test()
{
    re.connect( "127.0.0.1", 6379 );
    key1="sortedSet1";
}
void Test::clearAll()
{
    re.flushall();
}
void Test::print(const string &cmd,uint64_t num)
{
    cout<<cmd<<":"<<num<<endl;
}
void Test::print(const string &cmd,string& str)
{
    cout<<cmd<<":"<<str<<endl;
}
void Test::print(const string &cmd,VEC vec)
{
    cout<<cmd<<":"<<endl;
    VEC::const_iterator it = vec.begin();
    VEC::const_iterator end = vec.end();
    for ( ; it != end; ++it )
    {
        cout<<*it<<endl;
    }
}
void Test::print(const string &cmd,MAP map)
{
    cout<<cmd<<":"<<endl;
    MAP::const_iterator it = map.begin();
    MAP::const_iterator end = map.end();
    for ( ; it != end; ++it )
    {
        cout << it->first << ":     " << it->second << endl;
    }
}



