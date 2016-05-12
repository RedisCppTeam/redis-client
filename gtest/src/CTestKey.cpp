#include "CTestKey.h"

CRedisClient CTestKey::redis;

void CTestKey::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestKey::TearDownTestCase()
{

}

TEST_F(CTestKey, del)
{
    redis.flushall();
    redis.set("key", "value");
    EXPECT_EQ(1, redis.del(VecString{"key"}));
    EXPECT_EQ(0, redis.del(VecString{"key"}));
}

