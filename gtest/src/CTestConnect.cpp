#include "CTestConnect.h"

void CTestConnect::SetUpTestCase()
{

}

void CTestConnect::TearDownTestCase()
{

}

TEST_F(CTestConnect, auth)
{

    ASSERT_THROW({
                     CRedisClient redis;
                     redis.connect("127.0.0.1", 6379);
                     redis.auth("1234");
                 }, ExceptRedis);
}

TEST_F(CTestConnect, ping)
{
    ASSERT_NO_THROW({
                        CRedisClient redis;
                        redis.connect("127.0.0.1", 6379);

                        string value;
                        EXPECT_EQ(true, redis.ping(value));
                    });
}

TEST_F(CTestConnect, quit)
{
    ASSERT_NO_THROW({
                        CRedisClient redis;
                        redis.connect("127.0.0.1", 6379);
                        string value;
                        redis.quit();
                        EXPECT_EQ(false, redis.ping(value));
                    });
}

TEST_F(CTestConnect, echo)
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    string value;
    redis.echo("echo message", value);
    EXPECT_EQ("echo message", value);
}

TEST_F(CTestConnect, select)
{
    CRedisClient redis;
    redis.connect("127.0.0.1", 6379);
    redis.flushall();

    redis.set("key", "value");
    string value;
    EXPECT_EQ(true, redis.get("key", value));
    EXPECT_EQ("value", value);

    redis.select(1);
    EXPECT_EQ(false, redis.get("key", value));

    redis.select(0);
    value = "";
    EXPECT_EQ(true, redis.get("key", value));
    EXPECT_EQ("value", value);


}
