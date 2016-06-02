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




TEST_F(CTestHash, HDEL)
{
    redis.flushall();

    redis.hset("member", "name4", "testname4");
    redis.hset("member", "name5", "testname5");

    VecString fields;
    fields.push_back( "name4" );
    fields.push_back( "name5" );

    uint64_t delNum = redis.hdel( "member", fields );
    EXPECT_EQ(2, delNum);
}


TEST_F(CTestHash, HEXISTS)
{
    redis.flushall();

    string field  = "name4";
    bool isExists = redis.hexists( "testHash", field );

    EXPECT_EQ(0, isExists);
}


TEST_F(CTestHash, HGET)
{
    redis.flushall();

    EXPECT_EQ(true, redis.hset("hash", "key1", "value1"));
    string value;
    redis.hget("hash", "key1", value);
    EXPECT_EQ("value1", value);
}


TEST_F(CTestHash, HGETALL)
{
    redis.flushall();


    redis.hset("testHash", "name4", "testname4");
    redis.hset("testHash", "name5", "testname5");


    VecTupleString allValue;
    redis.hgetall( "testHash", allValue );
    VecTupleString::const_iterator it = allValue.begin();

    for ( int i = 0; it != allValue.end(); it++, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("name4", std::get<0>(*it));
            EXPECT_EQ("testname4", std::get<1>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("name5", std::get<0>(*it));
            EXPECT_EQ("testname5", std::get<1>(*it));
        }
    }


}

TEST_F(CTestHash, HINCRBY)
{
    redis.flushall();

    uint64_t incrybyNum = redis.hincrby( "testHash", "num2", 20 );
    EXPECT_EQ(20, incrybyNum);
}


TEST_F(CTestHash, HINCRBYFLOAT)
{
    redis.flushall();

    float floatNum = redis.hincrbyfloat( "testHash", "float", 10.1e2 );
    EXPECT_EQ(1010, floatNum);
}


TEST_F(CTestHash, HKEYS)
{
    redis.flushall();

    redis.hset("testHash", "name4", "testname4");
    redis.hset("testHash", "name5", "testname5");


    VecString hkeysValue;
    uint64_t hkeysNum = redis.hkeys( "testHash", hkeysValue );
    std::cout << "hkeysNum: " << hkeysNum << std::endl;

    VecString::const_iterator hkeysit = hkeysValue.begin();
    for ( int i = 0; hkeysit != hkeysValue.end(); hkeysit++, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("name4", *hkeysit);
        }
        else if (i == 1)
        {
            EXPECT_EQ("name5", *hkeysit);
        }
    }

}


TEST_F(CTestHash, HLEN)
{
    redis.flushall();

    redis.hset("testHash", "name4", "testname4");
    redis.hset("testHash", "name5", "testname5");

    uint64_t fieldNum = redis.hlen( "testHash" );
    EXPECT_EQ(2, fieldNum);
}


TEST_F(CTestHash, HMGET)
{
    redis.flushall();


    redis.hset("testHash", "name4", "testname4");
    redis.hset("testHash", "yuhaiyang", "testname5");

    CResult result;
    VecString hmgeFields;
    hmgeFields.push_back("name4");
    hmgeFields.push_back("yuhaiyang");
    redis.hmget( "testHash", hmgeFields,result );


    CResult::ListCResult::const_iterator it = result.getArry().begin();
    CResult::ListCResult::const_iterator end = result.getArry().end();
    for ( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("testname4", static_cast<string>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("testname5", static_cast<string>(*it));
        }
    }


}

TEST_F(CTestHash, HMSET)
{
    redis.flushall();


    VecTupleString pairs{std::tuple<string,string>("name4","testname4"),
                std::tuple<string,string>("name5","testname5")};

    string value;
    redis.hmset("testHash", pairs);

    redis.hget("testHash", "name4", value);
    EXPECT_EQ("testname4", value);
}

TEST_F(CTestHash, HSET)
{
    redis.flushall();

    EXPECT_EQ(true, redis.hset("hash", "key1", "value1"));
    EXPECT_EQ(false, redis.hset("hash", "key1", "value1"));
}

TEST_F(CTestHash, HSETNX)
{
    redis.flushall();

    bool hsetNxRet = redis.hsetnx( "testHash", "num4", "123" );
    EXPECT_EQ(true, hsetNxRet);
}


TEST_F(CTestHash, HVALS)
{
    redis.flushall();

    redis.hset("testHash", "name4", "testname4");
    redis.hset("testHash", "name5", "testname5");


    VecString hvals;
    redis.hvals( "testHash", hvals );

    VecString::const_iterator it = hvals.begin();
    VecString::const_iterator end = hvals.end();
    for ( int i = 0; it != end; ++it, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("testname4", static_cast<string>(*it));
        }
        else if (i == 1)
        {
            EXPECT_EQ("testname5", static_cast<string>(*it));
        }
    }


}

TEST_F(CTestHash, HSCAN)
{
    redis.flushall();


    redis.hset("testHash", "pair_11", "name_11");
    redis.hset("testHash", "pair_12", "name_12");


    VecTupleString hscanPairs;
    int64_t cursor = 0;
    while ( redis.hscan( "testHash", cursor, hscanPairs ,"pair_1?") );

    VecTupleString::const_iterator hscanIt = hscanPairs.begin();
    VecTupleString::const_iterator hscanEnd = hscanPairs.end();

    for ( int i = 0; hscanIt != hscanEnd ; ++hscanIt, ++i )
    {
        if (i == 0)
        {
            EXPECT_EQ("pair_11", std::get<0>(*hscanIt));
            EXPECT_EQ("name_11", std::get<1>(*hscanIt));
        }
        else if (i == 1)
        {
            EXPECT_EQ("pair_12", std::get<0>(*hscanIt));
            EXPECT_EQ("name_12", std::get<1>(*hscanIt));
        }

    }


}































