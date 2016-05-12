#include "CTestList.h"

CRedisClient CTestList::redis;

void CTestList::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestList::TearDownTestCase()
{

}

TEST_F(CTestList, lpush)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    EXPECT_EQ(2, redis.lpush("list", vec));
    vec.clear();
    vec.push_back("C");
    EXPECT_EQ(3, redis.lpush("list", vec));
}
