#include "CTestSortedSet.h"



void CTestSortedSet::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestSortedSet::TearDownTestCase()
{

}

void CTestSortedSet::SetVecTuple(CRedisClient::VecTuple &map, const std::string &mapName, uint64_t start, uint64_t end)
{
    map.clear();
    string member,score;
    std::stringstream ss;
    uint64_t i;
    for ( i = start; i <= end; i++ )
    {
        member=mapName;
        ss.str("");
        ss << i;
        score= ss.str();
        member += ss.str();
        map.push_back( CRedisClient::VecTuple::value_type(score,member) );
    }
}


TEST_F(CTestSortedSet, zadd)
{
    //TestStringMain();
}
