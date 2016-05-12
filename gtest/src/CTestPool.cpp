#include "CTestPool.h"


void CTestPool::SetUpTestCase()
{

}

void CTestPool::TearDownTestCase()
{

}

TEST_F(CTestPool, construction)
{
    CRedisPool redispool;
}

TEST_F(CTestPool, init)
{
    CRedisPool redispool;
    redispool.init("127.0.0.1", 6379, "", 0, 10, 1);
    sleep(1);
}

TEST_F(CTestPool, closeConnPool)
{
    CRedisPool redispool;
    redispool.init("127.0.0.1", 6379, "", 0, 10, 1);
    sleep(1);
    redispool.closeConnPool();
}

TEST_F(CTestPool, getRedis)
{
    CRedisClient redis;
    redis.connect("127.0.0.1");
    redis.flushall();
    CRedisPool redispool;
    redispool.init("127.0.0.1", 6379, "", 0, 3, 1);
    ASSERT_NO_THROW({
                        auto predis = redispool.getRedis(1000);
                        predis->set("key", "value");
                        string value;
                        predis->get("key", value);
                        EXPECT_EQ(value, "value");
                    });
    CRedisPool::Handle predis1, predis2, predis3, predis4;
    ASSERT_NO_THROW({
                        predis1 = redispool.getRedis(1000);
                    });
    ASSERT_NO_THROW({
                        predis2 = redispool.getRedis(1000);
                    });
    ASSERT_NO_THROW({
                        predis3 = redispool.getRedis(1000);
                    });

    ASSERT_THROW({
                     predis1 = redispool.getRedis(1000);
                 }, std::exception);

}
