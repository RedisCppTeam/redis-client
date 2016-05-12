#include "CTestSet.h"

CRedisClient CTestSet::redis;

void CTestSet::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestSet::TearDownTestCase()
{

}

TEST_F(CTestSet, sadd)
{
    redis.flushall();
    VecString vec;
    vec.push_back("A");
    vec.push_back("B");
    vec.push_back("A");
    EXPECT_EQ(2, redis.sadd("set", vec));
    EXPECT_EQ(1, redis.sadd("set", VecString{"C"}));
    EXPECT_EQ(0, redis.sadd("set", VecString{"A"}));
}
