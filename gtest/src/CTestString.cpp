#include "CTestString.h"

CRedisClient CTestString::redis;

void CTestString::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestString::TearDownTestCase()
{

}

TEST_F(CTestString, set)
{
    redis.flushall();
    redis.set("string", "value");
    ASSERT_NO_THROW(redis.set("string", "value"););
}

TEST_F(CTestString, get)
{
    redis.flushall();
    redis.set("string", "value");
    string value;
    redis.get("string", value);
    EXPECT_EQ("value", value);
}
