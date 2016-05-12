#include "CTestHash.h"

CRedisClient  CTestHash::redis;

void CTestHash::SetUpTestCase()
{
    redis.connect("127.0.0.1", 6379);
}

void CTestHash::TearDownTestCase()
{

}

void CTestHash::GetVecTuple(VecTupleString &vecTup, const std::string &keyPre, uint64_t start, uint64_t end)
{
    vecTup.clear();
    string member,score;
    std::stringstream ss;
    uint64_t i;
    for ( i = start; i <= end; i++ )
    {
        member=keyPre;
        ss.str("");
        ss << i;
        score= ss.str();
        member += ss.str();
        vecTup.push_back( VecTupleString::value_type(score,member) );
    }
}

TEST_F(CTestHash, hset)
{
    redis.flushall();
    EXPECT_EQ(true, redis.hset("hash", "key1", "value1"));
    EXPECT_EQ(false, redis.hset("hash", "key1", "value1"));
}

TEST_F(CTestHash, hget)
{
    redis.flushall();
    EXPECT_EQ(true, redis.hset("hash", "key1", "value1"));
    string value;
    redis.hget("hash", "key1", value);
    EXPECT_EQ("value1", value);
}
